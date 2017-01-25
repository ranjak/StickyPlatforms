#include "stormancerconnection.h"
#include "stormancer.h"

namespace
{
  const std::string endpoint = "https://api.stormancer.com/";
  const std::string account = "ranjak";
  const std::string application = "stickyplatforms";
}

namespace game
{

StormancerConnection::StormancerConnection(const std::string & username, const std::string & sceneName) :
  pimpl(new impl)
{
  auto taskResult = pimpl->client->getPublicScene(sceneName.c_str()).then([](auto result)
  {
    if (!result->success()) {
      throw std::exception(sceneResult->reason());
    }
    return result->get();
  });

  pimpl->scene.reset(taskResult.get());
  pimpl->scene->connect();
}

class StormancerConnection::impl
{
public:
  impl() :
    config(Stormancer::Configuration::create(endpoint, account, application)),
    client(Stormancer::Client::createClient(config), Stormancer::destroy),
    scene(nullptr, Stormancer::destroy)
    {}

public:
  std::shared_ptr<Stormancer::Configuration> config;
  std::unique_ptr<Stormancer::Client, void(*)(Stormancer::Client*)> client;
  std::unique_ptr<Stormancer::Scene, void(*)(Stormancer::Scene*)> scene;
};

} // namespace game