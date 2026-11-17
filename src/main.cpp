#include "main.h"
#include "lemlib/api.hpp"
extern lemlib::Chassis chassis;

extern pros::MotorGroup D4rb;
extern pros::Motor intake;
extern pros::Motor clawrot;
extern pros::adi::DigitalOut claw;

void on_center_button() {
    static bool pressed = false;
    pressed = !pressed;

    if (pressed) {
        pros::lcd::set_text(2, "I was pressed!");
    } 
    else {
        pros::lcd::clear_line(2);
    }
}


void disabled() {}

void competition_initialize() {}


void autonomous() {
    // Autonomous code goes here
    // Initialize pose and move to point
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 24, 999999999);

}

void opcontrol() {

    pros::Controller master(pros::E_CONTROLLER_MASTER);

    bool clawState = true;
    bool clawLastState = false;


    while (true) {

        // ======================
        // DRIVING
        // ======================

        int forward = master.get_analog(
            pros::E_CONTROLLER_ANALOG_LEFT_Y
        );

        int turn = master.get_analog(
            pros::E_CONTROLLER_ANALOG_RIGHT_X
        );

        chassis.arcade(forward, turn);



        // ======================
        // INTAKE
        // ======================

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {

            intake.move(127);

        } 
        else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {

            intake.move(-127);

        } 
        else {

            intake.move(0);

        }



        // ======================
        // LIFT
        // ======================

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {

            D4rb.move(127);

        } 
        else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {

            D4rb.move(-127);

        } 
        else {

            D4rb.move(0);

        }

		//clawrot

		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {

            clawrot.move(90);

        } 
        else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {

            clawrot.move(-90);

        } 
        else {

            clawrot.move(0);

        }



        // ======================
        // CLAW TOGGLE
        // ======================

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)
            && !clawLastState) {

            clawState = !clawState;
            clawLastState = true;

        }


        if (!master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {

            clawLastState = false;

        }


        claw.set_value(clawState);



        pros::delay(20);
    }
	
}
