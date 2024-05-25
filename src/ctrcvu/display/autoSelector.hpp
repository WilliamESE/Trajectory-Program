#pragma once

#include "api.h"
#include "screen.hpp"
#include <iostream>
#include <vector>
#include <string>

namespace sparkyLib {

//Auton structure
typedef struct {
    std::string name;
    std::string description;
    int tabContainer = 0;
    void (*callback)() = nullptr;
    lv_obj_t *btnObj = nullptr;
    lv_obj_t *lblObj = nullptr;
}a_auto_t;

//Tab structure
typedef struct {
    std::string name;
    s_style_t btnStyle;
    s_style_t pressedStyle;
    s_style_t selectedStyle;
    lv_obj_t *tabObj = nullptr;
    int x = 0;
    int y = 20;
    int count = 0;
}a_tab_t;

//Style structure:
typedef struct {
    lv_style_t autonStyle;
    lv_style_t pressedStyle;
    lv_style_t selectedStyle;
}a_tabStyle_t;

typedef struct{
    //Screen options
    s_config scr_config;

    //Buttons
    int btnWidth = 145;
    int btnHeight = 40;
    int btnHozSpace = 10;
    int btnVerSpace = 10;

    //Description
    int desHeight = 54;
    lv_color_t des_bg = LV_COLOR_MAKE(0x00,0x00,0x00);
    lv_color_t des_font = LV_COLOR_MAKE(0xFF,0xFF,0xFF);
    lv_color_t des_border = LV_COLOR_MAKE(0x07,0x07,0x07);
}a_config_t;

class AutoSelector : public sparkyLib::Screen {
    private:
        //Vectors to store tabs and buttons for autons
        static std::vector<a_auto_t> m_autonbtns;
        static std::vector<a_tab_t> m_autontabs;

        //Track which button is currently selected
        static int m_selectedBtn;

        //Style objects
        static std::vector<a_tabStyle_t> m_Styles;
        static lv_style_t m_errorStyle;
        static lv_style_t m_descriptionStyle;

        //Description object and label
        static lv_obj_t *m_desObj;
        static lv_obj_t *m_deslbl;

        //Tab container which houses all the tabs
        static lv_obj_t *m_tabContainer;

        //Configuration
        a_config_t m_config;

        /**
         * @brief Catch button pressed event. Changes the style of the previous and newly selected button
         *      Updates the m_selectedBtn variable and description
         * @param button The button being pressed
         * @return lv_res_t
         */
        static lv_res_t btnSelected(lv_obj_t *button);
    
    public:
        /**
         * @brief Construct a new Auto Selector object
         * 
         * @param tabs List of tabs {name, base style, pressed style, selected style}
         * @param autons List of autons {name (13 length), description (52 length), tab number, function to call}
         */
        AutoSelector(int selected, std::vector<a_tab_t> tabs, std::vector<a_auto_t> autons, a_config_t config);

        /**
         * @brief Draw the auto selector
         * 
         */
        void render() override;
        void remove() override;

        /**
         * @brief Run the function of the selected autonomous
         * 
         * @return int 0 - error occured (no selected auton, no function provided); 1 - success
         */
        int runSelectedAuto();
};

}//nameapce lamaLib