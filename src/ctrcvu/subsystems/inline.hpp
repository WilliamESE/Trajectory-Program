#pragma once

#include "api.h"
#include "okapi/api.hpp"
#include "devices/motor.hpp"
#include "devices/motorGroup.hpp"
#include "../utilities/logger.hpp"
#include "../controller/odometry.hpp"
#include "okapi/api/device/motor/abstractMotor.hpp"
#include <memory>
#include <sstream> 

namespace sparkyLib { 
class Inline {
    public:

    typedef struct chassisSizes{
        double wheelbase;
        double wheeldiameter;
        okapi::AbstractMotor::gearset gearset;
        double gearratio;
    }csizes_t;

    private:
    std::shared_ptr<MotorGroup> m_left;
    std::shared_ptr<MotorGroup> m_right;

    std::shared_ptr<pros::Rotation> m_leftRotation;
    std::shared_ptr<pros::Rotation> m_rightRotation;

    bool m_enocder = false;
    bool m_enableOdom = false;

    bool m_cubicDrive = true;
    double m_forwardScale = 1;
    double m_turnScale = 1;
    double m_deadzone = 0.05;

    public:
    csizes_t m_sizes;
    std::shared_ptr<Odometry> m_odometry;
    
    Inline(std::shared_ptr<MotorGroup> left, std::shared_ptr<MotorGroup> right, csizes_t sizes, bool cubic = true);
    Inline &withOdometry(Pose start,std::shared_ptr<pros::Rotation> odomLeft, std::shared_ptr<pros::Rotation> odomRight, EncoderScales sizes);
    Inline &withOdometry(Pose start,std::shared_ptr<Odometry> odom);
    Inline &withRotationSensors(std::shared_ptr<pros::Rotation> left, std::shared_ptr<pros::Rotation> right);
    Inline &withDriveScales(double forward, double turn);
    Inline &withDeadzone(double deadzone);
    Inline &withSlewRate(double rate);

    std::shared_ptr<Odometry> getOdometry();
    
    Pose getRobotPosition();
    double getRobotVelocity();
    double getLeftVelocity();
    double getRightVelocity();

    void startOdometry();
    void stopOdometry();

    void setLeftVelocity(double velocity);
    void setRightVelocity(double velocity);
    void moveChassis(double left, double right);
    void stopChassis();

    void arcade(double aAxis, double bAxis);
};
} //nampespace sparkyLib