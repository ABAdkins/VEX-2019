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
#include "User/IntakeCommand.h"
#include "User/MoveArmCommand.h"
#include "User/PIDController.h"

int SELECTED_AUTO;

okapi::Controller driver (okapi::ControllerId::master);
okapi::Controller operatorController (okapi::ControllerId::partner);

AutoSelector autoSelector("Do Nothing", DO_NOTHING_AUTO);
std::shared_ptr<DriveSubsystem> drive;
std::shared_ptr<TraySubsystem> tray;

void initialize() {
  okapi::Logger::setDefaultLogger(
    std::make_shared<okapi::Logger>(
        okapi::TimeUtilFactory::createDefault().getTimer(), // It needs a Timer
        "/ser/sout", // Output to the PROS terminal
        okapi::Logger::LogLevel::warn // Show errors and warnings
    )
);

  drive.reset(new DriveSubsystem(driver));
  tray.reset(new TraySubsystem(driver, operatorController));
  drive->reset();
  pros::delay(2000);


  autoSelector.registerAuto("Skills", SKILLS);                    // 12, 18, 30, 40, 50, push randomly
  autoSelector.registerAuto("Small Red", SMALL_RED);              //test 1 2 3
  autoSelector.registerAuto("Small Blue", SMALL_BLUE);            //test 1 2 3
  autoSelector.registerAuto("Large Red", LARGE_RED);              //good
  autoSelector.registerAuto("Large Blue", LARGE_BLUE);            //test 1 2 3
  autoSelector.registerAuto("One Cube",ONE_CUBE_AUTO);            //good
  autoSelector.registerAuto("Large Red Prep", LARGE_RED_PREP);    //test 1 2 3
  autoSelector.registerAuto("Large Blue Prep", LARGE_BLUE_PREP);  //test 1 2 3


  autoSelector.listOptions();
}

void disabled() {

}

void competition_initialize() {
  while (true) {
    SELECTED_AUTO = autoSelector.getSelectedAuto();
    pros::delay(20);
  }
}

void deployTray() {
  tray->cubeScorer.tarePosition();
  drive->driveTrain->moveDistance(3_in);
  drive->driveTrain->moveDistance(-3_in);
  tray->outtakeCube(200);
  pros::delay(1000);
  tray->outtakeCube(0);

  tray->trayMotor.tarePosition();
  tray->trayMotor.tarePosition();
  drive->initialize();
  drive->rightMotors.tarePosition();
  drive->leftMotors.tarePosition();
}

void scoreTower() {
  pros::delay(250);
  tray->outtakeCube(55);
  pros::delay(1000);
  tray->outtakeCube(0);
}

void turn(double angle, double maxSpeed, double timeout) {
  drive->driveTrain->setMaxVelocity(maxSpeed);
  okapi::Timer timer = okapi::Timer();
  timer.placeMark();

  while((abs(drive->getHeading() - angle) > .5) && ((timer.getDtFromMark().convert(okapi::second)) < (timeout))) {
    double error = (angle - drive->getHeading());
    double kP = 0.013;
    drive->tankDrive(kP*error, -kP*error, false);
    pros::delay(100);
  }
  drive->stop();
}

void stack(){
  tray->outtakeCube(55);
  pros::delay(500);
  tray->outtakeCube(25);

  CommandRunner::runCommand(new MoveTrayCommand(tray, TraySubsystem::TrayPosition::kStack, 90), 3);
  tray->outtakeCube(100);
  drive->driveTrain->setMaxVelocity(100);
  drive->driveTrain->moveDistance(-8_in);
  tray->outtakeCube(0);
  CommandRunner::runCommand(new MoveTrayCommand(tray, TraySubsystem::TrayPosition::kSlant, 100), 3);
  tray->trayMotor.moveVelocity(0);
  pros::delay(500);
  drive->driveTrain->stop();
}

