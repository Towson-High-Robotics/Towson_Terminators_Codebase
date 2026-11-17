#include "main.h"
#include "lemlib/api.hpp"


// ======================
// MOTORS
// ======================


// Drive
pros::MotorGroup left_motor_group(
    {-4, -3, -2},
    pros::MotorGears::blue
);


pros::MotorGroup right_motor_group(
    {8, 9, 10},
    pros::MotorGears::blue
);


// Lift
pros::MotorGroup D4rb(
    {11, -20},
    pros::MotorGears::red
);


// Intake
pros::Motor intake(
    5,
    pros::MotorGears::red
);

// Intake
pros::Motor clawrot(
    12,
    pros::MotorGears::green
);


// Claw pneumatic
pros::adi::DigitalOut claw('A');



// ======================
// LEMLIB DRIVETRAIN
// ======================


lemlib::Drivetrain drivetrain(
    &left_motor_group,
    &right_motor_group,
    11,
    lemlib::Omniwheel::NEW_325,
    360,
    2
);



// ======================
// SENSORS
// ======================


pros::Imu imu(15);


//pros::Rotation horizontal_encoder(16);


//pros::adi::Encoder vertical_encoder(
//    'C',
//    'D',
//    true
//);



//lemlib::TrackingWheel horizontal_tracking_wheel(
//    &horizontal_encoder,
//    lemlib::Omniwheel::NEW_325,
//    -5.75
//);



//lemlib::TrackingWheel vertical_tracking_wheel(
//    &vertical_encoder,
//    lemlib::Omniwheel::NEW_325,
//    -2.5
//);



// ======================
// ODOMETRY
// ======================

lemlib::OdomSensors sensors(
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &imu
);



// ======================
// PID
// ======================


lemlib::ControllerSettings lateral_controller(
    10,
    0,
    3,
    0,
    0,
    0,
    0,
    0,
    20
);


lemlib::ControllerSettings angular_controller(
    2,
    0,
    10,
    0,
    0,
    0,
    0,
    0,
    0
);



// ======================
// CHASSIS
// ======================


lemlib::Chassis chassis(
    drivetrain,
    lateral_controller,
    angular_controller,
    sensors
);



// ======================
// INITIALIZE
// ======================


void initialize() {

    pros::lcd::initialize();

    left_motor_group.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
right_motor_group.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

D4rb.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
clawrot.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);


    chassis.calibrate();


    pros::Task screen_task([] {

        while(true) {

            pros::lcd::print(
                0,
                "X: %.2f",
                chassis.getPose().x
            );

            pros::lcd::print(
                1,
                "Y: %.2f",
                chassis.getPose().y
            );

            pros::lcd::print(
                2,
                "Theta: %.2f",
                chassis.getPose().theta
            );


            pros::delay(50);
        }

    });
}