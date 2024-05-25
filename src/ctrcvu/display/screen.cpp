#include "screen.hpp"

using namespace sparkyLib;

short int Screen::getScreenX(short int x){
    return x + m_position.x1;
}

short int Screen::getScreenY(short int y){
    return y + m_position.y1;
}

lv_point_t Screen::getScreenPos(short int x, short int y) {
    return {x + m_position.x1, y + m_position.y1};
}

lv_point_t Screen::getScreenPos(lv_point_t pos) {
    return {pos.x + m_position.x1, pos.y + m_position.y1};
}


void Screen::setStyle(lv_obj_t *obj, lv_style_t *style, lv_style_t *pressedStyle){
    lv_btn_set_style(obj, LV_BTN_STYLE_REL, style);
    lv_btn_set_style(obj, LV_BTN_STYLE_PR, pressedStyle);
    lv_btn_set_style(obj, LV_BTN_STYLE_TGL_REL, style);
    lv_btn_set_style(obj, LV_BTN_STYLE_TGL_PR, pressedStyle);
}

lv_style_t Screen::generateStyle(s_style_t style, lv_style_t *copy){
    lv_style_t btnStyle;
    lv_style_copy(&btnStyle, copy);

    btnStyle.body.main_color = style.mainColour;
    btnStyle.body.grad_color = style.gradientColour;
    btnStyle.text.color = style.textColour;
    return btnStyle;
}

void Screen::loadScreen(s_config config){
    lv_style_copy(&m_backgroundStyle, &lv_style_plain);
    m_backgroundStyle.body.main_color = config.bg;
    m_backgroundStyle.body.grad_color = config.bg;
    m_backgroundStyle.body.border.color = config.border;
    m_backgroundStyle.body.border.width = config.borderWidth;

    m_position = calculateCoords(config);

    m_parent = lv_page_create(lv_scr_act(), NULL);
    lv_obj_align(m_parent, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(m_parent, m_position.x2, m_position.y2);
    lv_obj_set_pos(m_parent, m_position.x1, m_position.y1);
    lv_obj_set_style(m_parent, &m_backgroundStyle);
}

lv_area_t Screen::calculateCoords(s_config config) {
    lv_area_t coords;
    switch(config.screenOrientation){
        case TOP:
            coords.x1 = 0;
            coords.y1 = 0;
            coords.x2 = SCREEN_WIDTH;
            coords.y2 = coords.y1 + config.screenHeight;
            break;
        case RIGHT:
            coords.x1 = (480 - config.screenWidth);
            coords.y1 = 0;
            coords.x2 = coords.x1 + config.screenWidth;
            coords.y2 = SCREEN_HEIGHT;
            break;
        case BOTTOM:
            coords.x1 = 0;
            coords.y1 = (272 - config.screenHeight);
            coords.x2 = SCREEN_WIDTH;
            coords.y2 = coords.y1 + config.screenHeight;
            break;
        case LEFT:
            coords.x1 = 0;
            coords.y1 = 0;
            coords.x2 = coords.x1 + config.screenWidth;
            coords.y2 = SCREEN_HEIGHT;
            break;
    }
    return coords;
}