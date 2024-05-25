#include "pros/vision.h"
#include "pros/vision.hpp"
#include <memory>

namespace sparkyLib {
    class Vision: pros::Vision { 
        public:
        /**
        * @brief constructor for the vision sensor, aka the optical sensor
        * @param visionPort
        *
        **/
        Vision(int visionPort);

        /**
        * @brief assign signature ids to the appropriate object's signature values
        * @param objectSig[] array of different signature settings, up to 7 objects
        *
        **/
        void setSignatures(pros::vision_signature_s_t objectSig[]);

        /**
        * @brief detect which object is visible to the camera
        * @param vision_object the vision sensor class object
        * @param object_arr[] array representation of an object
        * @return the signature id of the largest object detected **should be second largest instead?
        **/
        int readSignatureObject(sparkyLib::Vision vision_object, pros::vision_object_s_t object_arr[]);

        /**
        * @brief set new exposure value from 0-150 for the camera
        * @param brightLevel the new exposure value, 0-150
        *
        **/
        void setBrightness(int brightLevel);

        private:
        pros::vision_signature_s_t SIG_1;
        pros::vision_signature_s_t SIG_2;
        pros::vision_signature_s_t SIG_3;
        pros::vision_signature_s_t SIG_4;
        pros::vision_signature_s_t SIG_5;
        pros::vision_signature_s_t SIG_6;
        pros::vision_signature_s_t SIG_7;

        int sig; // signature id
    };
}