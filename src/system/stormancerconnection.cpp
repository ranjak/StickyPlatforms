#include "stormancerconnection.h"
#include "game.h"
#include "stormancer.h"
#include "msgpack_adaptors.h"
#include <vector>


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

    MSGPACK_DEFINE(name, color, position, velocity, hp)
  };

  struct SpawnMsg
  {
    Color color;
    Vector<float> pos;
    int hp;

    MSGPACK_DEFINE(color, pos, hp)
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
    // All handlers will be run inside calls to StormancerConnection::update()
    config->actionDispatcher = std::make_shared<Stormancer::MainThreadActionDispatcher>();
    //config->actionDispatcher->start();

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
      this->game.changeLevel(mapFile);
    });
  }

  impl(const impl &) = delete;
  impl &operator=(const impl &) = delete;

  void initGame()
  {
    rpc->rpc<std::vector<Player>>("getPlayerList").then([this](auto task)
    {
      const std::vector<Player> &players = unwrapResult(*task.get());

      for (const Player &player : players) {
        if (player.name != username)
          game.getLevel().entities().createRemoteEntity("RemoteHero", player.name, player.position, player.color, player.hp);
      }
    });
  }

private:
  void registerRoutes()
  {
    scene->addRoute("newPlayer", [this](Stormancer::Packetisp_ptr packet)
    {
      Player player = packet->readObject<Player>();

      if (player.name != username)
        game.getLevel().entities().createRemoteEntity("RemoteHero", player.name, player.position, player.color, player.hp);
    });

    scene->addRoute("playerLeft", [this](Stormancer::Packetisp_ptr packet)
    {
      Entity* player = game.getLevel().entities().getEntity(packet->readObject<std::string>());

      if (player)
        player->kill();
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

void StormancerConnection::update()
{
  static_cast<Stormancer::MainThreadActionDispatcher &>(*pimpl->config->actionDispatcher).update(std::chrono::milliseconds(10));
}

} // namespace game