#include "cameracontroller.h"
#include "rect.h"
#include "entity.h"
#include "camera.h"
#include "gamevector.h"

namespace game {

void CameraFollowSimple::update(uint32_t step, Entity &entity, Camera &camera)
{
  camera.viewport().setCenter(entity.getGlobalBox().getCenter());
}

} // namespace game
