#include "main.h"
#include "User/AutoSelector.h"
#include "User/Constants.h"
#include "User/Extern.h"

int SELECTED_AUTO_NUMBER;
// Initialize autoSelector and set default values
AutoSelector autoSelector("Do Nothing", DO_NOTHING_AUTO);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	// Register the rest of the autonomous routines
	autoSelector.registerAuto("Log Position RED", LOG_POS_AUTO);
	autoSelector.registerAuto("Log Velocity RED", LOG_VEL_AUTO);
	// Create the dropdown
	autoSelector.listOptions();


}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	// Keep checking if the dropdown selection is updated
	SELECTED_AUTO_NUMBER = LOG_POS_AUTO;
	/*while(true) {
		std::printf("%s\n", "Running init");
		SELECTED_AUTO_NUMBER = autoSelector.getSelectedAuto();
		std::printf("%d\n", SELECTED_AUTO_NUMBER);
		pros::delay(20);
	}*/

}
