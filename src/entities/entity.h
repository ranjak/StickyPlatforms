#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <memory>
#include <vector>
#include "gamevector.h"
#include "rect.h"
#include "graphics.h"
#include "component.h"

namespace game {

class GameState;
class Display;
class Camera;
class Graphics;
class Tile;
class EntityManager;
class EntityFactory;

using EntityID = int;

class Entity
{
public:
  static const EntityID none = -1;

  virtual ~Entity();

  /**
   * @brief update Update this entity, making its simulation advance by \p step.
   * @param step Time in milliseconds to simulate.
   * @param game Currently running game.
   */
  virtual void update(std::uint32_t step, GameState& game);

  /**
   * @brief draw Draw this entity on \p target, if applicable.
   * @param target Render target on which the entity will be drawn.
   */
  virtual void draw(Display& target, const Camera& camera) const;

  /**
   * @brief sendMessage Send a message to the components of this entity.
   * @param message
   */
  void sendMessage(std::unique_ptr<Message> message);

  void addComponent(std::unique_ptr<Component> component);

  template<typename CompType>
  CompType *getComponent()
  {
    for (const std::unique_ptr<Component> &comp : mComponents) {
      CompType *typedComp = dynamic_cast<CompType *>(comp.get());
      if (typedComp)
        return typedComp;
    }
    return nullptr;
  }

  bool isEnabled() const { return mIsEnabled; }
  void setEnabled(bool enabled);

  bool isDead() const { return mIsDead; }
  void kill() { mIsDead = true; }

  void addChild(Entity &child);
  void removeChild(EntityID child);
  void detach();

  Entity *getParent();

  EntityManager &manager() { return mContainer; }

  Vector<float> getLocalPos() const;
  Vector<float> getGlobalPos() const;

  void setLocalPos(const Vector<float>& newPos);
  void setGlobalPos(const Vector<float> &newPos);

  Rect<float> &getLocalBox();
  const Rect<float> &getLocalBox() const;
  Rect<float> getGlobalBox() const;

  const std::string &getName() const { return mName; }

  friend std::ostream &operator<<(std::ostream &os, const Entity &e)
  {
    return os << "Entity(id="<<e.id<<",name="<<e.mName<<")";
  }

private:
  Entity(EntityID id, EntityManager &container, int x, int y, int w=0, int h=0, const std::string &name="", std::unique_ptr<Graphics> graphs=nullptr, EntityID parent=none);
  Entity(EntityID id, EntityManager &container, const Rect<float> &boundingBox, const std::string &name="", std::unique_ptr<Graphics> graphs=nullptr, EntityID parent=none);
  friend class EntityManager;

public:
  const EntityID id;

protected:
  bool mIsEnabled;
  bool mIsDead;
  Rect<float> mBoundingBox;
  std::unique_ptr<Graphics> mGraphics;
  EntityID mParent;
  std::vector<EntityID> mChildren;
  std::string mName;
  std::vector<std::unique_ptr<Component>> mComponents;
  EntityManager &mContainer;
};

} // namespace game

#endif // ENTITY_H
