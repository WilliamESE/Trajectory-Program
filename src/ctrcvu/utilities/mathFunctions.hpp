#pragma once

#include <cmath>

/**
 * @brief Converts a degree to a radian
 * 
 * @param deg The degree
 * @return The radian equivalent of the degree
 */
double degToRad(double deg);
/**
 * @brief Converts a radian to a degree
 * 
 * @param rad The radian
 * @return The degree equivalent of the radian
 */
double radToDeg(double rad);

/**
 * @brief Converts from inches to centimeters
 * 
 * @param in The measurement in inches
 * @return The measurement in centimeters
 */
double inToCm(double in);
/**
 * @brief Converts from centimeters to inches
 * 
 * @param in The measurement in centimeters
 * @return The measurement in inches
 */
double cmToIn(double cm);

/**
 * @brief Converts from feet to centimeters
 * 
 * @param in The measurement in feet
 * @return The measurement in centimeters
 */
double ftToCm(double ft);
/**
 * @brief Converts from centimeters to feet
 * 
 * @param in The measurement in centimeters
 * @return The measurement in feet
 */
double cmToFt(double cm);

/**
 * @brief Converts from inches to meters
 * 
 * @param in The measurement in inches
 * @return The measurement in meters
 */
double inToM(double in);
/**
 * @brief Converts from centimeters to meters
 * 
 * @param in The measurement in centimeters
 * @return The measurement in meters
 */
double mToIn(double m);

/**
 * @brief Converts from feet to meters
 * 
 * @param in The measurement in feet
 * @return The measurement in meters
 */
double ftToM(double ft);
/**
 * @brief Converts from meters to inches
 * 
 * @param in The measurement in meters
 * @return The measurement in inches
 */
double mToFt(double m);

double angleWrap180(double ang);
double angleWrap360(double ang);

/**
 * @brief Returns the sign of the given number
 * 
 * @param n The number
 * @return 1 for positive, -1 for negative, 0 if n is 0
 */
int sign(double n);
