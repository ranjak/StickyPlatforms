#ifndef STORMANCERCONNECTION_H
#define STORMANCERCONNECTION_H

#include <string>
#include <memory>

namespace game
{
class Game;
class Color;
template<typename T> class Vector;

class StormancerConnection
{
public:
  /**
   * @brief Establish the connection with the Stormancer server.
   */
  StormancerConnection(const std::string & username, const std::string & sceneName, Game & game);
  ~StormancerConnection();

  StormancerConnection(const StormancerConnection &) = delete;
  StormancerConnection &operator=(const StormancerConnection &) = delete;

  const std::string &getMapFile();
  const std::string &getUsername();

  /**
   * @brief Call once the level has been loaded:
   * retrieves the list of live players.
   */
  void initGame();

  void spawn(const Color &color, const Vector<float> &pos, int hp);

  void updateHealth(int hp);

private:
  class impl;
  std::unique_ptr<impl> pimpl;
};

} // namespace game
#endif // !STORMANCERCONNECTION_H
