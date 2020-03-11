#ifndef TRAYSUBSYSTEM_H_
#define TRAYSUBSYSTEM_H_
#include "../main.h"
#include "Subsystem.h"
#include <iostream>

/**

 * @param driverController VEX Gamepad for driver
 * @param operatorController VEX Gamepad for Operator
 */
class TraySubsystem : public Subsystem {
public:

	TraySubsystem(okapi::Controller driverController, okapi::Controller operatorController);

	void stop();
	void update();
	void reset();
  void initialize();

	okapi::Motor cubeScorer;


	/**
	 * State of the Robot
	 */
	enum RobotState {
		kInitialize, kTeleopDrive
	};

	/**
	 * Positions of the Intake Arm
	 */
	enum TowerPosition {
		kIntake = 0, kLowTower = 307, kMidTower = 425
	};

	/**
	 * Positions of the Intake Arm
	 */
	enum TrayPosition {
		kSlant = 0, kStack = 800
	};

	void moveTray(TrayPosition position, double targetVelocity, bool ramp);
	void scoreTower(TowerPosition position, double targetVelocity);
	void intakeCube();
	void outtakeCube(double targetVelocity);
	okapi::Motor trayMotor;

private:
	// Current state of robot
	uint32_t m_stateVal;
	// Upcoming state of robot
  uint32_t nextState;

  okapi::Controller driverController, operatorController;


	okapi::Motor leftIntakeMotor, rightIntakeMotor;
	okapi::MotorGroup intakeMotors;

	pros::ADIButton limitSwitch;

	// Driver Buttons
	okapi::ControllerButton intakeRollersButton, outtakeRollersButton;

	// Operator Buttons
	okapi::ControllerButton scoreStackButton, lowTowerButton, midTowerButton, intakePosButton, manualUpButton, manualDownButton, toggleManualTowerButton, trayDownButton, resetTrayButton, outtakeFullSpeedButton;
	int towerToggles;
	//toggles
	bool stackTrayToggle, toggleManualTower;

};

#endif
