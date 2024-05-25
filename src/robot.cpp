#include "robot.hpp"
#include "okapi/impl/device/controllerUtil.hpp"
#include "pros/adi.hpp"
#include "pros/motors.h"
#include "pros/rotation.hpp"
#include <memory>
#include <sstream>

using namespace sparkyLib;

std::shared_ptr<sparkyLib::Inline> Robot::m_chassis;
std::shared_ptr<okapi::Controller> Robot::m_driver;
std::shared_ptr<sparkyLib::Configuration> Robot::m_settings;
std::shared_ptr<sparkyLib::Odometry> Robot::m_Odometry;
std::shared_ptr<sparkyLib::Roller> Robot::m_roller;
std::shared_ptr<sparkyLib::Expansion> Robot::expand;
std::shared_ptr<Traveler> Robot::m_traveler;
std::shared_ptr<sparkyLib::Shooter> Robot::m_shooter;
std::shared_ptr<sparkyLib::ConveyorIntake> Robot::m_intake;
std::shared_ptr<sparkyLib::Display> Robot::m_display;

int btnPress = 0;
bool last = false;

enum class IntakeState {
    OFF,
    INTAKING,
    OUTTAKING
};

IntakeState intakeState = IntakeState::OFF;

Robot::Robot(){}

void Robot::init(){
    #if ROBOT != SPARKY
    std::shared_ptr<Motor> left_A = std::make_shared<Motor>(MTR_LEFT_PORT_A,MTR_LEFT_REVERSED_A,MTR_GEARSET);
    std::shared_ptr<Motor> left_B = std::make_shared<Motor>(MTR_LEFT_PORT_B,MTR_LEFT_REVERSED_B,MTR_GEARSET);
    std::shared_ptr<Motor> left_C = std::make_shared<Motor>(MTR_LEFT_PORT_C,MTR_LEFT_REVERSED_C,MTR_GEARSET);
    std::shared_ptr<Motor> left_D = std::make_shared<Motor>(MTR_LEFT_PORT_D,MTR_LEFT_REVERSED_D,MTR_GEARSET);

    std::shared_ptr<Motor> right_A = std::make_shared<Motor>(MTR_RIGHT_PORT_A,MTR_RIGHT_REVERSED_A,MTR_GEARSET);
    std::shared_ptr<Motor> right_B = std::make_shared<Motor>(MTR_RIGHT_PORT_B,MTR_RIGHT_REVERSED_B,MTR_GEARSET);
    std::shared_ptr<Motor> right_C = std::make_shared<Motor>(MTR_RIGHT_PORT_C,MTR_RIGHT_REVERSED_C,MTR_GEARSET);
    std::shared_ptr<Motor> right_D = std::make_shared<Motor>(MTR_RIGHT_PORT_D,MTR_RIGHT_REVERSED_D,MTR_GEARSET);
   
    std::shared_ptr<MotorGroup> leftSide = std::make_shared<MotorGroup>(MotorGroup({left_A,left_B,left_C,left_D}));
    std::shared_ptr<MotorGroup> rightSide = std::make_shared<MotorGroup>(MotorGroup({right_A,right_B,right_C,right_D}));
    
    //PID values
    leftSide->setVelPID({MTR_LEFT_KP, MTR_LEFT_KI,MTR_LEFT_KD,MTR_LEFT_KF});
    rightSide->setVelPID({MTR_RIGHT_KP, MTR_RIGHT_KI,MTR_RIGHT_KD,MTR_RIGHT_KF});

    #else

    std::shared_ptr<Motor> left_A = std::make_shared<Motor>(MTR_LEFT_PORT_A,MTR_LEFT_REVERSED_A,MTR_GEARSET);
    std::shared_ptr<Motor> left_B = std::make_shared<Motor>(MTR_LEFT_PORT_B,MTR_LEFT_REVERSED_A,MTR_GEARSET);

    std::shared_ptr<Motor> right_A = std::make_shared<Motor>(MTR_RIGHT_PORT_A,MTR_RIGHT_REVERSED_A,MTR_GEARSET);
    std::shared_ptr<Motor> right_B = std::make_shared<Motor>(MTR_RIGHT_PORT_B,MTR_RIGHT_REVERSED_B,MTR_GEARSET);
    std::shared_ptr<MotorGroup> leftSide = std::make_shared<MotorGroup>(MotorGroup({left_A,left_B}));
    std::shared_ptr<MotorGroup> rightSide = std::make_shared<MotorGroup>(MotorGroup({right_A,right_B}));
    //PID values
    leftSide->setVelPID({MTR_LEFT_KP, MTR_LEFT_KI,MTR_LEFT_KD,MTR_LEFT_KF});
    rightSide->setVelPID({MTR_RIGHT_KP, MTR_RIGHT_KI,MTR_RIGHT_KD,MTR_RIGHT_KF});

    #endif

    addMotorGroup(leftSide);
    addMotorGroup(rightSide);
    
    m_chassis.reset(new Inline(leftSide,rightSide,{ODOM_SCALE_LEFTRADIUS+ODOM_SCALE_RIGHTRADIUS,ODOM_SCALE_LEFTDIAMETER,MTR_GEARSET,MTR_GEAR_RATIO}));
    //Rotational sensors
    //m_chassis->withRotationSensors(std::make_shared<pros::Rotation>(DRIVE_ROT_LEFT,true), std::make_shared<pros::Rotation> (DRIVE_ROT_RIGHT));
    //Slew Rate
    m_chassis->withSlewRate(MTR_SLEW);
    m_chassis->withDriveScales(MTR_FORWARD_SCALE, MTR_TURN_SCALE);
    
    //Odometry
    //m_chassis->withOdometry({36.0,10},std::make_shared<pros::Rotation>(ODOM_ROT_PORT_LEFT,ODOM_ROT_REVERSED_LEFT), std::make_shared<pros::Rotation> (ODOM_ROT_PORT_RIGHT,ODOM_ROT_REVERSED_RIGHT),{ODOM_SCALE_LEFTRADIUS, ODOM_SCALE_RIGHTRADIUS, ODOM_SCALE_MIDDLERADIUS, ODOM_SCALE_LEFTDIAMETER, ODOM_SCALE_RIGHTDIAMETER, ODOM_SCALE_MIDDLEDIAMETER});
    m_chassis->withOdometry({104.52,132.54,179.09},std::make_shared<pros::Rotation>(ODOM_ROT_PORT_LEFT,ODOM_ROT_REVERSED_LEFT), std::make_shared<pros::Rotation> (ODOM_ROT_PORT_RIGHT,ODOM_ROT_REVERSED_RIGHT),{ODOM_SCALE_LEFTRADIUS, ODOM_SCALE_RIGHTRADIUS, ODOM_SCALE_MIDDLERADIUS, ODOM_SCALE_LEFTDIAMETER, ODOM_SCALE_RIGHTDIAMETER, ODOM_SCALE_MIDDLEDIAMETER});
    //13146,8114,-8933
    //4661,1022
    //12821,9290,27000 ; 12883,8160 ;10452,13254,17909
    #if (ROBOT == RIZ)
    //Shooter
    std::shared_ptr<Motor> sh_A = std::make_shared<Motor>(3,SHOOT_LEFTMTR_REVERSED,okapi::AbstractMotor::gearset::red);
    std::shared_ptr<Motor> sh_B = std::make_shared<Motor>(4,SHOOT_RIGHTMTR_REVERSED,okapi::AbstractMotor::gearset::red);

    std::shared_ptr<Motor> lch_A = std::make_shared<Motor>(12,true,okapi::AbstractMotor::gearset::red);
    std::shared_ptr<Motor> lch_B = std::make_shared<Motor>(18,false,okapi::AbstractMotor::gearset::red);
   
    std::shared_ptr<MotorGroup> shooterMtrs = std::make_shared<MotorGroup>(MotorGroup({sh_A,sh_B}));
    std::shared_ptr<MotorGroup> shooterLatches = std::make_shared<MotorGroup>(MotorGroup({lch_A,lch_B}));
    m_shooter.reset(new Shooter(0,1300,shooterMtrs,shooterLatches, std::make_shared<pros::ADIDigitalIn>('B')));

    //Intake
    std::shared_ptr<Motor> in_A = std::make_shared<Motor>(5,true,okapi::AbstractMotor::gearset::blue);
    MotorGroup intakeMtrs = MotorGroup({in_A});
    m_intake.reset(new ConveyorIntake(intakeMtrs,pros::ADIAnalogIn('B')));

    //Roller
    std::shared_ptr<Motor> roller = std::make_shared<Motor>(ROLLER_MTR_PORT);

    std::shared_ptr<MotorGroup> rollerGroup = std::make_shared<MotorGroup>(MotorGroup({roller}));
    std::shared_ptr<Vision> visionGroup = std::make_shared<Vision>(ROLLER_SNR_PORT);

    m_roller.reset(new Roller(rollerGroup, visionGroup, true));
    m_roller->setupVision();

    #elif ROBOT == AVENGER

    //Intake
    std::shared_ptr<Motor> in_A = std::make_shared<Motor>(INTAKE_MTR_PORT,INTAKE_MTR_REVERSED,okapi::AbstractMotor::gearset::blue);
    MotorGroup intakeMtrs = MotorGroup({in_A});
    m_intake.reset(new ConveyorIntake(intakeMtrs,pros::ADIAnalogIn('B')));

    //Roller
    std::shared_ptr<Motor> roller = std::make_shared<Motor>(ROLLER_MTR_PORT);

    std::shared_ptr<MotorGroup> rollerGroup = std::make_shared<MotorGroup>(MotorGroup({roller}));
    std::shared_ptr<Vision> visionGroup = std::make_shared<Vision>(ROLLER_SNR_PORT);

    m_roller.reset(new Roller(rollerGroup, visionGroup, true));
    m_roller->setupVision();

    //Expansion
    std::shared_ptr<Pneumatic> pneumatic = std::make_shared<Pneumatic>(pros::ADIDigitalOut(EXP_PORT));
    expand.reset(new Expansion(pneumatic));

    #endif
    
    m_traveler.reset(new Traveler(&g_Avenger_2,{0.3,0,0,0},{1.0,0.0000,0,0},{0.002,0,0,0},7,2));
    m_traveler->withChassis(m_chassis);
    // m_traveler->withExpansion();
    m_traveler->withIntake(m_intake);
    // m_traveler->withShooter(m_shooter);
    // m_traveler->withRoller(m_roller);
    m_autonDone = false;

    //Load autonomous screen
    m_display.reset(new Display());
    a_config_t autoConfig;
    //If any autos are turning roller to wrong colour, go to the functions at the bottom and change the alliance colour.
    AutoSelector autonSelector = AutoSelector(0,{
        {"Riz",tab1_Btn_Style,tab1_Press_Style,selectStyle},
        {"Avenger",tab1_Btn_Style,tab1_Press_Style,selectStyle},
    },{
        {"[R] Roller Front","Get roller",0,Robot::r1_red_roller},
        {"[B] Roller Front","Get roller",0,Robot::r1_blue_roller},
        
        {"[R] Roller Side","Get roller",1,Robot::r2_red_short},
        {"[B] Roller Side","Get roller",1,Robot::r2_blue_short},

        {"Skills","2 Rollers, extension",1,Robot::r2_red_long},
    },autoConfig);
    m_display->withAutoSelector(&autonSelector);
    m_display->loadAutoSelector();

    m_driver.reset(new okapi::Controller());
    startMotorController();
}

