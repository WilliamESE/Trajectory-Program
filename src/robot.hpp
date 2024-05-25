#pragma once

#include "ctrcvu/ctrclib.hpp"
#include "paths/path.hpp"
#include "robotConfig.hpp"
#include "okapi/api.hpp"
#include "api.h"
#include <memory>

#define DARK_RED LV_COLOR_MAKE(0xAA,0x00,0x00)
#define DARK_BLUE LV_COLOR_MAKE(0x00,0x00,0xAA)
#define DARK_PURPLE LV_COLOR_MAKE(0x50,0x00,0x50)

class Robot {

    int tmr = 0;
    bool pathTest = false;
    bool m_shoot = true;
    bool m_shoot2 = true;
    bool m_shoot3 = false;
    bool m_autoInit = false;
    bool m_autonDone = false;

    sparkyLib::s_style_t tab1_Btn_Style = {LV_COLOR_PURPLE,LV_COLOR_PURPLE,LV_COLOR_WHITE};//{LV_COLOR_RED,LV_COLOR_RED,LV_COLOR_WHITE};
    sparkyLib::s_style_t tab1_Press_Style = {DARK_PURPLE,DARK_PURPLE,LV_COLOR_WHITE};//{DARK_RED,DARK_RED,LV_COLOR_WHITE};

    sparkyLib::s_style_t tab2_Btn_Style = {LV_COLOR_PURPLE,LV_COLOR_PURPLE,LV_COLOR_WHITE};//{LV_COLOR_BLUE,LV_COLOR_BLUE,LV_COLOR_WHITE};
    sparkyLib::s_style_t tab2_Press_Style = {DARK_PURPLE,DARK_PURPLE,LV_COLOR_WHITE};//{DARK_BLUE,DARK_BLUE,LV_COLOR_WHITE};

    sparkyLib::s_style_t tab3_Btn_Style = {LV_COLOR_PURPLE,LV_COLOR_PURPLE,LV_COLOR_WHITE};
    sparkyLib::s_style_t tab3_Press_Style = {DARK_PURPLE,DARK_PURPLE,LV_COLOR_WHITE};

    sparkyLib::s_style_t selectStyle = {LV_COLOR_GREEN,LV_COLOR_GREEN,LV_COLOR_WHITE};

    static void r1_red_roller();
    static void r1_blue_roller();

    static void r2_red_short();
    static void r2_blue_short();
    static void r2_red_long();
    static void r2_blue_long();
    static void r2_red_long_B();
    static void r2_blue_long_B();

    public:
        Robot();
        //Subsystems
        static std::shared_ptr<sparkyLib::Inline> m_chassis;
        static std::shared_ptr<okapi::Controller> m_driver;
        static std::shared_ptr<sparkyLib::Configuration> m_settings;
        static std::shared_ptr<sparkyLib::Odometry> m_Odometry;
        static std::shared_ptr<sparkyLib::Roller> m_roller;
        static std::shared_ptr<sparkyLib::Vision> m_vision;
        static std::shared_ptr<sparkyLib::Expansion> expand;
        static std::shared_ptr<Traveler> m_traveler;
        static std::shared_ptr<sparkyLib::Shooter> m_shooter;
        static std::shared_ptr<sparkyLib::ConveyorIntake> m_intake;
        static std::shared_ptr<sparkyLib::Display> m_display;

        //Functions
        void init();
        void autoInit();
        void autonomous();
        void driverInit();
        void driverControl();
};