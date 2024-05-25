#include "motorGroup.hpp"
#include <memory> 

using namespace std;
using namespace sparkyLib;

MotorGroup::MotorGroup(std::initializer_list<std::shared_ptr<Motor>> mtrs) : motors(mtrs) {}

void MotorGroup::addMotor(std::shared_ptr<Motor> mtr){
    motors.push_back(mtr);
}

void MotorGroup::moveVelocity(int vel) {
    for (auto motor : motors)
        motor->moveVelocity(vel);
}

void MotorGroup::moveAbsolute(double position, int velocity){
    for (auto motor : motors)
        motor->moveAbsolute(position, velocity);
}

void MotorGroup::setVelocity(double velocity){
    for(auto motor : motors)
        motor->setVelocity(velocity);
}
void MotorGroup::setPosition(double position){
    for(auto motor : motors)
        motor->setPosition(position);
}
void MotorGroup::setVoltage(double voltage){
    for(auto motor : motors)
        motor->setVoltage(voltage);
}

void MotorGroup::moveVoltage(int volt) {
    for (auto motor : motors)
        motor->moveVoltage(volt);
}

void MotorGroup::step(){
    for (auto motor : motors)
        motor->step(); 
}

std::vector<std::shared_ptr<Motor>> MotorGroup::getMotors() {
    return motors;
}

void MotorGroup::setVelPID(PIDGains velPID, double max, double min, double iComp) {
    for (auto motor : motors)
        motor->setVelPID(velPID, max, min, iComp);
}

void MotorGroup::setPositionPID(PIDGains velPID, double max, double min, double iComp) {
    for (auto motor : motors)
        motor->setPosPID(velPID, max, min, iComp);
}

double MotorGroup::getActualVelocity() {
    return motors.at(1)->getActualVelocity();
}

void MotorGroup::setBrakeMode(okapi::AbstractMotor::brakeMode brakeMode) {
    for (auto motor : motors)
        motor->setBrakeMode(brakeMode);
}

okapi::AbstractMotor::brakeMode MotorGroup::getBrakeMode() {
    return motors.at(0)->getBrakeMode();
}

bool MotorGroup::isOverTemp() {
    for (auto motor : motors) {
        if (motor->isOverTemp())
            return true;
    }
    return false;
}

void MotorGroup::setGearing(okapi::AbstractMotor::gearset gearset) {
    for (auto motor : motors)
        motor->setGearing(gearset);
}

okapi::AbstractMotor::gearset MotorGroup::getGearing() {
    return motors.at(0)->getGearing();
}

void MotorGroup::setSlewRate(double rate){
    for(auto motor : motors)
        motor->setSlewRate(rate);
}
double MotorGroup::getTarget_Velocity(){
    return motors.at(0)->getTarget_Velocity();    
}
double MotorGroup::getTarget_Position(){
    return motors.at(0)->getTarget_Position();
}
int MotorGroup::getTarget_Voltage(){
    return motors.at(0)->getTarget_Voltage();
}
double MotorGroup::getEncoderValue(){
    int sum = 0;
    for(auto motor : motors)
        sum += motor->getEncoderValue();
    double avg = ((double)sum) / ((double)motors.size());
    return avg;
}
void MotorGroup::resetEncoder(){
    for(auto motor : motors)
        motor->resetEncoder(); 
}

void sparkyLib::motorgroupTest(int8_t port1, int8_t port2, int8_t port3){
    //Create the motors
    Motor mtr1 = Motor(port1).withSlewRate(100);
    Motor mtr2 = Motor(port2).withSlewRate(100);
    Motor mtr3 = Motor(port3).withSlewRate(100);

    MotorGroup grp1({std::make_shared<Motor>(mtr1),std::make_shared<Motor>(mtr2),std::make_shared<Motor>(mtr3)});

    //Make each move
    grp1.setVoltage(3000);

    for(int cnt = 0; cnt < 300; cnt++){
        grp1.step();
        pros::delay(20);
    }
}