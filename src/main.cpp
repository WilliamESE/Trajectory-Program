#include "main.h"
using namespace sparkyLib;
Robot robot;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	robot = Robot();
	robot.init();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	robot.autoInit();
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	while(true){
		robot.autonomous();
		pros::delay(20);
	}
}

void traversePath(){
	//Take path and begin traversing it.
	//This isn't difficult, just set the left and right velocities
	// FILE *logger = fopen("/usd/drivelog", "w");
	// char buf[256];
	// int idx = 0;
	// int ahead = 0, state = 0;
	// bool found = false;
	// double aDist = 0, angle = 0;
	// double lookahead = 7; //inches
	// double ax=0,ay=0,ah=0;
	// double signal = 0;
	// PIDController pos_pid = PIDController({0.9,0,0,0});
	// PIDController ang_pid = PIDController({0.2,0.00001,0,0});
	/*
	Lookahead must calculate a point along the curve to aim towards
	Generally this is done using a circle which intercets with the path at two points
	Then aim towards the one further down the path	
	The distance and angle would increase as the robot gets further away.
	Plus by looking ahead you avoid the issue of extreme sharp turns to 90 degrees

	One folly is the end of the curve when it can no longer look a head and a sharp turn
	would leave the robot pointing in the wrong direction

	First problem finding a point along the path. I do not have the path equations only
		individual points along it. I can start at the point the robot is suppose to be at
		and go along until I reach the nearest point at a certain distance
		that would most likely be my intersection. I cannot go through the whole path for two reasons
		(1) the processing time, and (2) overlapping paths
		If the last point is reached used that one

	Second calculate the presice distance and angle to the lookahead

	Third using that angle modify the speeds of the left and right velocities. If the angle is
		negative it must turn to the left, if positive turn to the right. A PID could be used here
		But what is the target? For the lookahead to be straight ahead. The angle would become 0
		and the robot should move straight forward.

	Forth recalculation of the drive wheels. We need the robot to turn towards the lookahead point
		The curvature technique would be tricky as we need the directions
	*/

	// for(auto m : g_Test){
	// 	ahead = idx;
	// 	double lv = (((double)m.lvel)/100.0) * 60.0 / (M_PI * (LEFT_D));
	// 	double rv = (((double)m.rvel)/100.0) * 60.0 / (M_PI * (LEFT_D));
	// 	double x = ((double)m.xpos)/100.0;
	// 	double y = ((double)m.ypos)/100.0;
	// 	double h = ((double)m.heading)/100.0;
	// 	Pose path = Pose({x,y,h});

	// 	//Find lookahead point
	// 	found = false;
	// 	bool done = false;
	// 	Pose pos = odomptr->getPose();
	// 	Pose aheadpnt = Pose();
	// 	signal = 0;

	// 	if(pos.distTo(path) > 2){
	// 		while(!done){
	// 			ax = ((double)g_Test[ahead].xpos)/100.0;
	// 			ay = ((double)g_Test[ahead].ypos)/100.0;
	// 			ah = ((double)g_Test[ahead].heading)/100.0;
	// 			aheadpnt = {ax,ay,ah};
	// 			aDist = pos.distTo(aheadpnt);
	// 			if(aDist > lookahead)
	// 				done = true;
	// 			ahead++;
	// 			if(ahead == g_Test.size())
	// 				done = true;
	// 		}

	// 		//Modify the left and right speeds			
	// 		//Calculate the distance and angle
	// 		//	Distance is already calculated in aDist
	// 		angle = angleWrap180(pos.angleTo(aheadpnt) - pos.theta);
	// 		signal = pos_pid.calculatePID(angle / 90.0, 0, 1/90.0);
	// 		lv -= signal * 50; //Modify the speed by max 50 RPM
	// 		rv += signal * 50; //Modify the speed by max 50 RPM
	// 		state = 1;
	// 	}
	// 	else {
	// 		//Here we can do a heading correction
	// 		// 	Current heading is pos.theta; desired heading is actual.theta
	// 		//	They need to be the same thing
	// 		double diff = pos.theta - path.theta;
	// 		signal = ang_pid.calculatePID(diff, 0, 0.5);
	// 		lv += signal * 10; //Modify the speed by max 50 RPM
	// 		rv -= signal * 10; //Modify the speed by max 50 RPM
	// 		state = 2;
	// 	}

	// 	//How far away are we from where we are suppose to be?
	// 	// double distance = pos.distTo({x,y,h});
	// 	// double angle = pos.angleTo({x,y,h});
	// 	//angle approach 0 as we get to the desired position?

	// 	// fl.moveVelocity(lv);
	// 	// fr.moveVelocity(rv);
	// 	// rl.moveVelocity(lv);
	// 	// rr.moveVelocity(rv);

	// 	if(m.flags.b_modes.shoot){
	// 		//Shoot the disks
	// 		//Might have to accound for how many disks it has in case one was lost
	// 	}

	// 	if(m.flags.b_modes.extend){
	// 		//Fire the extension
	// 	}


	// 	//Logger
	// 	//sprintf(buf,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",((double)m.time)/100.0,aDist,angle,x,y,h,ax,ay,ah,pos.x,pos.y,pos.theta,signal,lv,rv);
	// 	sprintf(buf,"%f,%i,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%i\n",((double)m.time)/100.0,ahead,aDist,angle,x,y,h,pos.x,pos.y,pos.theta,signal,lv,rv,state);
	// 	fputs(buf, logger);

	// 	//Special exit function in the event that something has gone wrong
	// 	if(masterC.getDigital(okapi::ControllerDigital::B)){
	// 		break;
	// 	}

	// 	idx++;
	// 	pros::delay(20);
	// }
	// // fl.moveVelocity(0);
	// // fr.moveVelocity(0);
	// // rl.moveVelocity(0);
	// // rr.moveVelocity(0);
	// fclose(logger);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	robot.driverInit();
	while(true){
		robot.driverControl();
		pros::delay(20);
	}
}


	/* bool newBtn = false;

	if ((!newBtn) && intakeC.getDigital(okapi::ControllerDigital::down)) {
		newBtn = true;
		if (driveCubic)
			driveCubic = false;
		else
			driveCubic = true;
	}

	if ((!newBtn) && intakeC.getDigital(okapi::ControllerDigital::up)) {
		newBtn = true;
		if (driveCubic)
			driveCubic = false;
		else
			driveCubic = true;
	}

	pros::Controller controller;

// Declare a motor object
pros::Motor intakeMotor(1);

void opcontrol() {
  // Main loop
  while (true) {
    // Check the state of the L1 and L2 buttons
    bool l1Pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    bool l2Pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2);

    // Control the intake motor
    if (l1Pressed) {
      intakeMotor.move_velocity(127);
    } else if (l2Pressed) {
      intakeMotor.move_velocity(-127);
    } else {
      intakeMotor.move_velocity(0);
    }

    // Wait for the next iteration
    pros::delay(20);
  }
}



 */


	/* Motor mtr1 = Motor(10).withSlewRate(100);
	MotorGroup grp1({std::make_shared<Motor>(mtr1)});
	addMotorGroup(grp1);
	startMotorController();
	pros::delay(1000);
	grp1.setVoltage(3000);
	pros::delay(1000);
	grp1.setVoltage(6000);
	pros::delay(1000);
	grp1.setVoltage(0);

		
	while (true) {
		//pros::Motor mtr1 = pros::Motor(10);
		//sparkyLib::addMotor(std::make_shared<sparkyLib::Motor>(mtr1));
		//sparkyLib::startMotorController();
		pros::delay(20);
	} 
	}*/