void Robot::autoInit(){
    m_autoInit = false;
    m_autonDone = false;
    m_display->runAutonFunction();
    m_chassis->startOdometry();
}

void Robot::autonomous(){
    if(!m_autonDone)
        m_autonDone = m_traveler->step();
}

void Robot::driverInit(){
    pathTest = false;
    m_chassis->startOdometry();
}

void Robot::driverControl(){
    double left = m_driver->getAnalog(okapi::ControllerAnalog::leftY);
	double right = -m_driver->getAnalog(okapi::ControllerAnalog::rightX);

    // if(tmr == 0){
    //     m_chassis->moveChassis(100, 100);
    // }

    // if(tmr == 100){
    //     m_chassis->moveChassis(50, 50);
    // }

    // if(tmr == 200){
    //     m_chassis->moveChassis(-100, -100);
    // }

    // if(tmr == 300){
    //     m_chassis->moveChassis(0, 0);
        
    // }

    // if(tmr == 400){
    //     m_chassis->moveChassis(0, 0);
    //     stopMotorController();
    // }
    // tmr++;

    // if(!pathTest && m_driver->getDigital(okapi::ControllerDigital::B)){
    //     pathTest = m_traveler->step();
    // }
    // else{
    //     std::cout << "Complete" << std::endl;
    //     m_chassis->moveChassis(0,0);
    //     m_chassis->stopOdometry();
    // } 

    // New Feb 19 2023 Comp Code
    // Drive
    m_chassis->arcade(left, right);

    // Roller
    if (m_driver->getDigital(okapi::ControllerDigital::R1)) {
        m_roller->moveRoller(600);
        
    } else if (m_driver->getDigital(okapi::ControllerDigital::R2)) {
        m_roller->moveRoller(-600);

    } else {
        m_roller->moveRoller(0);
    }

    // Intake
    if (m_driver->getDigital(okapi::ControllerDigital::L1)) {
        if (intakeState == IntakeState::OFF) {
            intakeState = IntakeState::INTAKING;
        } else if (intakeState == IntakeState::INTAKING) {
            intakeState = IntakeState::OFF;
        } else if (intakeState == IntakeState::OUTTAKING) {
            intakeState = IntakeState::INTAKING;
        }
        pros::delay(20); // debounce the button
    } else if (m_driver->getDigital(okapi::ControllerDigital::L2)) {
        if (intakeState == IntakeState::OFF) {
            intakeState = IntakeState::OUTTAKING;
        } else if (intakeState == IntakeState::INTAKING) {
            intakeState = IntakeState::OUTTAKING;
        } else if (intakeState == IntakeState::OUTTAKING) {
            intakeState = IntakeState::OFF;
        }
        pros::delay(20); // debounce the button
    }

    if (intakeState == IntakeState::INTAKING) {
        m_intake->moveVelocity(600);
    } else if (intakeState == IntakeState::OUTTAKING) {
        m_intake->moveVelocity(-600);
    } else {
        m_intake->moveVelocity(0);
    }

    //pros::delay(10); // update loop at 100 Hz

    // Expansion

    // if (m_driver->getDigital(okapi::ControllerDigital::R1)) {
    //     m_expansion->moveVelocity(600);
    // } else if (m_driver->getDigital(okapi::ControllerDigital::R2)) {
    //     m_expansion->moveVelocity(-600);
    // } else {
    //     m_expansion->moveVelocity(0);
    // }
    if (m_driver->getDigital(okapi::ControllerDigital::B) && m_driver->getDigital(okapi::ControllerDigital::down)) {
        expand->launch();
    }
    
    /* if (m_driver->getDigital(okapi::ControllerDigital::L2)) {
        switch (intakeState) {
            case IntakeState::OFF:
                intakeState = IntakeState::INTAKING;
                break;
            case IntakeState::INTAKING:
                intakeState = IntakeState::OUTTAKING;
                break;
            case IntakeState::OUTTAKING:
                intakeState = IntakeState::OFF;
                break;
        }
        pros::delay(200); // debounce the button
    }
    if (m_driver->getDigital(okapi::ControllerDigital::L1)) {
        switch (intakeState) {
            case IntakeState::OFF:
                intakeState = IntakeState::INTAKING;
                break;
            case IntakeState::INTAKING:
                intakeState = IntakeState::OUTTAKING;
                break;
            case IntakeState::OUTTAKING:
                intakeState = IntakeState::OFF;
                break;
        }
        pros::delay(200); // debounce the button
    } */
    /* if (m_driver->getDigital(okapi::ControllerDigital::L2)) {
        switch (intakeState) {
            case IntakeState::OFF:
                intakeState = IntakeState::INTAKING;
                break;
            case IntakeState::INTAKING:
                intakeState = IntakeState::OUTTAKING;
                break;
            case IntakeState::OUTTAKING:
                intakeState = IntakeState::OFF;
                break;
        }
        pros::delay(200); // debounce the button
    }

    if (intakeState == IntakeState::INTAKING) {
        m_intake->moveVelocity(900);
    } else if (intakeState == IntakeState::OUTTAKING) {
        m_intake->moveVelocity(-900);
    } else {
        m_intake->moveVelocity(0);
    } */

    /* if(m_driver->getDigital(okapi::ControllerDigital::L2)) {
        intakeOn = !intakeOn; // toggle the intake state
        pros::delay(200); // debounce the button
    }

    if(intakeOn) {
        m_intake->moveVelocity(600);
    } else {
        m_intake->moveVelocity(0);
    } */


    /* enum class IntakeState {
    OFF,
    INTAKING,
    OUTTAKING
    };

    IntakeState intakeState = IntakeState::OFF;

    if (m_driver->getDigital(okapi::ControllerDigital::L2)) {
        intakeState = IntakeState::INTAKING;
    } else if (m_driver->getDigital(okapi::ControllerDigital::L1)) {
        intakeState = IntakeState::OUTTAKING;
    } else {
        intakeState = IntakeState::OFF;
    }

    switch (intakeState) {
        case IntakeState::OFF:
            m_intake->moveVelocity(0);
            break;
        case IntakeState::INTAKING:
            m_intake->moveVelocity(600);
            break;
        case IntakeState::OUTTAKING:
            m_intake->moveVelocity(-600);
            break;
    } */



    // Flywheel Shooter

    // Endgame

    /* if(m_driver->getDigital(okapi::ControllerDigital::L2)) {
        if (btnPress == 1) {
            btnPress = 0;
        } else {
            btnPress = 1;
        }
        pros::delay(20);
    }

    if(m_driver->getDigital(okapi::ControllerDigital::L1)) {
        if (btnPress == -1) {
            btnPress = 0;
            pros::delay(20);
        } else {
            btnPress = -1;
            pros::delay(20);
        }
        pros::delay(20);
    }

    m_intake->moveVelocity(btnPress * 600); */
    // if(m_driver->getDigital(okapi::ControllerDigital::R1)) {
    //     m_roller->moveRoller(100);
    // } else {
    //     m_roller->moveRoller(0);
    // }

    // if(m_driver->getDigital(okapi::ControllerDigital::R2)) {
    //     m_roller->moveRoller(-100);
    // } else {
    //     m_roller->moveRoller(0);
    // }
    // if(m_driver->getDigital(okapi::ControllerDigital::A)) {
    //     //m_shooter->shoot();
    //     stopMotorController();
    // }





    // if(m_driver->getDigital(okapi::ControllerDigital::L2) && btnPress == 1 && !last) {
    //     last = true;
    //     btnPress = 0;
    // } else if (m_driver->getDigital(okapi::ControllerDigital::L2) && btnPress == 0 && !last) {
    //     last = true;
    //     btnPress = 1;
    // } else {
    //     m_intake->moveVelocity(0);
    //     last = false;
    // }
    // if(m_driver->getDigital(okapi::ControllerDigital::L1) && btnPress == -1 && !last) {
    //     last = true;
    //     btnPress = 0;
    // } else if (m_driver->getDigital(okapi::ControllerDigital::L1) && btnPress == 0 && !last) {
    //     last = true;
    //     btnPress = -1;
    // } else {
    //     m_intake->moveVelocity(0);
    //     last = false;
    // }


//     // std::cout << m_shooter->getEncoderValue() << "," << m_shooter->m_stallCount << std::endl;
//     // pros::lcd::print(3,"%f",m_shooter->m_latches->getEncoderValue());

//     /* if(m_driver->getDigital(okapi::ControllerDigital::Y)){
//         m_shooter->closeLatch();
//     }

//     if(m_driver->getDigital(okapi::ControllerDigital::B)){
//         m_shooter->openLatch();
//     } */
    
//     // if(m_driver->getDigital(okapi::ControllerDigital::Y)){
//     //     m_testMtrs->moveVelocity(-100);
//     // }

//     // if((!m_driver->getDigital(okapi::ControllerDigital::X)) && (!m_driver->getDigital(okapi::ControllerDigital::Y)))
//     //     m_testMtrs->moveVelocity(0);
    


//     if(m_shoot && m_driver->getDigital(okapi::ControllerDigital::A)){
//         m_shoot = false;
//     }
//     if(!m_shoot)
//         m_shoot = m_shooter->shoot();
//     if(!m_shoot3)     
//         m_shoot3 = m_shooter->initShooter();

    // if(m_driver->getDigital(okapi::ControllerDigital::A)) {
    //     tmr = 400;
    //     m_chassis->moveChassis(0, 0);
    // }
    //     m_chassis->stopOdometry();
        //m_traveler->saveLog();

    //m_chassis->arcade(left, right);

    // if(m_driver->getDigital(okapi::ControllerDigital::A)) {
    //     //m_shooter->shoot();
    //     stopMotorController();
    // }

//     if(m_driver->getDigital(okapi::ControllerDigital::R1)) {
//         m_roller->moveRoller(100);
//     } else {
//         m_roller->moveRoller(0);
//     }

//     if(m_driver->getDigital(okapi::ControllerDigital::R2)) {
//         m_roller->moveRoller(-100);
//     } else {
//         m_roller->moveRoller(0);
//     }

    // if(m_driver->getDigital(okapi::ControllerDigital::L2) && btnPress == 1 && !last) {
    //     last = true;
    //     btnPress = 0;
    // } else if (m_driver->getDigital(okapi::ControllerDigital::L2) && btnPress == 0 && !last) {
    //     last = true;
    //     btnPress = 1;
    // } else {
    //     m_intake->moveVelocity(0);
    //     last = false;
    // }
    // if(m_driver->getDigital(okapi::ControllerDigital::L1) && btnPress == -1 && !last) {
    //     last = true;
    //     btnPress = 0;
    // } else if (m_driver->getDigital(okapi::ControllerDigital::L1) && btnPress == 0 && !last) {
    //     last = true;
    //     btnPress = -1;
    // } else {
    //     m_intake->moveVelocity(0);
    //     last = false;
    // }
    // m_intake->moveVelocity(btnPress * 600);
//     /* if(m_driver->getDigital(okapi::ControllerDigital::L2)) {
//         if (btnPress == 1) {
//             btnPress = 0;
//         } else {
//             btnPress = 1;
//         }
//         pros::delay(20);
//     }

//     if(m_driver->getDigital(okapi::ControllerDigital::L1)) {
//         if (btnPress == -1) {
//             btnPress = 0;
//             pros::delay(20);
//         } else {
//             btnPress = -1;
//             pros::delay(20);
//         }
//         pros::delay(20);
//     } */

//     // std::cout << m_shooter->getEncoderValue() << "," << m_shooter->m_stallCount << std::endl;
//     // pros::lcd::print(3,"%f",m_shooter->m_latches->getEncoderValue());

//     /* if(m_driver->getDigital(okapi::ControllerDigital::Y)){
//         m_shooter->closeLatch();
//     }

//     if(m_driver->getDigital(okapi::ControllerDigital::B)){
//         m_shooter->openLatch();
//     } */
    
//     // if(m_driver->getDigital(okapi::ControllerDigital::Y)){
//     //     m_testMtrs->moveVelocity(-100);
//     // }

//     // if((!m_driver->getDigital(okapi::ControllerDigital::X)) && (!m_driver->getDigital(okapi::ControllerDigital::Y)))
//     //     m_testMtrs->moveVelocity(0);
    


//     if(m_shoot && m_driver->getDigital(okapi::ControllerDigital::A)){
//         m_shoot = false;
//     }
//     if(!m_shoot)
//         m_shoot = m_shooter->shoot();
//     if(!m_shoot3)     
//         m_shoot3 = m_shooter->initShooter();
}

