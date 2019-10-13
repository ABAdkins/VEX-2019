#include "main.h"
#include "User/Constants.h"
#include "User/Command.h"
#include "User/CommandRunner.h"
#include "User/CommandFactory.h"
#include "User/DriveSubsystem.h"
#include "User/TraySubsystem.h"
#include "User/AutoSelector.h"
#include "User/DriveDistanceCommand.h"
#include "User/DriveAndIntakeCommand.h"
#include "User/DriveTurnCommand.h"
#include "User/MoveTrayCommand.h"

int SELECTED_AUTO;

okapi::Controller driver (okapi::ControllerId::master);
okapi::Controller operatorController (okapi::ControllerId::partner);

AutoSelector autoSelector("Do Nothing", DO_NOTHING_AUTO);
std::shared_ptr<DriveSubsystem> drive;
std::shared_ptr<TraySubsystem> tray;

void initialize() {
  drive.reset(new DriveSubsystem(driver));
  tray.reset(new TraySubsystem(driver, operatorController));
  autoSelector.registerAuto("DRIVE FORWARD AUTO", DRIVE_FORWARD_AUTO);
  autoSelector.registerAuto("SMALL RED ZONE", SMALL_RED);
  autoSelector.registerAuto("SMALL BLUE ZONE", SMALL_BLUE);
  autoSelector.listOptions();
}


void disabled() {}


void competition_initialize() {
  while (true) {
    SELECTED_AUTO = autoSelector.getSelectedAuto();
    pros::delay(20);
  }
}

void autonomous() {
  tray->reset();
  drive->reset();
  switch(SELECTED_AUTO) {
    case DO_NOTHING_AUTO:
    break;
    case DRIVE_FORWARD_AUTO:
    CommandRunner::runCommand(new DriveTurnCommand(drive, 90, 75, 1));

      break;
    case SMALL_RED:
      CommandRunner::runCommand(new DriveAndIntakeCommand(drive, tray, 40, 100, 3));
      CommandRunner::runCommand(new DriveDistanceCommand(drive, -24, 75, 3));
      CommandRunner::runCommand(new DriveTurnCommand(drive, 150, 75, 1));
      CommandRunner::runCommand(new DriveDistanceCommand(drive, 40, 100, 3));
      CommandRunner::runCommand(new DriveTurnCommand(drive, -150, 75, 1));
      CommandRunner::runCommand(new DriveAndIntakeCommand(drive, tray, 24, 100, 3));
      CommandRunner::runCommand(new DriveTurnCommand(drive, 90, 75, 1));
      CommandRunner::runCommand(new DriveDistanceCommand(drive, 24, 100, 2));
      CommandRunner::runCommand(new DriveTurnCommand(drive, 90, 120, 1.5));
      CommandRunner::runCommand(new DriveDistanceCommand(drive, 48, 100, 3));
      //CommandRunner::runCommand(new MoveTrayCommand(tray, TraySubsystem::TrayPosition::kSlant, 3));
      CommandRunner::runCommand(new DriveDistanceCommand(drive, -12, 100, 3));
      break;
    default:
      break;
  }
  tray->stop();
  drive->stop();
}

void opcontrol() {
 	while (true) {
 		std::printf("Running");
 		drive->update();
    tray->update();
 		pros::delay(20);
 	}
  tray->stop();
 	drive->stop();
 }
