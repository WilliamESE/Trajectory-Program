#pragma once

#include "../ctrcvu/ctrclib.hpp"
#include <memory>
#include <vector>
#include <cstdint>
#include <sstream>

struct pModes{
    unsigned shoot : 1;     //1
    unsigned extend : 1;    //2
    unsigned bw : 1;        //4
    unsigned roller : 1;    //8
    unsigned turn : 1;      //16
    unsigned intake : 1;    //32
    unsigned delay : 1;     //64
    unsigned unused : 1;    //128
};

union pFlags{
    uint8_t statmode;
    pModes b_modes;
};

typedef struct{
    uint16_t time; //Max time would be 10 minutes
    int16_t xpos; //Max value is 14400 < 32768; signed in case user has 0,0 at the middle corner
    int16_t ypos;
    int32_t heading; //36000 > 32768; when using two decmial places
    int16_t rvel; //Can be negative; 2 decimal places
    int16_t lvel;
    pFlags flags;
}t_motiondata;

extern std::vector<t_motiondata> g_Auto_1;
extern std::vector<t_motiondata> g_Auton_2;
extern std::vector<t_motiondata> g_Avenger;
extern std::vector<t_motiondata> g_Avenger_2;

class Traveler {
    std::vector<t_motiondata> *m_path;
    sparkyLib::PIDController m_pidHeading;
    sparkyLib::PIDController m_pidTurn;
    sparkyLib::PIDController m_pidPurePursuit;
    std::shared_ptr<sparkyLib::Inline> m_chassis;
    std::shared_ptr<sparkyLib::Shooter> m_shooter;
    std::shared_ptr<sparkyLib::ConveyorIntake> m_intake;
    std::shared_ptr<sparkyLib::Roller> m_roller;
    std::shared_ptr<sparkyLib::Expansion> m_expand;

    bool m_headingCorrection = true;
    bool m_purePursuit = true;
    bool m_shoot1 = true; 

    int m_count = 0;
    double m_distance = 7;
    double m_range = 2;
    int m_delay = 0;
    bool m_turning = true;
    int m_tidx = 0;

    int rangeCount = 0;
    sparkyLib::Logger log;
    std::stringstream str;

    bool m_runRollers = false;

    bool m_withShooter = false;
    bool m_withIntake = false;
    bool m_withRoller = false;
    bool m_withExpansion = false;

    public:
        Traveler(std::vector<t_motiondata> *path, sparkyLib::PIDGains hpid, sparkyLib::PIDGains purepid, sparkyLib::PIDGains turnpid, double lookAHead, double range);
        Traveler &withChassis(std::shared_ptr<sparkyLib::Inline> chassis);
        Traveler &withIntake(std::shared_ptr<sparkyLib::ConveyorIntake> intake);
        Traveler &withShooter(std::shared_ptr<sparkyLib::Shooter> shooter);
        Traveler &withExpansion(std::shared_ptr<sparkyLib::Expansion> expand);
        Traveler &withRoller(std::shared_ptr<sparkyLib::Roller> roller);
        
        void setPath(std::vector<t_motiondata> *path);
        void setHeadingCorrectionPID(sparkyLib::PIDGains pid);
        void setPurePursuitPID(double distance, sparkyLib::PIDGains pid);
        void saveLog();

        bool step();
        bool turnTo(double angle);

        //Red == 2, blue == 1
        int m_allianceColour = 1;
};