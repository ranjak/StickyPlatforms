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
    client(Stormancer::Client::createClient(config), Stormancer::destroy),
    scene(nullptr, Stormancer::destroy),
    mapFile(),
    username(username),
    rpc(),
    game(game)
  {
    // All handlers will be run inside calls to StormancerConnection::update()
    config->actionDispatcher = std::make_shared<Stormancer::MainThreadActionDispatcher>();
    config->actionDispatcher->start();

    // Connect to the scene, then retrieve the scene's map filename.

    auto sceneResult = client->getPublicScene(sceneName.c_str()).get();
    scene.reset(unwrapResult(*sceneResult, "Couldn't obtain the scene: "));

    auto connectResult = scene->connect().get();
    unwrapResult(*connectResult, "Couldn't connect to the scene: ");

    rpc = scene->dependencyResolver()->resolve<Stormancer::IRpcService>();

    auto getMapResult = rpc->rpc<std::string, std::string>("joinGame", username).get();
    mapFile = unwrapResult(*getMapResult, "Couldn't join the game: ");
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

    scene->addRoute("newPlayer", [this](Stormancer::Packetisp_ptr packet)
    {
      Player player = packet->readObject<Player>();

      if (player.name != username)
        game.getLevel().entities().createRemoteEntity("RemoteHero", player.name, player.position, player.color, player.hp);
    });

    scene->addRoute("playerLeft", [this](auto packet)
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