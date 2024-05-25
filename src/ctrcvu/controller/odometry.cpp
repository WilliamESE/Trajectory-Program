#include "odometry.hpp"

using namespace std;
using namespace sparkyLib;

void EncoderScales::operator=(EncoderScales rhs) {
	leftRadius = rhs.leftRadius;
	rightRadius = rhs.rightRadius;
	rearRadius = rhs.rearRadius;
	leftWheelDiameter = rhs.leftWheelDiameter;
	rightWheelDiameter = rhs.rightWheelDiameter;
	rearWheelDiameter = rhs.rearWheelDiameter;
	
	wheelTrack = leftRadius + rearRadius;
}


Odometry::Odometry(Encoders encoders, EncoderScales scales) : encoders(encoders), scales(scales) {
    encoders.left->set_position(0);
    encoders.right->set_position(0);

    logger = fopen("/usd/odomData", "w");
	if(logger != 0){
		sprintf(buf,"Wheeltrack,Left,Right,Middle\n");
		fputs(buf, logger);
		sprintf(buf,"%f,%f,%f,%f,%f,%f\n",scales.leftRadius,scales.rightRadius,scales.rearRadius,scales.leftWheelDiameter,scales.rightWheelDiameter,scales.rearWheelDiameter);
		fputs(buf, logger);
		sprintf(buf,"Rl, Rr, Rm, Global X, Global Y, Global Theta\n");
		fputs(buf, logger);
	}
}

void Odometry::updatePose(EncoderTicks ireadingsDiff) {
    double leftWheelCircumference = scales.leftWheelDiameter * M_PI;
    double rightWheelCircumference = scales.rightWheelDiameter * M_PI;
    double rearWheelCircumference = scales.rearWheelDiameter * M_PI;
    double chassisDiameter = scales.leftRadius + scales.rightRadius;

    EncoderTicks current = getEncoderTicks();
    
    // Delta distance
    OdomValues delta;
    delta.left = ((double) ireadingsDiff.left / scales.tpr) * leftWheelCircumference;
    delta.right = ((double) ireadingsDiff.right / scales.tpr) * rightWheelCircumference;

    // Delta theta
    //if(!inertialTrack)
        delta.theta = (delta.left - delta.right) / chassisDiameter; // (R-L) / W
    //else
    //    delta.theta = degToRad(angleWrap360(currentEncoder.angle));

    delta.rear = ((delta.rear / scales.tpr) * rearWheelCircumference) - (delta.theta * scales.rearRadius);

    // Local coordinates
    double localOffsetX = delta.rear;
    double localOffsetY = (delta.left + delta.right) / 2;
    if (delta.left != delta.right) {
        localOffsetX = 2 * sin(delta.theta / 2) * (delta.rear / delta.theta + scales.rearRadius);
        //if(delta.left > delta.right)
            localOffsetY = 2 * sin(delta.theta / 2) * (delta.right / delta.theta + scales.rightRadius);
        //else
        //    ly = 2 * sin(delta.angle / 2) * (delta.left / delta.angle + m_sizes.leftRadius);
    }

    // Polar coordinates
    double polarRadius = sqrt(pow(localOffsetX, 2) + pow(localOffsetY, 2));
    double polarAngle = atan2(localOffsetY, localOffsetX) - (degToRad(pose.theta) + delta.theta / 2);

    // Global coordinates
    double deltaGlobalX = cos(polarAngle) * polarRadius;
    double deltaGlobalY = sin(polarAngle) * polarRadius;

    if (isnan(deltaGlobalX)) deltaGlobalX = 0;
    if (isnan(deltaGlobalY)) deltaGlobalY = 0;
    if (isnan(delta.theta)) delta.theta = 0;

    double globalX = pose.x + deltaGlobalX;
    double globalY = pose.y + deltaGlobalY;
    double globalTheta = pose.theta + radToDeg(delta.theta);
	setPose({globalX, globalY, globalTheta, pros::millis()});

    if(logger){
        sprintf(buf,"%i,%i,%f,%f,%f,%f\n",encoders.left->get_velocity(),encoders.right->get_velocity(),current.rear,globalX,globalY,globalTheta);
        fputs(buf, logger);
    }
}

Pose Odometry::getPose() {
	return pose;
}

void Odometry::setPose(Pose newPose) {
	pose = newPose;
}

Encoders Odometry::getEncoders() {
	return encoders;
}

void Odometry::setEncoders(Encoders newEncoders) {
	encoders = newEncoders;
}

EncoderScales Odometry::getEncoderScales() {
	return scales;
}

void Odometry::setEncoderScales(EncoderScales newScales) {
	scales = newScales;
}

EncoderTicks Odometry::getEncoderTicks() {
	return {(double)encoders.left->get_position(), (double)encoders.right->get_position(), 0};
}

