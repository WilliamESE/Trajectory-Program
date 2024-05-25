#pragma once 

#include "devices/motorGroup.hpp"
#include "devices/pneumatic.hpp"
#include "api.h"
#include <memory>

namespace sparkyLib{
class Shooter {
    std::shared_ptr<MotorGroup> m_mtrs;
    
    std::shared_ptr<pros::ADIDigitalIn> m_limitswitch;

    double m_limitMin;
    double m_limitMax;

    int m_tmr = 0;
    double m_prevValue = 0;

    

    public:
    double m_stallCount = 0;
    std::shared_ptr<MotorGroup> m_latches;
    Shooter(double minLimit, double maxLimit, std::shared_ptr<MotorGroup> mtrs, std::shared_ptr<MotorGroup> latch, std::shared_ptr<pros::ADIDigitalIn> limitswitch);

    void stopShooter();
    bool initShooter();

    bool shoot();
    bool goToPosition(double pos);

    double getEncoderValue();

    void closeLatch();
    void openLatch();
};
}