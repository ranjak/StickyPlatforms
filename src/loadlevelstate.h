#ifndef LOADLEVELSTATE_H
#define LOADLEVELSTATE_H

#include <cstdint>
#include <string>

namespace game {

class Game;
class Display;

class LoadLevelState
{
public:
  LoadLevelState(Game &game, Display &display);

  void update(std::uint32_t step);

  void enter(bool victory, const std::string &nextLevel);

private:
  static const std::string gameOverWidget;
  static const std::string victoryWidget;

  Game &mGame;
  // Display failure or victory
  const std::string *mCurrentWidget;
  // How much time to stay in this state...
  int mTimeRemaining;
  // ...Before loading the next level
  std::string mNextLevel;
};

} // namespace game

#endif // LOADLEVELSTATE_H
