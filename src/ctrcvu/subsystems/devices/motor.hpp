#pragma once

#include "okapi/api.hpp"
#include "api.h"
#include "../../controller/pid.hpp"
#include "../../utilities/mathFunctions.hpp"

namespace sparkyLib {

void testMotor(uint8_t port);

class Motor: public okapi::Motor {
    public:
        enum mtrState {
            VELOCITY,
            POSITION,
            VOLTAGE
        };

    private:
    std::shared_ptr<pros::Rotation> m_enocder;
    PIDController m_vpid;
    PIDController m_ppid;
    
    volatile int m_state;
    bool m_rotation = false;
    bool m_linear = false;
    double m_coefficient_M;
    double m_coefficient_B;
    double m_linearRange;

    double m_targetVelocity;
    double pidVoltOutput;
    double m_targetPosition;
    int m_targetVoltage;

    double m_slewCmd;
    double m_slewRate;

    /**
     * @brief Calculates the new voltage level using a slew rate.
     * 
     * @param command 
     */
    void calSlewRate(double command);

    public:
    Motor(int8_t port);
    Motor(int8_t port, bool reverse, okapi::AbstractMotor::gearset igearset, 
        okapi::AbstractMotor::encoderUnits encoderUnits = okapi::AbstractMotor::encoderUnits::degrees);

    /**
     * @brief Sets the motor's feedback sensor to an external rotational sensor
     * 
     * @param sensor a pointer to the pros rotation sensor
     * @return Motor& 
     */
    Motor &withRotationSensor(std::shared_ptr<pros::Rotation> sensor);
    /**
     * @brief Sets the motor's feedback sensor to an external rotational sensor
     * 
     * @param sensor the port the sensor is connected to
     * @return Motor& 
     */
    Motor &withRotationSensor(uint8_t sensor, bool reversed = false);

    /** 
     * @brief Adds slewrate which is be default disabled
     * 
     * @param rate the rate in voltage the motor is limited to when changing speed.
     * @return Motor& 
     */
    Motor &withSlewRate(double rate); 

    /**
     * @brief Enable linear conversion for velocity control, use the calibrate function to find m and b
     * 
     * @param m The slope of the linear conversion
     * @param b The y-intercept for the linear conversion
     * @param range 
     * @return Motor& 
     */
    Motor &withLinearCompensation(double m, double b, double range = 10);

    /**
     * @brief Set the target Velocity of the motor -- this will not move the motor, only sets the target
     * 
     * @param velocity 
     */
    void setVelocity(double velocity);
    /**
     * @brief Set the target position of the motor -- this will not move the motor, only sets the target
     * 
     * @param velocity 
     */
    void setPosition(double position);
    /**
     * @brief Set the target voltage of the motor -- this will not move the motor, only sets the target
     * 
     * @param velocity 
     */
    void setVoltage(int volts);

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
     * @brief Set the velocity pid
     * 
     * @param pid Teh pidGains structure for kp, ki, kd, and kf
     * @param max The maximum value the pid can output
     * @param min The minimum value the pid can output
     * @param iComp How close to the target should the pid be before it activates the intrgral
     */
    void setVelPID(PIDGains pid, double max = 1, double min = -1, double iComp = 10);
    /**
     * @brief Set the position pid
     * 
     * @param pid Teh pidGains structure for kp, ki, kd, and kf
     * @param max The maximum value the pid can output
     * @param min The minimum value the pid can output
     * @param iComp How close to the target should the pid be before it activates the intrgral
     */
    void setPosPID(PIDGains pid, double max = 1, double min = -1, double iComp = 10);
    /**
     * @brief Set the Slew Rate for the motor
     * 
     * @param rate the rate at which the outgoing voltage will be changed by in mV
     */
    void setSlewRate(double rate);

    /**
     * @brief The function that should be called to move the motor
     * 
     * @return int 
     */
    int step();
    /**
     * @brief Calculates the linear coefficents for voltage to velocity conversion
     * 
     * @return int 
     */
    int calibrate();

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

	const double tpr = getGearing() == AbstractMotor::gearset::blue ? 300 : // Blue gearset tpr
						getGearing() == AbstractMotor::gearset::green ? 600 : // Green gearset tpr
																		1800; // Red gearset tpr

    //Unique identifier for later use.
    int id;
};
} // namespace sparkyLib
