#ifndef STORMANCERCONNECTION_H
#define STORMANCERCONNECTION_H

#include <string>
#include <memory>

namespace game
{

class StormancerConnection
{
public:
  /**
   * @brief Establish the connection with the Stormancer server.
   */
  StormancerConnection(const std::string & username, const std::string & sceneName);

private:
  class impl;
  std::unique_ptr<impl> pimpl;
};

} // namespace game
#endif // !STORMANCERCONNECTION_H
