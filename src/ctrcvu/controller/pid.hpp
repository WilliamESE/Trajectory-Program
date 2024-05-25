#pragma once

#include <cmath>

namespace sparkyLib {
struct PIDGains {
    double kp;
    double ki;
    double kd;
    double kf;
};

class PIDController {
    double integral;
    double integralComp;
    double prevError;
    PIDGains pidValues;
    double m_max;
    double m_min;
    double count;
    int m_marks;
    bool complete = false;

    public:
    PIDController(PIDGains ipidValues, double max = 1, double min = -1, double iComp = 10, int leewayCount = 10);

    double calculatePID(double current, double target, double leeway);

    void setPID(PIDGains ipidValues);
    void setLimits(double max, double min);
    void setiComp(double icomp);
    PIDGains getPID();
    void resetPID();
    bool isComplete();
};
}
