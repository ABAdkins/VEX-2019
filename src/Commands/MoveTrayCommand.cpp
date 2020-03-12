#include "User/MoveTrayCommand.h"
#include <cmath>
#include <math.h>

MoveTrayCommand::MoveTrayCommand(std::shared_ptr<TraySubsystem> tray, TraySubsystem::TrayPosition position, double maxSpeed) : tray(tray)
{

  this->position = position;
  this->maxSpeed = maxSpeed;
}

void MoveTrayCommand::start() {

}

void MoveTrayCommand::update() {
  double p = 0.5;
  double error = (double) position - tray->trayMotor.getPosition();
  if((error * p) > maxSpeed) {
    tray->trayMotor.moveVelocity(maxSpeed);
  } else if ((error * p) < -maxSpeed) {
    tray->trayMotor.moveVelocity(-maxSpeed);
  } else {
    tray->trayMotor.moveVelocity(error * p);
  }
}

bool MoveTrayCommand::isFinished() {
  bool hitTarget = abs((double) position - tray->trayMotor.getPosition()) < 30;
  return hitTarget;
}

void MoveTrayCommand::finish() {
  tray->stop();
}
