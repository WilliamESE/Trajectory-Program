#include "devices/motorGroup.hpp"
#include "devices/visionSensor.hpp"

namespace sparkyLib { 
    class Roller {
        public:
        Roller(std::shared_ptr<MotorGroup> m_roller, std::shared_ptr<Vision> vision_sensor, bool red_alliance);

        void moveRoller(int ivel);
        void stopRoller();
        void setupVision();
        void getRollerVision();

        /**
         * @brief 
         * 
         * @return int returns the signature reading, 1 == blue, 2 == red
         */
        int readVision();

        private:
        std::shared_ptr<MotorGroup> m_roller;
        std::shared_ptr<Vision> vision_sensor;
        int sig;
        int velocity = 100;
        bool red_alliance = true;
    };
}