void Robot::r1_red_roller(){
    m_traveler->m_allianceColour = 2;
    m_traveler->setPath(&g_Auto_1);
    m_Odometry->setPose({36.00,10.00,0.0});
}
void Robot::r1_blue_roller(){
    m_traveler->m_allianceColour = 1;
    m_traveler->setPath(&g_Auto_1);
    m_Odometry->setPose({36.00,10.00,0.0});
}

void Robot::r2_red_short(){
    m_traveler->m_allianceColour = 2;
    m_traveler->setPath(&g_Avenger);
    m_Odometry->setPose({128.83,81.60,90});
}
void Robot::r2_blue_short(){
    m_traveler->m_allianceColour = 1;
    m_traveler->setPath(&g_Avenger);
    m_Odometry->setPose({128.83,81.60,90});
}
void Robot::r2_red_long(){
    m_traveler->m_allianceColour = 2;
    m_traveler->setPath(&g_Avenger_2);
    m_Odometry->setPose({104.52,132.54,179.09});
}




//Unused
void Robot::r2_blue_long(){
    m_traveler->m_allianceColour = 1;
   // m_traveler->setPath(&g_Auton_2_B);
    m_Odometry->setPose({131.46,81.14,-89.33});
}

void Robot::r2_red_long_B(){
    m_traveler->m_allianceColour = 2;
    //m_traveler->setPath(&g_Auton_2_C);
    m_Odometry->setPose({131.46,81.14,-89.33});
}
void Robot::r2_blue_long_B(){
    m_traveler->m_allianceColour = 1;
    //m_traveler->setPath(&g_Auton_2_C);
    m_Odometry->setPose({131.46,81.14,-89.33});
}