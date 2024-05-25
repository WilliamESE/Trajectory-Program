#include "shooter.hpp"

using namespace sparkyLib; 

Shooter::Shooter(double minLimit, double maxLimit, std::shared_ptr<MotorGroup> mtrs, std::shared_ptr<MotorGroup> latch, std::shared_ptr<pros::ADIDigitalIn> limitswitch) : 
    m_limitMin(minLimit), m_limitMax(maxLimit), m_mtrs(mtrs), m_latches(latch), m_limitswitch(limitswitch) {
    m_mtrs->setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
}

void Shooter::stopShooter(){
    m_mtrs->moveVelocity(0);
}

void Shooter::closeLatch(){
    m_latches->moveAbsolute(45, 100);
}

void Shooter::openLatch(){
    m_latches->moveAbsolute(-15, 100);
}  

bool Shooter::initShooter(){
    closeLatch();
    return goToPosition(m_limitMax - 50);
}

bool Shooter::shoot(){
    if(m_tmr == 0){
        openLatch();
    }
    else if(m_tmr == 50){
        closeLatch();
        m_mtrs->resetEncoder();
        m_prevValue = 0;
        m_stallCount = 0;
    }
    else if(m_tmr >= 100){
        if(goToPosition(1050)){
            m_tmr = 0;
            return true;
        }
    }
    m_tmr++;
    return false;
}

double Shooter::getEncoderValue(){
    return m_mtrs->getEncoderValue();
}

bool Shooter::goToPosition(double pos){
    double enc = m_mtrs->getEncoderValue();
    double diff = fabs(enc - m_prevValue);
    std::cout << diff << std::endl;
    if(diff < 2)
        m_stallCount++;
    else
        m_stallCount = 0;
    m_mtrs->moveVelocity(100);
    if((enc >= pos) || (m_stallCount > 4) || m_limitswitch->get_value() == 1){
        if(m_stallCount > 4){
            m_mtrs->moveVelocity(-100);
            pros::delay(400);
        }   
        m_mtrs->moveVelocity(0);
        return true;
    }
    m_prevValue = enc;
    return false;
}
