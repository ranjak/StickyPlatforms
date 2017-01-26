#include "stormancerconnection.h"
#include "stormancer.h"

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
}

namespace game
{

class StormancerConnection::impl
{
public:
  impl(const std::string & username, const std::string & sceneName) :
    config(Stormancer::Configuration::create(endpoint, account, application)),
    client(Stormancer::Client::createClient(config), Stormancer::destroy),
    scene(nullptr, Stormancer::destroy),
    mapFile(),
    rpc()
  {
    // Connect to the scene, then retrieve the scene's map filename.

    auto sceneResult = client->getPublicScene(sceneName.c_str()).get();
    scene.reset(unwrapResult(*sceneResult, "Couldn't obtain the scene: "));

    auto connectResult = scene->connect().get();
    unwrapResult(*connectResult, "Couldn't connect to the scene: ");

    rpc = scene->dependencyResolver()->resolve<Stormancer::IRpcService>();

    auto getMapResult = rpc->rpc<std::string, std::string>("getMapFilename", username).get();
    mapFile = unwrapResult(*getMapResult, "Couldn't join the game: ");
  }

public:
  std::shared_ptr<Stormancer::Configuration> config;
  std::unique_ptr<Stormancer::Client, void(*)(Stormancer::Client*)> client;
  std::unique_ptr<Stormancer::Scene, void(*)(Stormancer::Scene*)> scene;
  std::string mapFile;
  std::shared_ptr<Stormancer::IRpcService> rpc;
};



StormancerConnection::StormancerConnection(const std::string & username, const std::string & sceneName) :
  pimpl(new impl(username, sceneName))
{
}

StormancerConnection::~StormancerConnection() = default;

const std::string & StormancerConnection::getMapFile()
{
  return pimpl->mapFile;
}

} // namespace game