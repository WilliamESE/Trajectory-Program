#include "../subsystems/devices/motorGroup.hpp"
#include <cmath>

namespace sparkyLib {
class ConveyorIntake { 
    public:
    // Define what motors will be used in the CoveyorIntake class as well as the line tracker sensor
    /**
    * @brief constructor for conveyor intake class to intake at most 3 disks at a time
    * @param imotors motor that controls the intake
    * @param lineTrackerSensor to count the number of disks currently in the intake
    *
    */
    ConveyorIntake(MotorGroup imotors, pros::ADIAnalogIn lineTrackerSensor);

    /**
    * @brief move the conveyor at a given velocity
    * @param ivel velocity to move the conveyor at
    *
    */
    void moveVelocity(int ivel);

    /**
     * @brief start the conveyor intake task
     * 
     */
    void startConveyor();

    /**
     * @brief end the conveyor intake task
     * 
     */
    void stopConveyor();

    /**
    * @brief calibrate the line tracker sensor (0.5s)
    *
    */
    void initializeLineTracker();

    /**
     * @brief read value of line tracker sensor to count disks
     *
     */
    void updateConveyor();

    /**
    * @brief run the conveyor based on how many disks are in the intake or if intake is allowed to be on
    *
    */
    void runConveyor();

    /**
    * @brief set the number of disks in the intake, usually on the initialization of the intake
    * @param initCount number of disks to set in the intake
    */
    void setDiskCount(int initCount);

    /**
    * @brief get the number of disks in the intake
    *
    */
    int getDiskCount();

    /**
    * @brief check if the intake will be getting >3 disks
    *
    */
    void checkOverload();

    /**
    * @brief reset the number of disks in the intake, used mostly for after shooting
    *
    */
    void resetConveyor();

    MotorGroup motors;
    int velocity;
    bool intakeOn = false; // made public to allow toggle control

    private:
    pros::task_t conveyorTask;
    pros::ADIAnalogIn lineTrackerSensor_;
    int countDisk = 0;
    bool passingDisk_ = false; // whether a disk is currently in the process of going through the intake
};

/**
 * @brief intake task to be running in the background
 * 
 * @param conveyor conveyor intake object
 */
void conveyorMain(void* conveyor);
} // namespace sparkyLib