#include "pid.hpp"
#include <iostream>

using namespace std;
using namespace sparkyLib;

PIDController::PIDController(PIDGains ipidValues, double max, double min, double iComp, int leewayCount) :
                            pidValues(ipidValues), m_max(max), m_min(min), integralComp(iComp), m_marks(leewayCount) {}

double PIDController::calculatePID(double current, double target, double leeway) {
    double error = target - current;

    if (fabs(error) <= leeway) {
        count++;
        if (count > m_marks)
            complete = true;
    } else {
        count = 0;
    }

    double derivative = error - prevError;
    double newIntegral = fabs(error) < integralComp ? integral + error : 0;
    prevError = error;

    double signal = error * pidValues.kp + integral * pidValues.ki + derivative * pidValues.kd + pidValues.kf;
    if (signal > m_max)
        signal = m_max;
    else if (signal < m_min)
        signal = m_min;
    else
        integral = newIntegral;

    return signal;
}

bool PIDController::isComplete(){
    return complete;
}

void PIDController::setLimits(double max, double min){
    m_max = max;
    m_min = min;
}

void PIDController::setiComp(double icomp){
    integralComp = icomp;
}

void PIDController::setPID(PIDGains ipidValues) {
    pidValues = ipidValues;
}

PIDGains PIDController::getPID() {
    return pidValues;
}

void PIDController::resetPID() {
    integral = 0;
    prevError = 0;
    count = 0;
    complete = false;
}
