#pragma once

#include "api.h"
#include "motor.hpp"
#include "../../controller/pid.hpp"
#include <vector>
#include <initializer_list>
#include <memory>
 
namespace sparkyLib {

void motorgroupTest(int8_t port1, int8_t port2, int8_t port3);

class MotorGroup {

    std::vector<std::shared_ptr<Motor>> motors;

    public:
    /**
     * @brief A group of motors that have similar properties, such as always being powered at the same time, same gearbox, etc.
     * 
     * @param mtr A vector of the motors that are in the group
     */
    MotorGroup(std::initializer_list<std::shared_ptr<Motor>> mtrs);

    void addMotor(std::shared_ptr<Motor> mtr);
    /**
     * @brief Sets all the motors to a specific velocity with a built-in PID
     * 
     * @param vel The desired velocity in rpm
     */
    void moveVelocity(int vel);

    void moveAbsolute(double position, int velocity);

    /**
     * @brief Set the Velocity of each motor -- does not make them move, step must be called peroidically
     * 
     * @param velocity 
     */
    void setVelocity(double velocity);
    /**
     * @brief Set the position of each motor -- does not make them move, step must be called peroidically
     * 
     * @param position 
     */
    void setPosition(double position);
    /**
     * @brief Set the voltage of each motor -- does not make them move, step must be called peroidically
     * 
     * @param voltage 
     */
    void setVoltage(double voltage);
    /**
     * @brief Runs the control function for each motor.
     * 
     */
    void step();

    /**
     * @brief Sets all the motors to a specific voltage
     * 
     * @param volt The desired voltage in mV
     */
    void moveVoltage(int volt);

    /**
     * @brief Gets the actual velocity of the motors
     * 
     * @return The velocity of the motors in rpm
     */
    double getActualVelocity();

    /**
     * @brief Gets the motors
     * 
     * @return A vector containing the motors in the motor group
     */
    std::vector<std::shared_ptr<Motor>> getMotors();

    /**
     * @brief Set the Velocity P I D object
     * 
     * @param velPID The kp, ki, kd, and kf; default is 0
     * @param max The maximum the pid can output, default is 1
     * @param min The minimum the pid can output, default is -1
     * @param iComp When the integral actives
     */
    void setVelPID(PIDGains velPID, double max = 1, double min = -1, double iComp = 10);
    /**
     * @brief Set the Velocity P I D object
     * 
     * @param velPID The kp, ki, kd, and kf; default is 0
     * @param max The maximum the pid can output, default is 1
     * @param min The minimum the pid can output, default is -1
     * @param iComp When the integral actives
     */
    void setPositionPID(PIDGains velPID, double max = 1, double min = -1, double iComp = 10);

    /**
     * @brief Sets the brake mode of the motors
     * 
     * @param brakeMode The new brake mode value from okapi that is to be set to the motors
     */
    void setBrakeMode(okapi::AbstractMotor::brakeMode brakeMode);

    /**
     * @brief Gets the brake mode of the motors
     * 
     * @return The motors' brakemode 
     */
    okapi::AbstractMotor::brakeMode getBrakeMode();

    bool isOverTemp();

    /**
     * @brief Sets the gearing
     * 
     * @param gearset The new gearset from okapi that is to be set to the motors
     */
    void setGearing(okapi::AbstractMotor::gearset gearset);

    /**
     * @brief Gets the gearset
     * 
     * @return the motors' internal gearset 
     */
    okapi::AbstractMotor::gearset getGearing();

    /**
     * @brief Set the Slew Rate for each motor
     * 
     * @param rate the rate in mV
     */
    void setSlewRate(double rate);

    /**
     * @brief Get the Target Velocity of the motor
     * 
     * @return double 
     */
    double getTarget_Velocity();
    /**
     * @brief Get the Target position of the motor
     * 
     * @return double 
     */
    double getTarget_Position();
    /**
     * @brief Get the Target voltage of the motor
     * 
     * @return double 
     */
    int getTarget_Voltage();

    /**
     * @brief Get the Encoder Value
     * 
     * @return double 
     */
    double getEncoderValue();
    /**
     * @brief Resets the encoder position to 0
     * 
     */
    void resetEncoder();
};
} // namespace sparkyLib
