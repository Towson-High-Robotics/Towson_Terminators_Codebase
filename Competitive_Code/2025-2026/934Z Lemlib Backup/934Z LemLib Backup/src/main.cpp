#include "lemlib/api.hpp"
#include "main.h"
#include "main.hpp"

const inline uint32_t HORIZONTAL_RES = 480;
const inline uint32_t VERTICAL_RES = 240;

enum Autos {
	Left,
	Right,
	Solo,
	Skills,
	None
};

Autos current_auto = Autos::None;

void render_loop() {
	while(true) {
		pros::screen::erase();
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 4, std::format("LF: {:.0}°C {:.0}°", left.get_temperature(0), left.get_position(0)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 20, std::format("LM: {:.0}°C {:.0}°", left.get_temperature(1), left.get_position(1)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 36, std::format("LB: {:.0}°C {:.0}°", left.get_temperature(2), left.get_position(2)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 52, std::format("RF: {:.0}°C {:.0}°", right.get_temperature(0), right.get_position(0)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 68, std::format("RM: {:.0}°C {:.0}°", right.get_temperature(1), right.get_position(1)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 84, std::format("RB: {:.0}°C {:.0}°", right.get_temperature(2), right.get_position(2)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 76, std::format("IF: {:.0}°C {:.0}°", intake_float.get_temperature(), intake_float.get_position()).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 88, std::format("IT: {:.0}°C {:.0}°", intake_float.get_temperature(), intake_half.get_position()).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 100, std::format("IB: {:.0}°C {:.0}°", indexer.get_temperature(), indexer.get_position()).c_str());
		pros::delay(250);
	}
}

void initialize() {
	pros::screen::erase();
	chassis.calibrate();
	pros::Task([&]{ render_loop(); });
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	matchload.retract();
	descore.retract();
	switch (current_auto) {
		case Autos::Left:
			chassis.setPose(0, 0, 0, false);
			matchload.extend();
			chassis.moveToPose(6, 31, 270, 5000, {}, false);
			intake_float.move(127);
			intake_half.move(63);
			pros::delay(500);
			chassis.moveToPose(34, 31, 270, 5000, { .forwards = false }, false);
			intake_float.move(127);
			intake_half.move(127);
			indexer.move(127);
			descore.extend();
			break;
		case Autos::Right:
			chassis.setPose(0, 0, 90, false);
			chassis.moveToPose(7, 30.261, 45, 5000, {}, false);
			chassis.moveToPose(1.5, 36.5, 45, 5000, {}, false);
			intake_float.move(-127);
			pros::delay(250);
			intake_float.move(0);
			chassis.moveToPose(39, -1, 225, 5000, { .forwards = false }, false);
			matchload.extend();
			chassis.moveToPose(39, -10, 270, 5000, {}, false);
			intake_float.move(127);
			intake_half.move(127);
			pros::delay(500);
			chassis.moveToPose(39, 18, 270, 5000, { .forwards = false }, false);
			indexer.move(127);
			descore.extend();
			break;
		case Autos::Solo:
			chassis.setPose(0, 0, 90, false);
			chassis.moveToPose(7, 30.261, 45, 5000, {}, false);
			chassis.moveToPose(1.5, 36.5, 45, 5000, {}, false);
			intake_float.move(-127);
			pros::delay(250);
			intake_float.move(0);
			chassis.moveToPose(39, -1, 225, 5000, { .forwards = false }, false);
			matchload.extend();
			chassis.moveToPose(39, -10, 270, 5000, {}, false);
			intake_float.move(127);
			intake_half.move(127);
			pros::delay(500);
			chassis.moveToPose(39, 18, 270, 5000, { .forwards = false }, false);
			indexer.move(127);
			descore.extend();
			pros::delay(2000);
			descore.retract(); matchload.retract();
			intake_float.move(0); intake_half.move(0);
			indexer.move(0);
			chassis.moveToPose(28, 10, 0, 5000, {}, false);
			chassis.moveToPose(-55, 10, 270, 5000, {}, false);
			matchload.extend();
			chassis.moveToPose(-55, -10, 270, 5000, {}, false);
			intake_float.move(127);
			intake_half.move(127);
			pros::delay(500);
			chassis.moveToPose(-55, 18, 270, 5000, { .forwards = false }, false);
			indexer.move(127);
			descore.extend();
			break;
		case Autos::Skills:
			break;
		case Autos::None:
			chassis.setPose(0, 0, 0, false);
			chassis.moveToPoint(0, 8, 5000, {}, false);
			break;
	}
}

void opcontrol() {
	while(true) {
		int leftY = cont.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int rightY = cont.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

		chassis.tank(leftY, rightY);

		if(cont.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			intake_float.move(127); intake_half.move(127);
		} else if(cont.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			intake_float.move(-127); intake_half.move(-127);
		} else {
			intake_float.move(0); intake_half.move(0);
		}

		if(cont.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
			indexer.move(127);
		} else if(cont.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
			indexer.move(-127);
		} else {
			indexer.move(0);
		}

		if(cont.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
			matchload.toggle();
		}

		if(cont.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
			descore.toggle();
		}

		pros::delay(25);
	}
}
