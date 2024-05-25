#include "settings.hpp"
#include <sstream>
#include <stdexcept>

using namespace sparkyLib;

Configuration::Configuration(){
    int success = retrieveSettings();
    if(success){
        throw success;
    }
}

int Configuration::retrieveSettings(){
    Logger settingFile("settings","ini",READ);
    //The logger will retrieve one line at a time. Each item will be separated by a comma
    if(!settingFile.isOpen())
        return 1;
    while(!settingFile.isEOF()){
        std::stringstream line(settingFile.readline());
        std::vector<std::string> item;
        
        if(settingFile.isEOF())
            break;

        //Parse string
        while(line.good()){
            std::string subString;
            getline( line, subString, ',' );
            if(subString != "")
                item.push_back(subString);
        }

        if(item.size() != 10)
            continue;

        //Each item is a string
        setting_t setting = {
            std::stoi(item[0]),
            (SET_TYPE)std::stoi(item[1]),
            item[2],
            std::stoi(item[3]),
            std::stoi(item[4]),
            std::stoi(item[5]),
            std::stoi(item[6]),
            std::stoi(item[7]),
            std::stoi(item[8]),
            item[9]
        };
        m_settings.insert({item[2], setting});
    }
    settingFile.close();

    //String file
    Logger stringFile("strings","ini",READ);
    if(!stringFile.isOpen())
        return 3;
    while(!stringFile.isEOF()){
        std::string str = stringFile.readline();
        if(str != "")
            m_strings.push_back(str);
    }
    return 0;
}

int Configuration::saveSettings(){
    Logger settingFile("settings","ini");
    if(!settingFile.isOpen())
        return 0;
    //The setting file must be open
    for (auto const& x : m_settings){
        std::stringstream out;
        setting_t setting = x.second;

        out << setting.id << "," 
            << setting.type << "," 
            << setting.name << ","
            << setting.value << ","
            << setting.previous << ","
            << setting.factory << ","
            << setting.decimals << ","
            << setting.min << ","
            << setting.max << ","
            << setting.details;

        settingFile.writeline(out.str());
        out.str("");
    }

    Logger stringFile("strings","ini");
    if(!stringFile.isOpen())
        return 0;
    for(auto str: m_strings)
        stringFile.writeline(str);

    stringFile.close();
    settingFile.close();
    return 1;
}

void Configuration::resetSetting(std::string name){
    m_settings[name].value = m_settings[name].factory;
}

void Configuration::undoSetting(std::string name){
    m_settings[name].value = m_settings[name].previous;
}

void Configuration::addString(std::string str){
    m_strings.push_back(str);
}

void Configuration::removeString(std::string str){
    int cnt = 0;
    bool found = false;
    for(std::string vstr : m_strings){
        if(vstr == str)
            m_strings.erase(m_strings.begin() + cnt);
        cnt++;
    }        
}

int Configuration::getIntValue(std::string name){
    //Check if setting exists
    if (m_settings.find(name) == m_settings.end())
        return 0;
    
    //Get setting
    setting_t setting = m_settings[name];
    
    //Allow only ints
    if((setting.type == STRING) || (setting.type == DOUBLE))
        return 0;

    return setting.value;
}

double Configuration::getDoubleValue(std::string name){
    //Check if setting exists
    if (m_settings.find(name) == m_settings.end())
        return 0;

    //Get setting
    setting_t setting = m_settings[name];
    
    //Allow only doubles
    if((setting.type != DOUBLE))
        return 0;

    return ((double)(((double)setting.value) / (pow(10.0,(double)setting.decimals))));
}

std::string Configuration::getStringValue(std::string name){
    //Check if setting exists
    if (m_settings.find(name) == m_settings.end())
        return "";
    
    //Get setting
    setting_t setting = m_settings[name];
    
    //Allow only strings
    if((setting.type != STRING))
        return "";

    return m_strings[setting.value];
}

Configuration::setting_t Configuration::getSetting(std::string name){
    //Check if setting exists
    if (m_settings.find(name) == m_settings.end())
        throw 1;
    return m_settings[name];
}

void Configuration::updateSetting(std::string name, int value){
    //Check if setting exists
    if (m_settings.find(name) == m_settings.end())
        return;
    
    //Get setting
    setting_t setting = m_settings[name];

    //Only ints allowed
    if((setting.type == STRING) || (setting.type == DOUBLE))
        return;

    //Check min and max
    if(value < setting.min)
        value = setting.min;
    if(value > setting.max)
        value = setting.max;

    //Save previous
    setting.previous = setting.value;

    //Update current value
    setting.value = (int)value;
    m_settings[name] = setting;
}

