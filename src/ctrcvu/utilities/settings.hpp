#pragma once

#include <string>
#include <vector>
#include <map>
#include <cmath>
#include "logger.hpp"

namespace sparkyLib{

void configTestFunction();

class Configuration {
    public:
        enum SET_TYPE{
            INTEGER,
            DOUBLE,
            STRING,
            CONST,
        };
        typedef struct {
            int id;
            SET_TYPE type;
            std::string name;   //Easy identification of the setting
            int value;          //The current value of the setting, ints may reference other data like strings
            int previous;       //The previous value
            int factory;        //The default value if reset
            int decimals;       //Number of decimal places of the value if needed
            int min;            //For modifying the value this is the minimum it can be
            int max;            //For modifying the value this is the maximum it can be
            std::string details;//Description of what the setting does
        }setting_t;

    private:
    std::vector<std::string> m_strings;
    std::map<std::string, setting_t> m_settings;

    /**
     * @brief Gets settings from sd card
     * 
     * @return int returns 1 on success, 0 on failure
     */
    int retrieveSettings();
    
    public:
    /**
     * @brief Construct a new Configuration object
     * Auto retrieves the settings from the SD card
     */
    Configuration();

    /**
     * @brief Write all the settings to the SD Card
     * 
     * @return int returns 1 on success, 0 on failure
     */
    int saveSettings();

    /**
     * @brief Resets the setting to its factory value
     * 
     * @param id The setting to reset
     */
    void resetSetting(std::string name);

    /**
     * @brief Reloads the previous value of the setting
     * 
     * @param id The setting to undo last change
     */
    void undoSetting(std::string name);

    /**
     * @brief Get the value of a setting as an integer
     * 
     * @param name The name of the setting
     * @return int 
     */
    int getIntValue(std::string name);
    /**
     * @brief Get the value of a setting as a double
     * 
     * @param name The name of the setting
     * @return double 
     */
    double getDoubleValue(std::string name);
    /**
     * @brief Get the value of a setting as a string
     * 
     * @param name The name of the setting
     * @return string 
     */
    std::string getStringValue(std::string name);
    /**
     * @brief Get the setting object
     * 
     * @param name The name of the setting
     * @return setting_t 
     */
    setting_t getSetting(std::string name);

    /**
     * @brief Modify the value of a setting
     * 
     * @param name The name of the setting
     * @param value The new value
     */
    void updateSetting(std::string name, int value);
    void updateSetting(std::string name, double value);
    void updateSetting(std::string name, std::string value);
    void updateSetting(std::string name, setting_t value);

    /**
     * @brief Adds the setting to the list
     * 
     * @param setting 
     */
    void addSetting(setting_t setting);

    //Strings
    /**
     * @brief Add the string from the database list
     * 
     * @param str 
     */
    void addString(std::string str);

    /**
     * @brief Removes the string from the database list
     * 
     * @param str 
     */
    void removeString(std::string str);
};
}
