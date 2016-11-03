#ifndef CAMERACONTROLMSG_H
#define CAMERACONTROLMSG_H

#include "message.h"

namespace game {

/**
 * @brief Change some parameters of a CameraAnchoredY object.
 * The coordinates and sizes are expressed in tile coordinates, not in pixels.
 * A negative value for a parameter means this parameter is to be left unchanged in the camera controller.
 */
struct CameraControlMsg : public Message
{
  CameraControlMsg(float baseY, float topMargin, float bottomMargin) : Message(CameraControl), baseY(baseY), topMargin(topMargin), bottomMargin(bottomMargin) {}

  float baseY;
  float topMargin;
  float bottomMargin;
};

} // namespace game

#endif // CAMERACONTROLMSG_H
