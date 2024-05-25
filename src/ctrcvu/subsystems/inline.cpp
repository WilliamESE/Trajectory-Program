#include "inline.hpp"

using namespace sparkyLib;

Inline::Inline(std::shared_ptr<MotorGroup> left, std::shared_ptr<MotorGroup> right, csizes_t sizes, bool cubic) : 
    m_left(left), m_right(right), m_sizes(sizes){
    m_cubicDrive = cubic; 
    }

Inline &Inline::withOdometry(Pose start,std::shared_ptr<pros::Rotation> odomLeft, std::shared_ptr<pros::Rotation> odomRight, EncoderScales sizes){
    Encoders enc = {odomLeft, odomRight};
    m_odometry = std::make_shared<Odometry> (enc, sizes);
    m_odometry->setPose(start);
    m_enableOdom = true;
    return *this;
}

Inline &Inline::withOdometry(Pose start,std::shared_ptr<Odometry> odom){
    m_odometry = odom;
    m_odometry->setPose(start);
    m_enableOdom = true;
    return *this;
}

Inline &Inline::withRotationSensors(std::shared_ptr<pros::Rotation> left, std::shared_ptr<pros::Rotation> right){
    m_enocder = true;
    m_leftRotation = left;
    m_rightRotation = right;
    return *this;
}

Inline &Inline::withDriveScales(double forward, double turn) {
    m_forwardScale = forward;
    m_turnScale = turn;
    return *this;
}

Inline &Inline::withDeadzone(double deadzone){
    m_deadzone = deadzone;
    return *this;
}

Inline &Inline::withSlewRate(double rate){
    m_left->setSlewRate(rate);
    m_right->setSlewRate(rate);
    return *this;
}

std::shared_ptr<Odometry> Inline::getOdometry() {
    if(!m_enableOdom)
        return nullptr;
    return m_odometry;
}
  
Pose Inline::getRobotPosition() {
    if(!m_enableOdom)
        return {0,0,0};
    return m_odometry->getPose();
}

double Inline::getRobotVelocity() {
    return (getLeftVelocity() + getRightVelocity()) / 2;
}

double Inline::getLeftVelocity() {
    double value = m_left->getActualVelocity();
    // if(m_enocder)
    //     value = m_leftRotation->get_velocity();
    //The value must be converted to in/s
    return value;
}

double Inline::getRightVelocity() {
    double value = m_right->getActualVelocity();
    // if(m_enocder)
    //     value = m_leftRotation->get_velocity();
    //The value must be converted to in/s
    return value;
}

void Inline::startOdometry(){
    if(!m_enableOdom)
        return;
    m_odometry->startOdom();
}

void Inline::stopOdometry(){
    if(!m_enableOdom)
        return;
    m_odometry->saveLog();
}

void Inline::setLeftVelocity(double velocity){
    m_left->setVelocity(velocity);
}

void Inline::setRightVelocity(double velocity){
    m_right->setVelocity(velocity);
}

void Inline::moveChassis(double left, double right){
    setLeftVelocity(left);
    setRightVelocity(right);
    // m_left->moveVelocity(left);
    // m_right->moveVelocity(right);
}

void Inline::stopChassis(){
    m_left->moveVoltage(0);
    m_right->moveVoltage(0);
}

void Inline::arcade(double aAxis, double bAxis){
    double leftSpeed = 0, rightSpeed = 0;
    if(m_cubicDrive){
        if((fabs(aAxis) > m_deadzone)||(fabs(bAxis) > m_deadzone)){
            aAxis = m_forwardScale * (aAxis * aAxis * aAxis);
            bAxis = m_turnScale * (bAxis * bAxis * bAxis);
            leftSpeed = aAxis - bAxis;
            rightSpeed = aAxis + bAxis;
        }
    }
    else{
        if((fabs(aAxis) > m_deadzone)||(fabs(bAxis) > m_deadzone)){
            leftSpeed = (aAxis * m_forwardScale) - (bAxis * m_turnScale);
            rightSpeed = (aAxis * m_forwardScale) + (bAxis * m_turnScale);
        }
    }

    if(leftSpeed > 1)
        leftSpeed = 1;
    if(rightSpeed > 1)
        rightSpeed = 1;
    if(leftSpeed < -1)
        leftSpeed = -1;
    if(rightSpeed < -1)
        rightSpeed = -1;

    if(m_sizes.gearset == okapi::AbstractMotor::gearset::red){
        leftSpeed *= 100;
        rightSpeed *= 100;
    }
    else if(m_sizes.gearset == okapi::AbstractMotor::gearset::green){
        leftSpeed *= 200;
        rightSpeed *= 200;
    }
    else{
        leftSpeed *= 600;
        rightSpeed *= 600;
    }
    moveChassis(leftSpeed,rightSpeed);
}
