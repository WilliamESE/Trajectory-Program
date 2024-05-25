#pragma once

#include "display/lvgl.h"
#include "autoSelector.hpp"
#include <vector>

namespace sparkyLib {

class Display {

    lv_obj_t *m_root;
    std::vector<lv_obj_t *> menuItems;
    AutoSelector *autoSelector;

    public:
    Display();

    Display &withAutoSelector(int selected, std::vector<a_tab_t> tabs, std::vector<a_auto_t> autons, a_config_t config);
    Display &withAutoSelector(AutoSelector *autoSelect);

    //Loading functions
    void loadAutoSelector();
    void runAutonFunction();
};

}//sparkyLib