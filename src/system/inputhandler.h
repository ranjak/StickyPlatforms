#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <vector>
#include <cstdint>

namespace game {

enum class ModifierKey
{
  NONE,
  ALT
};

/**
 * @brief The InputHandler class is a generic interface that presents
 * system input events conveniently.
 */
class InputHandler
{
public:
  InputHandler();

  virtual ~InputHandler() {}

  /**
   * @brief Handle player input for one frame.
   * This function is responsible for managing the hit/held/released key containers.
   */
  virtual void handle() = 0;

  /**
   * @brief isModifierPressed Get the current state of a modifier key (Alt, Ctrl...)
   * @param modifier The modifier key to check.
   * @return \c true if \p modifier is currently pressed.
   */
  virtual bool isModifierPressed(ModifierKey modifier) const = 0;

  /**
   * @brief applicationHasFocus Check whether the program currently has input focus.
   * @return \c true if the application is in the foreground, \c false otherwise.
   */
  virtual bool applicationHasFocus() const = 0;

  bool isKeyHit(std::uint32_t scancode) const;
  bool isKeyHeld(std::uint32_t scancode) const;
  bool isKeyReleased(std::uint32_t scancode) const;

protected:
  // Keys that were just pressed at the time handle() was called
  std::vector<std::uint32_t> mHitKeys;
  // Keys that were in mHitKeys last time handle() was called and haven't been released
  std::vector<std::uint32_t> mHeldKeys;
  // Keys that were just released
  std::vector<std::uint32_t> mReleasedKeys;
};

}

#endif // INPUTHANDLER_H
