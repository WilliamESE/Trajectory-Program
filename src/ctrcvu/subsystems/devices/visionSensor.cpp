#include "visionSensor.hpp"
#include "pros/rtos.hpp"
#include "pros/vision.h"
#include "pros/vision.hpp"
#include <iostream>

namespace sparkyLib { 
Vision::Vision(int visionPort): pros::Vision(visionPort) {}

void Vision::setSignatures(pros::vision_signature_s_t objectSig[]) {
    // red will usually go first then blue
    // so red == 1, blue == 2
    for (int i = 1; i <= 7; i++) {
		set_signature(i, &objectSig[i-1]);
	}
}

int Vision::readSignatureObject(sparkyLib::Vision vision_object, pros::vision_object_s_t object_arr[]) {
    vision_object.read_by_sig(0, 1, 4, object_arr);
	sig = object_arr[0].signature;
    std::cout << "sig: " << sig << std::endl;
    return sig;
}

void Vision::setBrightness(int brightLevel) {
    set_exposure(brightLevel);
}
}