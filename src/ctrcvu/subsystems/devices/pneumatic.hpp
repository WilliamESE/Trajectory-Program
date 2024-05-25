#pragma once

#include "api.h"

namespace sparkyLib {
class Pneumatic {
    public:
    Pneumatic(pros::ADIDigitalOut ipneumatic, bool initState = false); 

    /**
     * @brief Sets the state of the pneumatic to either opened or closed
     * 
     * @param istate true = open; false = close
     */
    void setState(bool istate);

    /**
     * @brief Opens the pneumatic
     */
    void open();

    /**
     * @brief Closes the pneumatic
     */
    void close();

    /**
     * @brief Toggles the pneumatic between opened and closed 
     */
    void toggle();

    /**
     * @brief Gets the state of the pneumatic
     * 
     * @return true for open
     * @return false for closed
     */
    bool getState();

    private:
    pros::ADIDigitalOut pneumatic;
    bool state;
};
} // namespace lamaLib
