#include "lemlib/api.hpp"
#include "main.h"
#include "lemlib/chassis/chassis.hpp"
#include "main.hpp"
#include "pros/rtos.hpp"
#include "pros/screen.h"
#include "pros/screen.hpp"

const inline uint32_t HORIZONTAL_RES = 480;
const inline uint32_t VERTICAL_RES = 240;

enum Autos {
	None = 0,
	Left = 1,
	Right = 2,
	Solo = 4,
	Skills = 3,
};

std::string autos_as_string(Autos v) {
	switch (v) {
        case None: return "None - Default";
        case Left: return "Left";
        case Right: return "Right";
        case Solo: return "SoloAWP";
        case Skills: return "Skills";
		default: return "";
    }
}

Autos next_auto(Autos v) {
	switch (v) {
        case None: return Autos::Left;
        case Left: return Autos::Right;
        case Right: return Autos::Solo;
        case Solo: return Autos::Skills;
        case Skills: return Autos::None;
    }
}

Autos current_auto = Autos::Left;

void touch_event_cb() {
	current_auto = next_auto(current_auto);
}

void render_loop() {

	pros::screen::touch_callback(touch_event_cb, pros::last_touch_e_t::E_TOUCH_PRESSED);

	while(true) {
		pros::screen::erase();
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 4, std::format("LF: {:.0}°C {:.0}°", left.get_temperature(0), left.get_position(0)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 22, std::format("LM: {:.0}°C {:.0}°", left.get_temperature(1), left.get_position(1)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 40, std::format("LB: {:.0}°C {:.0}°", left.get_temperature(2), left.get_position(2)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 58, std::format("RF: {:.0}°C {:.0}°", right.get_temperature(0), right.get_position(0)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 76, std::format("RM: {:.0}°C {:.0}°", right.get_temperature(1), right.get_position(1)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 94, std::format("RB: {:.0}°C {:.0}°", right.get_temperature(2), right.get_position(2)).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 112, std::format("IF: {:.0}°C {:.0}°", intake_float.get_temperature(), intake_float.get_position()).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 130, std::format("IT: {:.0}°C {:.0}°", intake_float.get_temperature(), intake_half.get_position()).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 148, std::format("IB: {:.0}°C {:.0}°", indexer.get_temperature(), indexer.get_position()).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 166, std::format("pos: x: {} in, y: {} in", chassis.getPose().x, chassis.getPose().y).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 184, std::format("theta: {}°", chassis.getPose().theta).c_str());
		pros::screen::print(pros::text_format_e_t::E_TEXT_MEDIUM, 4, 202, std::format("auto: {}", autos_as_string(current_auto)).c_str());

		pros::delay(500);
	}
}

void initialize() {
	pros::screen::erase();
	chassis.calibrate();
	//pros::delay(3000);
	pros::Task([&]{ render_loop(); });
}

void disabled() {}

void competition_initialize() {
	//chassis.calibrate();
	pros::delay(3000);
}

void move_intake(int32_t volts) {
	intake_half.move(volts);
	intake_float.move(volts);
}



void opcontrol() {
	while(true) {
		int leftY = cont.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int rightX = cont.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

		chassis.arcade(leftY, rightX);

		if(cont.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			intake_float.move(127); intake_half.move(127);
		} else if(cont.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			intake_float.move(-127); intake_half.move(-127);
		} else {
			intake_float.move(0); intake_half.move(0);
		}
		// Continue for the rest of the motors..

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
		// And continue for the other pistons

		if(cont.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
			descore.toggle();
		}

		pros::delay(25);
	}
}
