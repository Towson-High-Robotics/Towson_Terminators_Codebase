#include "lemlib/api.hpp"
#include "main.h"
#include "lemlib/chassis/chassis.hpp"
#include "main.hpp"
#include "pros/rtos.hpp"

const inline uint32_t HORIZONTAL_RES = 480;
const inline uint32_t VERTICAL_RES = 240;

enum Autos {
	Left,
	Right,
	Solo,
	Skills,
	None
};

Autos current_auto = Autos::Left;

void render_loop() {
	while(true) {
		pros::screen::erase();
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 4, std::format("LF: {:.0}°C {:.0}°", left.get_temperature(0), left.get_position(0)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 22, std::format("LM: {:.0}°C {:.0}°", left.get_temperature(1), left.get_position(1)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 40, std::format("LB: {:.0}°C {:.0}°", left.get_temperature(2), left.get_position(2)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 58, std::format("RF: {:.0}°C {:.0}°", right.get_temperature(0), right.get_position(0)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 76, std::format("RM: {:.0}°C {:.0}°", right.get_temperature(1), right.get_position(1)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 94, std::format("RB: {:.0}°C {:.0}°", right.get_temperature(2), right.get_position(2)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 88, std::format("IF: {:.0}°C {:.0}°", intake_float.get_temperature(), intake_float.get_position()).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 102, std::format("IT: {:.0}°C {:.0}°", intake_float.get_temperature(), intake_half.get_position()).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 116, std::format("IB: {:.0}°C {:.0}°", indexer.get_temperature(), indexer.get_position()).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 128, std::format("x: {}°", chassis.getPose().x).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 142, std::format("y: {}°", chassis.getPose().y).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 156, std::format("theta: {}°", chassis.getPose().theta).c_str());
		pros::delay(500);
	}
}

void initialize() {
	pros::screen::erase();
	chassis.calibrate();
	pros::delay(3000);
	pros::Task([&]{ render_loop(); });
}

void disabled() {}

void competition_initialize() {
	chassis.calibrate();
	pros::delay(3000);
}

void move_intake(int32_t volts) {
	intake_half.move(volts);
	intake_float.move(volts);
}

void autonomous() {
	matchload.retract();
	descore.retract();
	switch (current_auto) {
		case Autos::Left:
			chassis.setPose(0, 0, 0, false);
			chassis.moveToPose(-10, 30, 250, 3500);
			chassis.waitUntilDone();
			matchload.extend();
			move_intake(127);
			chassis.moveToPose(-36, 10, 290, 5000);
			chassis.waitUntilDone();
			pros::delay(100);
			chassis.moveToPose(24, 10, 290, 5000);
			chassis.waitUntilDone();
			descore.extend();
			break;
		case Autos::Right:
			break;
		case Autos::Solo:
			break;
		case Autos::Skills:
			break;
		case Autos::None:
			chassis.moveToPoint(0, 24, 5000);
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
