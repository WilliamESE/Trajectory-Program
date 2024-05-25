#pragma once

#include "display/lvgl.h"
#include <string>

namespace sparkyLib {

#define SCREEN_WIDTH    480
#define SCREEN_HEIGHT   239

//Simple style stucture
typedef struct {
    lv_color_t mainColour;
    lv_color_t gradientColour;
    lv_color_t textColour;
}s_style_t;

typedef enum {
    TOP,
    RIGHT,
    BOTTOM,
    LEFT,
}S_ORIENT_T;

typedef struct {
    lv_color_t bg = LV_COLOR_MAKE(0x00,0x00,0x00);
    int screenWidth = SCREEN_WIDTH;
    int screenHeight = SCREEN_HEIGHT;
    S_ORIENT_T screenOrientation = LEFT;
    lv_color_t border = LV_COLOR_MAKE(0x00,0x00,0x00);
    int borderWidth = 0;
}s_config;

class Screen {
    protected:
    
    lv_style_t m_backgroundStyle;
    lv_obj_t *m_parent;
    lv_area_t m_position;

    lv_point_t getScreenPos(short int x, short int y);
    lv_point_t getScreenPos(lv_point_t pos);
    short int getScreenX(short int x);
    short int getScreenY(short int y);

    private:

    lv_area_t calculateCoords(s_config config);

    public:
    /**
     * @brief Displays the screen on the LCD of the V5 brain
     * 
     */
    virtual void render() = 0;

    virtual void remove() = 0;

    void loadScreen(s_config config);

    /**
    * @brief Converts the simiplist t_style structure to the lvgl stule structure lv_style_t
    * 
    * @param style The t_style structure to convert
    * @param copy The base style to apply the t_style to
    * @return lv_style_t 
    */
    lv_style_t generateStyle(s_style_t style, lv_style_t *copy);

    /**
    * @brief Load styles into auton object
    * 
    * @param obj The object to load the styles into
    * @param style Main style
    * @param pressedStyle Pressed style
    */
    static void setStyle(lv_obj_t *obj, lv_style_t *style, lv_style_t *pressedStyle);

    /**
     * @brief Set the Parent object
     * 
     * @param obj 
     */
    void setParent(lv_obj_t *obj) {m_parent = obj;}
};
}