void Odometry::startOdom() {
	odomTask = pros::c::task_create(odometryMain, this, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Odometry");
}

void Odometry::endOdom() {
	pros::c::task_delete(odomTask);
}

EncoderTicks EncoderTicks::operator+(EncoderTicks rhs) {
    return {left + rhs.left, right + rhs.right, rear + rhs.rear};
}
EncoderTicks EncoderTicks::operator-(EncoderTicks rhs) {
    return {left - rhs.left, right - rhs.right, rear - rhs.rear};
}
EncoderTicks EncoderTicks::operator*(EncoderTicks rhs) {
	return {left * rhs.left, right * rhs.right, rear * rhs.rear};
}
EncoderTicks EncoderTicks::operator/(EncoderTicks rhs) {
	return {left / rhs.left, right / rhs.right, rear / rhs.rear};
}
EncoderTicks EncoderTicks::operator*(double factor) {
	return {left * factor, right * factor, rear * factor};
}
EncoderTicks EncoderTicks::operator/(double dividend) {
	return {left / dividend, right / dividend, rear / dividend};
}
EncoderTicks EncoderTicks::toDistance(EncoderScales scales, int ticksPerRotation) {
	double leftDistance = (left / ticksPerRotation) * scales.leftWheelDiameter * M_PI;
	double rightDistance = (right / ticksPerRotation) * scales.rightWheelDiameter * M_PI;
	double rearDistance = (rear / ticksPerRotation) * scales.rearWheelDiameter * M_PI;
	return {leftDistance, rightDistance, rearDistance};
}

void Odometry::startCalibration() {
	calibrating = true;
	calibrationStart = getEncoderTicks();
	cout << "Start calibration\n";
}

void Odometry::calibrateWheelDiameter(double actualDist, EncoderTicks ticksTravelled) {
	if (!calibrating)
		cerr << "Did not start calibration\n";

	EncoderTicks distTravelled = ticksTravelled.toDistance(scales, encoders.tpr);
	EncoderTicks ratios = {actualDist / distTravelled.left, actualDist / distTravelled.right, actualDist / distTravelled.rear};
	EncoderTicks newScales = {scales.leftWheelDiameter * ratios.left, scales.rightWheelDiameter * ratios.right, scales.rearWheelDiameter * ratios.rear};

	cout << "ticks:\t" << ticksTravelled.left << "\t" << ticksTravelled.right << "\t" << ticksTravelled.rear << "\n";
	cout << "dist:\t" << distTravelled.left << "\t" << distTravelled.right << "\t" << distTravelled.rear << "\n";
	cout << "ratios:\t" << ratios.left << "\t" << ratios.right << "\t" << ratios.rear << "\n";
	cout << "scales:\t" << scales.leftWheelDiameter << "\t" << scales.rightWheelDiameter << "\t" << scales.rearWheelDiameter << "\n";
	cout << "Left: " << newScales.left << " m\n";
	cout << "Right: " << newScales.right << " m\n";
	cout << "Rear: " << newScales.rear << " m\n";
	pros::lcd::print(5, "Left: %f m", newScales.left);
	pros::lcd::print(6, "Right: %f m", newScales.right);
	pros::lcd::print(7, "Rear: %f m", newScales.rear);
}
void Odometry::calibrateChassisDiameter(double actualAng, EncoderTicks ticksTravelled) {
	if (!calibrating)
		cerr << "Did not start calibration\n";

	EncoderTicks distTravelled = ticksTravelled.toDistance(scales, encoders.tpr);
	EncoderTicks angleTurned = {radToDeg(distTravelled.left / scales.leftRadius), radToDeg(distTravelled.right / scales.rightRadius), radToDeg(distTravelled.rear / scales.rearRadius)};
	EncoderTicks ratios = angleTurned / actualAng;
	EncoderScales newScales = {scales.leftRadius * ratios.left, scales.rightRadius * ratios.right, scales.rearRadius * ratios.rear};

	cout << "ticks:\t" << ticksTravelled.left << "\t" << ticksTravelled.right << "\t" << ticksTravelled.rear << "\n";
	cout << "dist:\t" << distTravelled.left << "\t" << distTravelled.right << "\t" << distTravelled.rear << "\n";
	cout << "angle:\t" << angleTurned.left << "\t" << angleTurned.right << "\t" << angleTurned.rear << "\n";
	cout << "ratios:\t" << ratios.left << "\t" << ratios.right << "\t" << ratios.rear << "\n";
	cout << "scales:\t" << scales.leftRadius << "\t" << scales.rightRadius << "\t" << scales.rearRadius << "\n";
	cout << "Left: " << newScales.leftRadius << " m\n";
	cout << "Right: " << newScales.rightRadius << " m\n";
	cout << "Rear: " << newScales.rearRadius << " m\n";
	pros::lcd::print(5, "Left: %f m", newScales.leftRadius);
	pros::lcd::print(6, "Right: %f m", newScales.rightRadius);
	pros::lcd::print(7, "Rear: %f m", newScales.rearRadius);
}

bool Odometry::isCalibrating() {
	return calibrating;
}

void Odometry::saveLog(){
    fclose(logger);
}

void sparkyLib::odometryMain(void* odometry) {
	Odometry* odom = (Odometry*) odometry;

	Encoders encoders = odom->getEncoders();
	EncoderScales scales = odom->getEncoderScales();

	EncoderTicks prev = {0, 0, 0};

	uint32_t time = pros::millis();
	while (true) {
		EncoderTicks current = odom->getEncoderTicks();
		EncoderTicks diff = current - prev;

		odom->updatePose(diff);

		pros::lcd::print(0, "x: %.2f in   y: %.2f in", odom->getPose().x, odom->getPose().y);
        pros::lcd::print(1, "theta: %.2f deg", odom->getPose().theta);

        pros::lcd::print(2, "left: %.2f    right: %.2f", odom->getEncoderTicks().left, odom->getEncoderTicks().right);

		prev = current;
		pros::Task::delay_until(&time, 10);
	}
}
