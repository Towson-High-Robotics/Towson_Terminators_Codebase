#pragma once

#include "main.h"
#include "lemlib/api.hpp"

pros::MotorGroup left({-1, 2, -3}, pros::MotorGearset::blue);
pros::MotorGroup right({10, -9, 8}, pros::MotorGearset::blue);

lemlib::Drivetrain drivetrain(
    &left, &right, 10.372,
    lemlib::Omniwheel::NEW_325, 450, 2
);

pros::Imu imu(11);

lemlib::OdomSensors sensors(
    nullptr, nullptr, nullptr, nullptr, &imu
);

//old kP 6 kI 0 kD 10 windup 5 small 1 smalltimeout 100, large 2, largetimeout 500 slew 127
lemlib::ControllerSettings lateral(
    6, 0, 10, 5,
    2, 100, 4, 500,
    63
);

//old kP 4 kI 0 kD 15 windup 0 small 0 smalltimeout 0 large 0 largetimeout 0 slew 0
lemlib::ControllerSettings angular(
    4, 0, 15, 5,
    1, 100, 3, 500,
    63
);

lemlib::ExpoDriveCurve throttle(
    3, 10, 1.019
);

lemlib::ExpoDriveCurve steer(
    3, 10, 1.019
);

lemlib::Chassis chassis(
    drivetrain,
    lateral, angular,
    sensors,
    &throttle, &steer
);

pros::Controller cont(pros::E_CONTROLLER_MASTER);

// Intake & Storage
pros::Motor intake_float(-4, pros::MotorGearset::blue);
pros::Motor intake_half(5, pros::MotorGearset::green);
pros::Motor indexer(6, pros::MotorGearset::green);
// Pistons
pros::adi::Pneumatics matchload('A', false, false);
pros::adi::Pneumatics descore('B', false, false);