#include "motor.hpp"
#include "okapi/api/device/motor/abstractMotor.hpp"

using namespace std;
using namespace sparkyLib;

Motor::Motor(int8_t port) : okapi::Motor(port), m_vpid({1,0,0,0}), m_ppid({1,0,0,0}) {
	setBrakeMode(AbstractMotor::brakeMode::brake);
    m_targetPosition = 0;
    m_targetVelocity = 0;
    m_targetVoltage = 0;
    pidVoltOutput = 0;
    m_slewCmd = 0;
    m_state = mtrState::VOLTAGE;
}
Motor::Motor(int8_t port, bool reverse, okapi::AbstractMotor::gearset igearset, 
    okapi::AbstractMotor::encoderUnits encoderUnits)
			: okapi::Motor (port, reverse, igearset, encoderUnits), m_vpid({1,0,0,0}), m_ppid({1,0,0,0}) {
    setBrakeMode(AbstractMotor::brakeMode::brake);
    m_targetPosition = 0;
    m_targetVelocity = 0;
    m_targetVoltage = 0;
    pidVoltOutput = 0;
    m_slewCmd = 0;
    m_state = mtrState::VOLTAGE;
}

Motor &Motor::withRotationSensor(std::shared_ptr<pros::Rotation> sensor){
    m_rotation = true;
    m_enocder = sensor;
    return *this;
}
Motor &Motor::withRotationSensor(uint8_t sensor, bool reversed){
    m_rotation = true;
    pros::Rotation rot(sensor, reversed);
    m_enocder = std::make_shared<pros::Rotation>(rot);
    return *this;
}

Motor &Motor::withSlewRate(double rate){
    m_slewRate = rate;
    m_slewCmd = 0;
    return *this;
}

Motor &Motor::withLinearCompensation(double m, double b, double range){
    m_linear = true;
    m_coefficient_B = b;
    m_coefficient_M = m;
    m_linearRange = range;
    return *this;
}

void Motor::setVelocity(double velocity){
    m_state = 0;
    m_targetVelocity = velocity;
}
void Motor::setPosition(double position){
    m_state = 1;
    m_targetPosition = position;
}
void Motor::setVoltage(int volts){
    m_state = 2;
    m_targetVoltage = volts;
}

double Motor::getTarget_Velocity(){
    return m_targetVelocity;
}
double Motor::getTarget_Position(){
    return m_targetPosition;
}
int Motor::getTarget_Voltage(){
    return m_targetVoltage;
}

void Motor::setVelPID(PIDGains pid, double max, double min, double iComp){
    m_vpid.setPID(pid);
    m_vpid.setLimits(max,min);
    m_vpid.setiComp(iComp);
}
void Motor::setPosPID(PIDGains pid, double max, double min, double iComp){
    m_ppid.setPID(pid);
    m_ppid.setLimits(max,min);
    m_ppid.setiComp(iComp);
}
void Motor::setSlewRate(double rate){
    m_slewRate = rate;
    m_slewCmd = 0;
}

void Motor::calSlewRate(double target){
    //Calculate the next voltage level
    if(m_slewRate == 0){
        //No slewrate, just set the speed and return
        moveVoltage(target);
        return;
    }

    //Slew rate calulations
    int direction = sign(target - m_slewCmd);
    m_slewCmd += m_slewRate * direction;
    if((direction < 0) && ((target - m_slewCmd) > 0))
        m_slewCmd = target;
    else if((direction > 0) && ((target - m_slewCmd) < 0))
        m_slewCmd = target;

    //std::cout << "," << m_slewCmd << "," << target << "," << direction;
    //Set voltage
    moveVoltage(m_slewCmd);
}

