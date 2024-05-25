#pragma once

#include "api.h"
#include "mathFunctions.hpp"

namespace sparkyLib {
/**
* @brief The coordinate position of the robot in inches and degrees
*
* Includes a timestamp of the position
*/
struct Pose {
	public:
	double x;
	double y;
	double theta;
	uint time;

	/**
	* @brief Finds the distance between 2 points
	* 
	* @param ipoint The second point from the current point
	* @return double 
	*/
	double distTo(Pose ipoint);
	/**
	* @brief Finds the angle between the 2 points
	* 
	* @param ipoint The second point that the first point will face towards
	* @return double 
	*/
	double angleTo(Pose ipoint);

	/**
	 * @brief Operator overloading for +
	 * 
	 * @param rhs The other value on the right hand side
	 * @return The new pose with the sum of the values in the 2 poses
	 */
	Pose operator+(Pose rhs);
	/**
	 * @brief Operator overloading for -
	 * 
	 * @param rhs The other value on the right hand side
	 * @return The new pose with the difference of the values in the 2 poses
	 */
	Pose operator-(Pose rhs);
};
} // namespace ctrLib
