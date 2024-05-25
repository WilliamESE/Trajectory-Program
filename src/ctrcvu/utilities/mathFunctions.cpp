#include "mathFunctions.hpp"

double degToRad(double deg) {
    return deg * (M_PI / 180);
}
double radToDeg(double rad) {
    return rad * (180 / M_PI);
}

double inToCm(double in) {
    return in * 2.54;
}
double cmToIn(double cm) {
    return cm / 2.54;
}

double ftToCm(double ft) {
    return ft * 30.48;
}
double cmToFt(double cm) {
    return cm / 30.48;
}

double inToM(double in) {
    return in / 39.37;
}
double mToIn(double m) {
    return m * 39.37;
}

double ftToM(double ft) {
    return ft / 3.281;
}
double mToFt(double m) {
    return m * 3.281;
}

double angleWrap180(double ang) {
	ang = fmod(ang + 180, 360);
    if (ang < 0)
        ang += 360;
    return ang - 180;
}
double angleWrap360(double ang) {
	ang = fmod(ang, 360);
    if (ang < 0)
        ang += 360;
    return ang;
}

int sign(double n) {
    return (n > 0) - (n < 0);
}
