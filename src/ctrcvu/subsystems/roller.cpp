#include "roller.hpp"
#include "pros/vision.h"
#include <iostream>

namespace sparkyLib {
    Roller::Roller(std::shared_ptr<MotorGroup> m_roller, std::shared_ptr<Vision> vision_sensor, bool red_alliance) 
        : m_roller(m_roller), vision_sensor(vision_sensor) {}

void Roller::moveRoller(int ivel) {
    velocity = ivel;
    m_roller -> moveVelocity(velocity); //rpm: red gearset is 100
}

void Roller::stopRoller() {
    m_roller -> moveVelocity(0);
}

void Roller::setupVision() {
    pros::vision_signature_s_t objects[7] {
			pros::Vision::signature_from_utility(1, 1857, 5301, 3579, -871, 743, -64, 0.700, 0),
			pros::Vision::signature_from_utility(2, -1531, -89, -810, 1107, 3795, 2451, 1.300, 0),
			pros::Vision::signature_from_utility(3, 0, 0, 0, 0, 0, 0, 3.000, 0),
			pros::Vision::signature_from_utility(4, 0, 0, 0, 0, 0, 0, 3.000, 0),
			pros::Vision::signature_from_utility(5, 0, 0, 0, 0, 0, 0, 3.000, 0),
			pros::Vision::signature_from_utility(6, 0, 0, 0, 0, 0, 0, 3.000, 0),
			pros::Vision::signature_from_utility(7, 0, 0, 0, 0, 0, 0, 3.000, 0)
  		};
    vision_sensor -> setBrightness(150);
    vision_sensor -> setSignatures(objects);
}

int Roller::readVision(){
    pros::vision_object_s_t object_arr[4];
    //assuming the robot is moving backwards
    moveRoller(velocity);
    //Read vision sensor
    return vision_sensor->readSignatureObject(*vision_sensor, object_arr);
}

void Roller::getRollerVision() { //returns signature id of object detected, ie 1 means red roller
    // get the id from the readSignatureObject function
    // this id will be used to roll to the correct color

    pros::vision_object_s_t object_arr[4];

    if (red_alliance) {
        sig = vision_sensor->readSignatureObject(*vision_sensor, object_arr); // returns detected signature id
        // moveRoller until sig == 1
        while (sig != 2) { // blue has to be at the bottom for red to be at the top
            moveRoller(velocity);
            sig = vision_sensor->readSignatureObject(*vision_sensor, object_arr);
        }
        pros::delay(750);
        stopRoller();
    } else if (!red_alliance) { // blue_alliance
        sig = vision_sensor->readSignatureObject(*vision_sensor, object_arr); // returns detected signature id
        // moveRoller until sig == 2
        while (sig != 1) {
            moveRoller(velocity);
            sig = vision_sensor->readSignatureObject(*vision_sensor, object_arr);
        }
        pros::delay(750);
        stopRoller();
    }
}
}