#include "display.hpp"

using namespace sparkyLib;

Display::Display() {
    //Tabs cannot display vertically it appears :(
    
};

Display &Display::withAutoSelector(int selected, std::vector<a_tab_t> tabs, std::vector<a_auto_t> autons, a_config_t config) {
    autoSelector = new AutoSelector(selected, tabs, autons, config);
    return *this;
}

Display &Display::withAutoSelector(AutoSelector *autoSelect) {
    autoSelector = autoSelect;
    return *this;
}

void Display::loadAutoSelector(){
    // lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    // lv_img_set_src(img1, "/usd/Logo.bin");
    // // lv_obj_set_size(m_desObj, m_config.scr_config.screenWidth, m_config.desHeight);
    // lv_obj_set_pos(img1, 100, 100);
    //lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);

    autoSelector->render();
}

void Display::runAutonFunction(){
    autoSelector->runSelectedAuto();
}