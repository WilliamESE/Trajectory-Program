#pragma once

#include "../subsystems/devices/motor.hpp"
#include "../subsystems/devices/motorgroup.hpp"
#include "../utilities/logger.hpp"
#include <vector>
#include <memory>   

namespace sparkyLib { 

//Global vecotr of pointers to motors used by the motor controller to communicate with the motor objects
extern std::vector<std::shared_ptr<Motor>> g_motors;
extern pros::task_t mtrControllerTask;
extern bool mtrTaskRun;
extern Logger controllerLog;

void motorControllerTest();

/**
* @brief Adds a motor to the control system
* 
* @param mtr Motor object to be added
*/
void addMotor(std::shared_ptr<Motor> mtr);

/**
* @brief Adds all the motors from a group into the controller
* 
* @param grp group to be added
*/
void addMotorGroup(std::shared_ptr<MotorGroup> grp);

/**
* @brief Removes a motor from control system
* 
* @param index Index of the motor to be removed
*/
void removeMotor(int index);

/**
* @brief Motor control task function, do not call this.
* 
* @param param 
*/
void motorControl(void* param);

/**
 * @brief Starts the motor controller task
 * 
 */
void startMotorController();

/**
 * @brief Stops the motor controller task
 * 
 */
void stopMotorController();

}