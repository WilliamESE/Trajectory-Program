#include "pose.hpp"

using namespace sparkyLib;

double Pose::distTo(Pose ipoint) {
    Pose diff = ipoint - *this;
    return sqrt((diff.x * diff.x) + (diff.y * diff.y));
}

double Pose::angleTo(Pose ipoint) {
    Pose diff = ipoint - *this;
    return radToDeg(std::atan2(diff.x, diff.y));
}

Pose Pose::operator+(Pose rhs) {
    return {x + rhs.x, y + rhs.y, theta + rhs.theta, time + rhs.time};
}
Pose Pose::operator-(Pose rhs) {
    return {x - rhs.x, y - rhs.y, theta - rhs.theta, time - rhs.time};
}