void stackSlowly(){
  tray->outtakeCube(55);
  pros::delay(500);
  tray->outtakeCube(25);
  tray->trayMotor.moveVelocity(100);

  CommandRunner::runCommand(new MoveTrayCommand(tray, TraySubsystem::TrayPosition::kStack, 45), 5);
  tray->outtakeCube(100);
  drive->driveTrain->moveDistance(-10_in);
  tray->outtakeCube(0);
  CommandRunner::runCommand(new MoveTrayCommand(tray, TraySubsystem::TrayPosition::kSlant, 100), 3);
  tray->trayMotor.moveVelocity(0);
  pros::delay(500);
  drive->driveTrain->stop();
}

void moveSmallTower() {
  tray->cubeScorer.moveAbsolute(380, 100);
}

void autonomous() {
  tray->reset();
  drive->setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
  drive->reset();

  switch(SELECTED_AUTO) {
    case DO_NOTHING_AUTO:
      while(true) {
        std::cout << drive->getHeading() << std::flush;
        std::cout << "" << std::flush;
        pros::delay(1000);
        drive->stop();
        break;
    }

    case ONE_CUBE_AUTO:
      deployTray();

      drive->driveTrain->setMaxVelocity(65);
      drive->driveTrain->moveDistance(20_in);
      tray->outtakeCube(100);
      pros::delay(500);
      drive->driveTrain->moveDistance(-20_in);
      turn(45, 100, 2);
      tray->outtakeCube(0);

      drive->stop();
      break;

    case SMALL_RED:
      deployTray();
      drive->reset();
      tray->intakeCube();
      drive->driveTrain->setMaxVelocity(110);
      drive->driveTrain->moveDistance(43_in);
      turn(-19, 100, 0.75);
      drive->driveTrain->setMaxVelocity(50);
       //got cube 1
      drive->driveTrain->moveDistance(7_in);

      drive->driveTrain->moveDistance(-12_in);
      turn(120, 50, 1.0);
      turn(153, 10, 1.5);

      drive->driveTrain->setMaxVelocity(100);
      drive->driveTrain->moveDistance(31_in);
      stack();
      drive->stop();
      break;

    case SMALL_BLUE:
      deployTray();
      drive->reset();
      tray->intakeCube();
      drive->driveTrain->setMaxVelocity(110);
      drive->driveTrain->moveDistance(43_in);
      turn(19, 100, 0.75);
      drive->driveTrain->setMaxVelocity(50);
      //got cube 1
      drive->driveTrain->moveDistance(7_in);

      drive->driveTrain->moveDistance(-12_in);
      turn(-120, 50, 1.0);
      turn(-153, 10, 1.5);

      drive->driveTrain->setMaxVelocity(100);
      drive->driveTrain->moveDistance(31_in);
      stack();
      pros::delay(15000);
      drive->stop();
      break;

    case LARGE_RED:
      drive->reset();
      deployTray();
      tray->intakeCube();
      drive->driveTrain->setMaxVelocity(125);
      drive->driveTrain->moveDistance(30_in);
      drive->driveTrain->moveDistance(-3_in);
      turn(90, 40, 1.5);
      drive->driveTrain->moveDistance(12_in);
      drive->driveTrain->moveDistance(-2_in);
      turn(-90, 30, 2.5);
      drive->driveTrain->setMaxVelocity(150);
      drive->driveTrain->moveDistance(28_in);
      tray->outtakeCube(0);
      turn(-130, 50, 1);

      drive->driveTrain->moveDistance(10_in);

      stack();
      drive->stop();
      break;

    case LARGE_BLUE:
      drive->reset();
      deployTray();
      tray->intakeCube();
      drive->driveTrain->setMaxVelocity(125);
      drive->driveTrain->moveDistance(30_in);
      drive->driveTrain->moveDistance(-3_in);
      turn(-90, 40, 1.4);
      drive->driveTrain->setMaxVelocity(100);
      drive->driveTrain->moveDistance(12_in);
      drive->driveTrain->moveDistance(-2_in);
      turn(90, 30, 2);
      drive->driveTrain->setMaxVelocity(150);
      drive->driveTrain->moveDistance(30_in);
      tray->outtakeCube(0);
      turn(130, 50, 1);

      drive->driveTrain->setMaxVelocity(100);
      drive->driveTrain->moveDistance(10_in);

      stack();
      drive->stop();
      break;

    case LARGE_RED_PREP:
      drive->reset();
      deployTray();
      tray->intakeCube();
      drive->driveTrain->setMaxVelocity(125);
      drive->driveTrain->moveDistance(25_in);
      turn(45, 40, 1);
      turn(90, 40, 1);
      drive->driveTrain->moveDistance(35_in);
      drive->driveTrain->moveDistance(-3_in);
      drive->driveTrain->setMaxVelocity(100);
      turn(75, 40, 1.5);
      drive->driveTrain->moveDistance(10_in);
      drive->driveTrain->moveDistance(-10_in);
      tray->outtakeCube(0);
      turn(90, 40, 1.5);

      drive->stop();
      break;

    case LARGE_BLUE_PREP:
      drive->reset();
      deployTray();
      tray->intakeCube();
      drive->driveTrain->setMaxVelocity(125);
      drive->driveTrain->moveDistance(25_in);
      turn(-45, 40, 1);
      turn(-90, 40, 1);
      drive->driveTrain->moveDistance(35_in);
      drive->driveTrain->moveDistance(-3_in);
      drive->driveTrain->setMaxVelocity(100);
      turn(-75, 40, 1.5);
      drive->driveTrain->moveDistance(10_in);
      drive->driveTrain->moveDistance(-10_in);
      tray->outtakeCube(0);
      turn(-90, 40, 1.5);

      drive->stop();
      break;

    case SKILLS:
    deployTray();

    tray->intakeCube();
    drive->driveTrain->setMaxVelocity(50);
    drive->driveTrain->moveDistance(11_in);

    tray->outtakeCube(0);
    pros::delay(200);
    tray->outtakeCube(80);
    pros::delay(500);
    tray->outtakeCube(0);

    turn(-47, 60, 1);
    drive->driveTrain->moveDistance(7_in);

    tray->cubeScorer.moveAbsolute(300, 100);
    pros::delay(1200);
    drive->driveTrain->setMaxVelocity(40);
    drive->driveTrain->moveDistance(11_in);
    tray->outtakeCube(100);
    pros::delay(1000);

    drive->driveTrain->setMaxVelocity(50);
    drive->driveTrain->moveDistance(-11_in);
    tray->outtakeCube(0);
    tray->cubeScorer.moveAbsolute(0, 100);
    drive->driveTrain->moveDistance(-4_in);

    turn(0, 50, 1.5);
    drive->driveTrain->moveDistance(-7_in);
    pros::delay(100);
    //first tower scored


    //get first 4 cubes
    tray->intakeCube();
    drive->driveTrain->setMaxVelocity(110);
    drive->driveTrain->moveDistance(38_in);
    pros::delay(750);
    drive->driveTrain->setMaxVelocity(50);
    drive->driveTrain->moveDistance(-5_in);
    //turn and get cube 5
    turn(20, 40, 1.25);
    drive->driveTrain->moveDistance(8_in);
    drive->driveTrain->moveDistance(-9_in);
    //turn and get cube 6
    turn(0, 35, 1);
    drive->driveTrain->setMaxVelocity(36);
    drive->driveTrain->moveDistance(23_in);
    //go to 0
    turn(0, 50, 0.75);

    drive->driveTrain->moveDistance(-32_in);
    tray->outtakeCube(0);
    turn(-90, 10, 1.5);
    turn(-135, 10, 1.5);
    drive->driveTrain->setMaxVelocity(50);
    drive->driveTrain->moveDistance(12_in);
    stackSlowly();

    //line up for tower 2
    turn(90, 10, 3);
    drive->driveTrain->moveDistance(-15_in);
    //go to tower 2
    tray->intakeCube();
    drive->driveTrain->moveDistance(35_in);


    drive->driveTrain->stop();
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
