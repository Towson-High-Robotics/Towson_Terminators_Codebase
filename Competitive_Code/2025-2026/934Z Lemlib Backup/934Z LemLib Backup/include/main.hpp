#pragma once

#include "main.h"
#include "lemlib/api.hpp"

pros::MotorGroup left({-10, 9, -8}, pros::MotorGearset::blue);
pros::MotorGroup right({1, -2, 3}, pros::MotorGearset::blue);

lemlib::Drivetrain drivetrain(
    &left, &right, 10.372,
    lemlib::Omniwheel::NEW_325, 450, 2
);

pros::Imu imu(4);

lemlib::OdomSensors sensors(
    nullptr, nullptr, nullptr, nullptr, &imu
);

lemlib::ControllerSettings lateral(
    90, 0.125, 35, 5,
    1, 100, 5, 500,
    20
);

lemlib::ControllerSettings angular(
    60, 0.125, 20, 2,
    0.5, 100, 2, 500,
    0
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

pros::Motor intake_float(4, pros::MotorGearset::blue);
pros::Motor intake_half(5, pros::MotorGearset::green);
pros::Motor indexer(6, pros::MotorGearset::green);

pros::adi::Pneumatics matchload('A', false, false);
pros::adi::Pneumatics descore('B', false, true);