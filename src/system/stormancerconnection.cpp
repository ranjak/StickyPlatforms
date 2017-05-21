#include "stormancerconnection.h"
#include "game.h"
#include "stormancer.h"
#include "msgpack_adaptors.h"
#include "gamecommands.h"
#include "remoteinputcomponent.h"
#include <vector>
#include <array>
#include <algorithm>


namespace game
{
namespace
{
  const std::string endpoint = "https://api.stormancer.com/";
  const std::string account = "ranjak";
  const std::string application = "stickyplatforms";

  template<typename T>
  T unwrapResult(Stormancer::Result<T> &result, const std::string &errorPrefix = "Stormancer error: ")
  {
    if (!result.success()) {
      throw std::exception(result.reason());
    }
    return result.get();
  }

  void unwrapResult(Stormancer::Result<void> &result, const std::string &errorPrefix = "Stormancer error: ")
  {
    if (!result.success()) {
      throw std::exception(result.reason());
    }
  }

  struct Player
  {
    std::string name;
    Color color;
    Vector<float> position;
    Vector<float> velocity;
    int hp;
    std::vector<bool> keyStatus;

    MSGPACK_DEFINE(name, color, position, velocity, hp, keyStatus)
  };

  struct SpawnMsg
  {
    Color color;
    Vector<float> pos;
    int hp;

    MSGPACK_DEFINE(color, pos, hp)
  };

  struct InputUpdate
  {
    std::string name;
    std::vector<int> pressed;
    std::vector<int> released;
    Vector<float> pos;

    MSGPACK_DEFINE(name, pressed, released, pos)
  };
}

class StormancerConnection::impl
{
public:
  impl(const std::string & username, const std::string & sceneName, Game &game) :
    config(Stormancer::Configuration::create(endpoint, account, application)),
    client(nullptr, Stormancer::destroy),
    scene(nullptr, Stormancer::destroy),
    mapFile(),
    username(username),
    rpc(),
    game(game)
  {
    client.reset(Stormancer::Client::createClient(config));
    // Connect to the scene, then retrieve the scene's map filename.

    client->getPublicScene(sceneName.c_str()).then([this](Stormancer::Result<Stormancer::Scene *> *sceneResult)
    {
      scene.reset(unwrapResult(*sceneResult, "Couldn't obtain the scene: "));

      registerRoutes();
      return scene->connect();
    }
    ).then([this](Stormancer::Result<void> *connectResult)
    {
      unwrapResult(*connectResult, "Couldn't connect to the scene: ");

      rpc = scene->dependencyResolver()->resolve<Stormancer::IRpcService>();
      return rpc->rpc<std::string, std::string>("joinGame", this->username);
    }
    ).then([this](std::shared_ptr<Stormancer::Result<std::string>> joinResult)
    {
      mapFile = unwrapResult(*joinResult, "Couldn't join the game: ");
      this->game.pushEvent([this] { this->game.changeLevel(mapFile); });
    }
    ).then([this](auto task)
    {
      try {
        task.get();
      }
      catch (std::exception&) {
        std::exception_ptr exp = std::current_exception();
        this->game.pushEvent([exp] { std::rethrow_exception(exp); });
      }
    });
  }

  impl(const impl &) = delete;
  impl &operator=(const impl &) = delete;

  void initGame()
  {
    rpc->rpc<std::vector<Player>>("getPlayerList").then([this](auto task)
    {
      auto resultPtr = task.get();

      game.pushEvent([resultPtr, this]
      {
        const std::vector<Player> &players = unwrapResult(*resultPtr);

        for (const Player &player : players) {
          if (player.name != username) {
            EntityID entity = game.getLevel().entities().createRemoteEntity("RemoteHero", player.name, player.position, player.velocity, player.color, player.hp);
            
            auto input = game.getLevel().entities().getEntity(entity)->getComponent<RemoteInputComponent>();
            for (int i = 0; i < player.keyStatus.size(); i++) {
              if (player.keyStatus[i])
                input->hitCommand(i);
            }
          }
        }
      });
    });
  }

private:
  void registerRoutes()
  {
    scene->addRoute("newPlayer", [this](Stormancer::Packetisp_ptr packet)
    {
      Player player = packet->readObject<Player>();
      if (player.name == username)
        return;

      game.pushEvent([player, this]
      {
        game.getLevel().entities().createRemoteEntity("RemoteHero", player.name, player.position, player.velocity, player.color, player.hp);
      });
    });

    scene->addRoute("playerLeft", [this](Stormancer::Packetisp_ptr packet)
    {
      std::string playerName = packet->readObject<std::string>();
      
      game.pushEvent([playerName, this]
      {
        Entity* player = game.getLevel().entities().getEntity(playerName);

        if (player)
          player->kill();
      });
    });

    scene->addRoute("remoteInputUpdate", [this](Stormancer::Packetisp_ptr packet)
    {
      InputUpdate msg = packet->readObject<InputUpdate>();
      if (msg.name == username)
        return;

      game.pushEvent([msg, this]
      {
        Entity* player = game.getLevel().entities().getEntity(msg.name);

        if (!player)
          return;

        auto input = player->getComponent<RemoteInputComponent>();
        std::for_each(msg.pressed.begin(), msg.pressed.end(), [input](int key){ input->hitCommand(key); });
        std::for_each(msg.released.begin(), msg.released.end(), [input](int key) { input->releaseCommand(key); });
        // Synchronize the position when the input changes
        player->setGlobalPos(msg.pos);
      });
    });
  }

public:
  std::shared_ptr<Stormancer::Configuration> config;
  std::unique_ptr<Stormancer::Client, void(*)(Stormancer::Client*)> client;
  std::unique_ptr<Stormancer::Scene, void(*)(Stormancer::Scene*)> scene;
  std::string mapFile;
  std::string username;
  std::shared_ptr<Stormancer::IRpcService> rpc;
  Game &game;
};



StormancerConnection::StormancerConnection(const std::string & username, const std::string & sceneName, Game & game) :
  pimpl(new impl(username, sceneName, game))
{
}

StormancerConnection::~StormancerConnection() = default;

const std::string & StormancerConnection::getMapFile()
{
  return pimpl->mapFile;
}

const std::string & StormancerConnection::getUsername()
{
  return pimpl->username;
}

void StormancerConnection::initGame()
{
  pimpl->initGame();
}

void StormancerConnection::spawn(const Color &color, const Vector<float> &pos, int hp)
{
  pimpl->scene->sendPacket("spawn", [=](Stormancer::bytestream *writer)
  {
    msgpack::pack(writer, SpawnMsg{color, pos, hp});
  });
}

void StormancerConnection::updateHealth(int hp)
{
  pimpl->scene->sendPacket("updateHp", [=](Stormancer::bytestream *writer)
  {
    msgpack::pack(writer, hp);
  });
}

void StormancerConnection::updatePhysics(Vector<float> pos, Vector<float> velocity)
{
  pimpl->scene->sendPacket("updatePhysics", [=](Stormancer::bytestream *writer)
  {
    msgpack::packer<Stormancer::bytestream>(writer)
      .pack_array(2)
      .pack(pos)
      .pack(velocity);
  }, PacketPriority::LOW_PRIORITY, PacketReliability::UNRELIABLE_SEQUENCED);
}

void StormancerConnection::updateKeys(std::vector<int> pressed, std::vector<int> released)
{
  pimpl->scene->sendPacket("updateKeys", [=](Stormancer::bytestream *writer)
  {
    msgpack::packer<Stormancer::bytestream>(writer)
      .pack_array(2)
      .pack(pressed)
      .pack(released);
  }, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED);
}

} // namespace game