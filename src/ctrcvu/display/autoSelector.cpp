#include "autoSelector.hpp"

using namespace sparkyLib;

std::vector<a_tabStyle_t> AutoSelector::m_Styles;
std::vector<a_auto_t> AutoSelector::m_autonbtns;
std::vector<a_tab_t> AutoSelector::m_autontabs;
int AutoSelector::m_selectedBtn;
lv_style_t AutoSelector::m_descriptionStyle;
lv_obj_t *AutoSelector::m_desObj;
lv_obj_t *AutoSelector::m_deslbl;
lv_obj_t *AutoSelector::m_tabContainer;

AutoSelector::AutoSelector(int selected, std::vector<a_tab_t> tabs, std::vector<a_auto_t> autons, a_config_t config){
    //Initialize class members
    m_config = config;
    if(selected >= autons.size())
        selected = 0;
    m_selectedBtn = selected;
    m_autontabs = tabs;
    m_autonbtns = autons;

    //Set up description style
    lv_style_copy(&m_descriptionStyle, &lv_style_plain);
    m_descriptionStyle.body.main_color = m_config.des_bg;
    m_descriptionStyle.body.grad_color = m_config.des_bg;
    m_descriptionStyle.body.border.color = m_config.des_border;
    m_descriptionStyle.body.border.width = 2;
    m_descriptionStyle.text.color = m_config.des_font;
    m_descriptionStyle.text.font = &lv_font_dejavu_20;
}

int AutoSelector::runSelectedAuto(){
    //Make sure an item has been selected
    if(m_selectedBtn == -1)
        return 0;
    
    //Make sure the callback is set
    if(m_autonbtns[m_selectedBtn].callback == nullptr)
        return 0;

    //Call auton function
    (*m_autonbtns[m_selectedBtn].callback)();
    return 1;
}

void AutoSelector::render(){
    //Load screen
    loadScreen(m_config.scr_config);
    
    //Create container for the tabs
    m_tabContainer = lv_tabview_create(lv_scr_act(), NULL);
    lv_obj_set_size(m_tabContainer, m_config.scr_config.screenWidth, m_config.scr_config.screenHeight);
    lv_point_t pos = getScreenPos(0,0);
    lv_obj_set_pos(m_tabContainer, pos.x, pos.y);

    a_tabStyle_t container;

    //Loop through the tabs
    for(int i = 0; i < m_autontabs.size(); i++){
        //Generate the styles and store them in a static vector list
        container.autonStyle = generateStyle(m_autontabs[i].btnStyle, &lv_style_pretty_color);
        container.pressedStyle = generateStyle(m_autontabs[i].pressedStyle, &lv_style_pretty_color);
        container.selectedStyle = generateStyle(m_autontabs[i].selectedStyle, &lv_style_pretty_color);

        m_Styles.push_back(container);

        //Create tab
        m_autontabs[i].tabObj = lv_tabview_add_tab(m_tabContainer, m_autontabs[i].name.c_str());
    }

    //Description object
    m_desObj = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(m_desObj, m_config.scr_config.screenWidth, m_config.desHeight);
    pos = getScreenPos(0,SCREEN_HEIGHT - m_config.desHeight);
    lv_obj_set_pos(m_desObj, pos.x, pos.y);
    lv_obj_set_style(m_desObj, &m_descriptionStyle);

    //Description label
    m_deslbl = lv_label_create(m_desObj, NULL);
    lv_obj_set_pos(m_deslbl, 10, (m_config.desHeight / 2) - 10);
    lv_label_set_text(m_deslbl, "");
    
    
    //Loop through the buttons
    int tabIndex = 0;
    for(int i = 0; i < m_autonbtns.size(); i++){
        //Make sure the auton's tab reference exists -- the number isn't larger than the number of tabs
        tabIndex = m_autonbtns[i].tabContainer;
        if(tabIndex >= m_autontabs.size())
            continue;

        //Make sure we haven't reached the allowed number for the tab
        if(m_autontabs[tabIndex].count >= 6)
            continue;
        
        //Create the button
        m_autonbtns[i].btnObj = lv_btn_create(m_autontabs[tabIndex].tabObj, NULL);
        
        //Set styles -- if this is the selected button set style as selected
        if(i == m_selectedBtn){
            setStyle(m_autonbtns[i].btnObj, &m_Styles[tabIndex].selectedStyle, &m_Styles[tabIndex].selectedStyle);
            //Update description
            lv_label_set_text(m_deslbl, m_autonbtns[i].description.c_str());
        }
        else
            setStyle(m_autonbtns[i].btnObj, &m_Styles[tabIndex].autonStyle, &m_Styles[tabIndex].pressedStyle);
        
        //Set button position and size -- Uses the tab's x,y value
        lv_obj_set_pos(m_autonbtns[i].btnObj, m_autontabs[tabIndex].x, m_autontabs[tabIndex].y);
        lv_obj_set_size(m_autonbtns[i].btnObj, m_config.btnWidth, m_config.btnHeight);
        //Set the free number = to the index value to make easy to identify later on.
        lv_obj_set_free_num(m_autonbtns[i].btnObj, i);

        //Add label to the button
        m_autonbtns[i].lblObj = lv_label_create(m_autonbtns[i].btnObj, NULL);
        lv_label_set_text(m_autonbtns[i].lblObj, m_autonbtns[i].name.c_str());
        
        //Update the tab's x,y value for the next button
        m_autontabs[tabIndex].x += m_config.btnWidth + m_config.btnHozSpace;
        //If the position is > 380 the next button needs to go on the next line
        if(m_autontabs[tabIndex].x > (m_config.scr_config.screenWidth - m_config.btnWidth)){
            m_autontabs[tabIndex].x = 0;
            m_autontabs[tabIndex].y += m_config.btnHeight + m_config.btnVerSpace;
        }

        //Set button pressed action
        lv_btn_set_action(m_autonbtns[i].btnObj, LV_BTN_ACTION_CLICK, btnSelected);

        //Increment tab count to track how many buttons have been added.
        m_autontabs[tabIndex].count++;
    }
}

void AutoSelector::remove(){
    lv_obj_clean(m_tabContainer);
    lv_obj_clean(m_desObj);
    lv_obj_clean(m_parent);

    lv_obj_del(m_tabContainer);
    lv_obj_del(m_desObj);
    lv_obj_del(m_parent);
}

lv_res_t AutoSelector::btnSelected(lv_obj_t *button){
    //Grab previous selected button's tab number
    int tabIndex = m_autonbtns[m_selectedBtn].tabContainer;

    //Deselect the current button
    if(m_selectedBtn != -1)
        setStyle(m_autonbtns[m_selectedBtn].btnObj, &m_Styles[tabIndex].autonStyle, &m_Styles[tabIndex].pressedStyle);

    //Update selected variable
    m_selectedBtn = lv_obj_get_free_num(button);

    //Update tab number
    tabIndex = m_autonbtns[m_selectedBtn].tabContainer;

    //Update button style to appear as selected
    setStyle(m_autonbtns[m_selectedBtn].btnObj, &m_Styles[tabIndex].selectedStyle, &m_Styles[tabIndex].selectedStyle);
    
    //Update description
    lv_label_set_text(m_deslbl, m_autonbtns[m_selectedBtn].description.c_str());

    return LV_RES_OK;
}