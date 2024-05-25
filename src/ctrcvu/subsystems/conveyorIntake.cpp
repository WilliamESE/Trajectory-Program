#include "conveyorIntake.hpp"
#include <iostream>

using namespace sparkyLib;

/**
 * @brief Construct a new ConveyorIntake:: ConveyorIntake object
 * @param imotors Motor Group
 * @param lineTrackerSensor Line Tracker Sensor
 */
ConveyorIntake::ConveyorIntake(MotorGroup imotors, pros::ADIAnalogIn lineTrackerSensor)
                : motors(imotors), lineTrackerSensor_(lineTrackerSensor) {}

/**
* @brief move the conveyor at a given velocity
* @param ivel velocity to move the conveyor at
*
*/
void ConveyorIntake::moveVelocity(int ivel) {
    motors.moveVelocity(ivel);
    velocity = ivel;
}

/**
 * @brief start the conveyor using a task
 * 
 */
void ConveyorIntake::startConveyor() {
    conveyorTask = pros::c::task_create(conveyorMain, this, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Conveyor");
}

/**
 * @brief stop the task controlling the conveyor
 * 
 */
void ConveyorIntake::stopConveyor() {
    pros::c::task_delete(conveyorTask);
}

/**
* @brief calibrate the line tracker sensor (0.5s)
*
*/
void ConveyorIntake::initializeLineTracker() {
    lineTrackerSensor_.calibrate();
    pros::delay(500);
}

/**
 * @brief check how many disks have passed the intake with the line tracker sensor
 *
 */
void ConveyorIntake::updateConveyor() { 
    if (lineTrackerSensor_.get_value() < 200) { // dark enough for a disk to pass
        passingDisk_ = true;
        while (passingDisk_ == true) {
            // wait until disk is fully in the intake before counting it
            if (lineTrackerSensor_.get_value() > 1700) { // light enough for a disk to have passed
                countDisk++;
                passingDisk_ = false;
            }
        }
    }

    if (countDisk == 3) { // max capacity in the intake means intake should be off
        intakeOn = false;
        moveVelocity(0);
    }
}

/**
* @brief run the conveyor whether the intake should be on or off
*
*/
void ConveyorIntake::runConveyor() {
    if (intakeOn) {
        moveVelocity(velocity);
    } else {
        moveVelocity(0);
    }
}

/**
* @brief to set up disk count, if preloaded (e.g. 2 disks)
* @param initCount the amount of preloaded disks
*
*/
void ConveyorIntake::setDiskCount(int initCount) {
    countDisk = initCount;
}

/**
* @brief get the amount of disks in the conveyor
* @return the amount of disks in the conveyor
*
*/
int ConveyorIntake::getDiskCount() {
    return countDisk;
}

/**
* @brief check if there is an overload of disks (>3) 
*
*/
void ConveyorIntake::checkOverload() {
    if (countDisk >= 4) {
        moveVelocity(-1 * velocity); // reverse the conveyor and push disks out
        pros::delay(1000);
    }
}

/**
* @brief reset disk count, meant to be used when shooter is executed
*
*/
void ConveyorIntake::resetConveyor() {
    countDisk = 0;
}

/**
 * @brief used to control the conveyor with a PID and given RPM
 * @param conveyor pointer to the conveyor
 *
 */
void sparkyLib::conveyorMain(void* conveyor){
    // define the pointer for the conveyor
    ConveyorIntake* intake = (ConveyorIntake*) conveyor;
    while(true){

        // update line tracker sensor counting disks
        intake -> updateConveyor();

        // check if there are too many disks in the conveyor
        intake -> checkOverload();

        // wait
        //pros::Task::delay_until(&time, 10);
        pros::delay(20);
    }
} // namespace sparkylib