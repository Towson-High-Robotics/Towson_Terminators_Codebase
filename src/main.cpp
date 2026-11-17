
#include "main.h"
#include "lemlib/api.hpp"

extern lemlib::Chassis chassis;

extern pros::MotorGroup D4rb;
extern pros::Motor intake;
extern pros::Motor clawrot;
extern pros::adi::DigitalOut claw;

// ======================
// AUTONOMOUS SETTINGS
// ======================

// Change these distances to match your robot's path.
// Blue is mirrored across the X axis.







// ======================
// DRIVER CONTROL
// ======================

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


// ======================
// AUTONOMOUS
// ======================


// ==========================================
// RED AUTON 1
// ==========================================


//this one drive forwards to the pin (0,12)
//collect pin (lower, lift and do all that stuff)
//turn right and place pin at scoring area (12,24)

//coords in inches 
// ==========================================
// RED AUTON 1
// ==========================================


void rotate(int time) {
    // Get current time
int startTime = pros::millis();
int timeout = 4000;
double targetHeading = 90.0;
double kP = 2.0; // Tune this proportional constant based on your robot's tuning

while (pros::millis() - startTime < timeout) {
    // Get the current heading of the chassis (in degrees)
    lemlib::Pose pose = chassis.getPose();
    double currentHeading = pose.theta;

    // Calculate error (shortest path around 360 degrees if needed, or simple difference)
    double error = targetHeading - currentHeading;
    
    // Convert error to a turn power/voltage
    double turnPower = error * kP;

    // Run arcade with 0 forward power and calculated turn power
    chassis.arcade(0, turnPower);

    // Break out early if error is very small (within an acceptable tolerance like 1 degree)
    if (std::abs(error) < 1.0) {
        break;
    }

    pros::delay(10);
}

// Stop the arcade drive after turning
chassis.arcade(0, 0);

}
void redAuton1() {
    /*claw.set_value(true);
    /hassis.setPose(0, 0, 0);
    /D4rb.move(-127);
    pros::delay(800);
    chassis.moveToPoint(0, -20, 2000, {.forwards = false});
    pros::delay(800);
    chassis.moveToPoint(0, 20, 2000);
    /*D4rb.move(0);
    chassis.moveToPoint(0, 20, 2000);
    pros::delay
    D4rb.move(127);
    pros::delay(800);
    chassis.moveToPoint(0, -20, 2000, {.forwards = false});
    D4rb.move(-127);*/

    /*for(int i = 0; i<2;i++) {
        chassis.arcade(127, 0); 
        pros::delay(200);
        chassis.arcade(0, 0);
        pros::delay(200);
        chassis.arcade(-127, 0);
        pros::delay(400);
        chassis.arcade(0, 0);
        pros::delay(200);
    }*/

    claw.set_value(true);
    chassis.arcade(127, 0); 
    pros::delay(250);
    chassis.arcade(0, 0);
    pros::delay(200);
    chassis.arcade(-127, 0);
    pros::delay(350);
    chassis.arcade(0, 0);
    pros::delay(200);

    //2

    chassis.arcade(127, 0); 
    pros::delay(250);
    chassis.arcade(0, 0);
    pros::delay(200);
    chassis.arcade(-127, 0);
    pros::delay(400);
    chassis.arcade(0, 0);
    pros::delay(200);
    //other

    chassis.arcade(127, 0); 
    pros::delay(200);
    chassis.arcade(0, 0); 


    
    //moving forward
    /*
    /chassis.arcade(0,0);
    //chassis.turnToHeading(90, 4000);
    rotate(0);
    chassis.arcade(0,0);
    D4rb.move(-127);
    pros::delay(450);
    D4rb.move(0);
    chassis.arcade(127,0);
    pros::delay(300);
    //move backwards
    chassis.arcade(0, 0); 
    D4rb.move(-127);
    pros::delay(300);
    claw.set_value(false);

    D4rb.move(0);*/
    
   // pros::delay(650);
    //chassis.arcade(0, 0); 

    



   
    // Move forward 4 inches
    

    // Raise lift
  
}


// ==========================================
// RED AUTON 2
// ==========================================

void redAuton2() {

    chassis.setPose(0, 0, 0);
    D4rb.move(127);
    pros::delay(800);
    D4rb.move(0);
    // Move forward 4 inches
    chassis.moveToPoint(0, 4, 2000);

    // Raise lift
 
}


// ==========================================
// BLUE AUTON 1
// ==========================================

void blueAuton1() {

    chassis.setPose(0, 0, 180);
    D4rb.move(127);
    pros::delay(800);
    D4rb.move(0);
    // Move forward 4 inches
   // chassis.moveToPoint(0, -4, 2000);

    // Raise lift
 
}


// ==========================================
// BLUE AUTON 2
// ==========================================

void blueAuton2() {

    chassis.setPose(0, 0, 180);
    D4rb.move(127);
    pros::delay(800);
    D4rb.move(0);
    // Move forward 4 inches
    chassis.moveToPoint(0, -4, 2000);

    // Raise lift
   
}


// ==========================================
// AUTONOMOUS SELECTION
// ==========================================

void autonomous() {

    redAuton1();

    // redAuton2();
    // blueAuton1();
    // blueAuton2();
}



// ==========================================
// AUTONOMOUS SELECTION
// ==========================================






// ======================
// DRIVER CONTROL
// ======================

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


        // ======================
        // CLAW ROTATION
        // ======================

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