void Configuration::updateSetting(std::string name, std::string value){
    //Check if setting exists
    if (m_settings.find(name) == m_settings.end())
        return;
    
    //Get setting
    setting_t setting = m_settings[name];

    if(setting.type != STRING)
        return;

    //Save previous
    setting.previous = setting.value;

    //Find string first
    int cnt = 0;
    for(std::string vstr : m_strings){
        if(vstr == value){
            setting.value = cnt;
            break;
        }
        cnt++;
    }

    m_settings[name] = setting;
}

void Configuration::updateSetting(std::string name, double value){
    //Check if setting exists
    if (m_settings.find(name) == m_settings.end())
        return;
    
    //Get setting
    setting_t setting = m_settings[name];

    if(setting.type != DOUBLE)
        return;

    int newValue = (int)(value * ((double)std::pow(10, setting.decimals)));
    
    //Check min and max
    if(newValue < setting.min)
        newValue = setting.min;
    if(newValue > setting.max)
        newValue = setting.max;

    //Save previous
    setting.previous = setting.value;

    //Find string first
    setting.value = newValue;
    m_settings[name] = setting;
}

void Configuration::updateSetting(std::string name, setting_t setting){
    if (m_settings.find(name) == m_settings.end())
        return;
    m_settings[name] = setting;
}

void Configuration::addSetting(setting_t setting){
    m_settings.insert({setting.name, setting});
}

void sparkyLib::configTestFunction(){
    try{
        Configuration settings;
        std::cout << "Settings Test" << std::endl;
        std::cout << "int Test: " << settings.getIntValue("intTest") << std::endl;
        std::cout << "double Test: " << settings.getDoubleValue("doubleTest") << std::endl;
        std::cout << "string Test: " << settings.getStringValue("stringTest") << std::endl;
        std::cout << "const Test: " << settings.getIntValue("constTest") << std::endl;

        //Change a setting
        std::cout << "Updating Double Setting" << std::endl;
        if(settings.getDoubleValue("doubleTest") == 2.5)
            settings.updateSetting("doubleTest", 10.5);
        else
            settings.updateSetting("doubleTest", 2.5);
        
        std::cout << "Saving" << std::endl;
        settings.saveSettings();

        std::cout << "Rerun: double should have a value of " << ((settings.getDoubleValue("doubleTest") == 2.5) ? "2.5" : "10.5") << std::endl;

    }
    catch(int &e){
        std::cout << "Setting File not found, making one" << std::endl;
        //Build settings file for test
        Logger sFile("settings","ini");
        std::stringstream str;
        str << 0 << "," << Configuration::INTEGER << "," << "intTest" << "," << 1 << "," << 1 << "," << 1 << "," << 0 << "," << 0 << "," << 1 << "," << "Just an integer for testing purposes";
        sFile.writeline(str.str());
        str.str("");
        str << 1 << "," << Configuration::DOUBLE << "," << "doubleTest" << "," << 25 << "," << 25 << "," << 25 << "," << 1 << "," << 0 << "," << 500 << "," << "Just an double for testing purposes";
        sFile.writeline(str.str());
        str.str("");
        str << 2 << "," << Configuration::STRING << "," << "stringTest" << "," << 0 << "," << 0 << "," << 0 << "," << 0 << "," << 0 << "," << 0 << "," << "Just an string for testing purposes";
        sFile.writeline(str.str());
        str.str("");
        str << 3 << "," << Configuration::CONST << "," << "constTest" << "," << 100 << "," << 100 << "," << 100 << "," << 0 << "," << 0 << "," << 1000 << "," << "Just an integer for testing purposes";
        sFile.writeline(str.str());
        str.str("");

        sFile.close();

        Logger strFile("strings","ini");
        strFile.writeline("Hello World");
        strFile.close();

        std::cout << "Setting File Created" << std::endl;

        try{
            Configuration settings;
            std::cout << "Settings Test" << std::endl;
            std::cout << "int Test: " << settings.getIntValue("intTest") << std::endl;
            std::cout << "double Test: " << settings.getDoubleValue("doubleTest") << std::endl;
            std::cout << "string Test: " << settings.getStringValue("stringTest") << std::endl;
            std::cout << "const Test: " << settings.getIntValue("constTest") << std::endl;

            //Change a setting
            std::cout << "Updating Double Setting" << std::endl;
            if(settings.getDoubleValue("doubleTest") == 2.5)
                settings.updateSetting("doubleTest", 10.5);
            else
                settings.updateSetting("doubleTest", 2.5);
            
            std::cout << "Saving" << std::endl;
            settings.saveSettings();

            std::cout << "Rerun: double should have a value of " << ((settings.getDoubleValue("doubleTest") == 2.5) ? "2.5" : "10.5") << std::endl;

        }
        catch(int &e){
            std::cout << "Setting Failed" << std::endl;
        }
    }
}