int Motor::step(){
    // return 1;
    //Calculate the new voltage level to set it to
    //Call the calSlewRate -> which then sets the new speed
    switch(m_state){
        case VELOCITY: {
            //If linear is enabled, calculate the voltage
            double volts;
            if(m_linear){
                int direction = sign(m_targetVelocity);
                volts = m_coefficient_M * (m_targetVelocity) + m_coefficient_B;
                //Wait until we are in range before applying the pid
                if(fabs(m_targetVelocity - getActualVelocity()) > m_linearRange){
                    calSlewRate(volts);
                    break; //Exit switch statement
                }
            }
            //PID
            double out = m_vpid.calculatePID(getActualVelocity(), m_targetVelocity, 0.1);
            //call sew rate function
            if(m_linear)
                calSlewRate(volts + (out * 2000));
            else{
                pidVoltOutput += (out * 6000);
                //std::cout << id << "," << pidVoltOutput << "," << out << "," << getActualVelocity() << "," << m_targetVelocity << "," << getVoltage() << "," << getCurrentDraw();      
                calSlewRate(pidVoltOutput);
                if(id == 3)
                    std::cout << std::endl;
                else
                    std::cout << ",";
            }
            break;
        }
        case POSITION:{
            double out = m_ppid.calculatePID(getEncoderValue(), m_targetPosition, 50);
            calSlewRate(out * 12000);
            break;
            }
        case VOLTAGE:
        default:
            calSlewRate(m_targetVoltage);
            break;
    }
    return 1;
}
int Motor::calibrate(){
    //Set the speed to zero
	moveVoltage(0);

	double vel[12];
	double avgP = 0, avgV = 0;
	double SSxy = 0, SSxx = 0;
	int volt,i;

	for(volt = 0,i=0; i < 12; volt+=1000, i++){
		moveVoltage(volt);
        std::cout << volt << std::endl;
		pros::delay(1000); //wait for it to get up to a constant speed

        //Collect data
        long sum = 0;
        for(int j=0;j<100;j++)
		    sum += getActualVelocity();
        
        //Calculate average
        vel[i] = ((double)sum) / 100.0;
		avgP += volt;
		avgV += vel[i];

        moveVoltage(0);
        pros::delay(10000); //Wait 10 seconds before cont...
	}

	//I need to calculate the avgX, avgY
	avgP /= 12;
	avgV /= 12;

	//SSxy = SUM((X_i - avgX)*(Y_i - angY))
	//SSxx = SUM(X_i - avgX)^2
	for(i=0;i<12;i++){
		SSxy += (vel[i] - avgV)*((i*1000) - avgP);
		SSxx += (vel[i] - avgV)*(vel[i] - avgV);
	}

	double B1_Coefficient = SSxy / SSxx;
	double B0_Coefficient = avgP - B1_Coefficient * avgV;

	std::cout << id << " Coefficient B1 = " << B1_Coefficient << std::endl;
	std::cout << id << " Coefficient B0 = " << B0_Coefficient << std::endl;

    m_coefficient_M = B1_Coefficient;
    m_coefficient_B = B0_Coefficient;

    return 1;
}

double Motor::getEncoderValue(){
    if(m_rotation)
        return m_enocder->get_position();
    return getEncoder()->get();
}

void Motor::resetEncoder(){
    if(m_rotation)
        m_enocder->set_position(0);
    else
        getEncoder()->reset();
}


void sparkyLib::testMotor(uint8_t port){
    //Create motor -- should have no load
    Motor mtr = Motor(port).withSlewRate(100).withLinearCompensation(48.3088, 194.082, 20);

    //Calibrate it -- this will update the coefficents
    //They should be around 48 and 194
    mtr.calibrate();

    //With a range of 20 above for the linear compensation
    //  the max 1 should be at the limit of that range
    //  1/20 = 0.05
    //This pid is not tuned, no point in doing so for a motor with no load.
    mtr.setVelPID({0.05,0,0,0},1,-1,10);

    //Set the desired velocity
    //should see it climb up to 100rpm because of the slewrate being 100
    mtr.setVelocity(100);

    //Run a loop for 6 (300/50) seconds
    for(int cnt=0;cnt<300;cnt++){
        //Run the step function for the motor to update to the next speed
        mtr.step();
        //Print out the speed when graphed should be a steady climb to 100 rpm and holds that velocity with s standard deviation of around 3-5 rpm.
        std::cout << "vel= " << mtr.getActualVelocity() << std::endl;
        //A delay of 20ms creates a loop of 50hz, every 50 interations = 1 second
        pros::delay(20);
    }
}