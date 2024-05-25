#include "motorController.hpp"
#include <memory>
#include <vector>
#include <sstream>

using namespace sparkyLib;

std::vector<std::shared_ptr<Motor>> sparkyLib::g_motors;
pros::task_t sparkyLib::mtrControllerTask;
bool sparkyLib::mtrTaskRun = false;
Logger sparkyLib::controllerLog = Logger("mtrLog");

void sparkyLib::addMotor(std::shared_ptr<Motor> mtr){
    int items = g_motors.size();
    mtr->id = items;
    g_motors.push_back(mtr); 
}

void sparkyLib::addMotorGroup(std::shared_ptr<MotorGroup> grp){
    std::vector<std::shared_ptr<Motor>> mtrs = grp->getMotors();
    for (auto motor : mtrs){
        addMotor(motor);
    }
}

void sparkyLib::removeMotor(int index){
    g_motors.erase(g_motors.begin() + index);
}

void sparkyLib::startMotorController(){
    mtrTaskRun = true;
    mtrControllerTask = pros::c::task_create(motorControl, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Motor Control");
}
void sparkyLib::stopMotorController(){
    mtrTaskRun = false;
    controllerLog.close();
}

void sparkyLib::motorControl(void* param){
    std::stringstream str;
    int id = 0;
    while(mtrTaskRun){
        id = 0;
        for(auto mtr : g_motors){
            mtr->step();
            str << id << "," << mtr->getActualVelocity() << "," << mtr->getVoltage()<< "," << mtr->getCurrentDraw()<< "," << mtr->getCurrentLimit() << "," << mtr->isOverCurrent() << ",";
            id++;
        }
        controllerLog.writeline(str.str());
        pros::delay(10);
        str.str("");
    }
}

void sparkyLib::motorControllerTest(){
     //Create the motors
    Motor mtr1 = Motor(1).withSlewRate(100);
    Motor mtr2 = Motor(2).withSlewRate(100);
    Motor mtr3 = Motor(3).withSlewRate(100);

    MotorGroup grp1({std::make_shared<Motor>(mtr1),std::make_shared<Motor>(mtr2),std::make_shared<Motor>(mtr3)});

    addMotorGroup(std::make_shared<MotorGroup>(grp1));

    //Start motor controller task
    startMotorController();

    //Give new tasks for the motors
    pros::delay(1000);
    grp1.setVoltage(3000);
    pros::delay(1000);
    grp1.setVoltage(6000);
    pros::delay(1000);
    grp1.setVoltage(0);
}