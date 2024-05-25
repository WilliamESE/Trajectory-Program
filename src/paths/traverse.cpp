#include "path.hpp"
#include "../ctrcvu/utilities/pose.hpp"
#include "../ctrcvu/utilities/mathFunctions.hpp"

Traveler::Traveler(std::vector<t_motiondata> *path, sparkyLib::PIDGains hpid, sparkyLib::PIDGains purepid, sparkyLib::PIDGains turnpid, double lookAHead, double range) : 
    m_path(path), m_pidHeading(hpid), m_pidPurePursuit(purepid), m_pidTurn(turnpid), log("pathData"){
    m_count = 0;
}

Traveler &Traveler::withChassis(std::shared_ptr<sparkyLib::Inline> chassis){
    m_chassis = chassis;
    return *this;
}

Traveler &Traveler::withIntake(std::shared_ptr<sparkyLib::ConveyorIntake> intake){
    m_intake = intake;
    m_withIntake = true;
    return *this;
}

Traveler &Traveler::withShooter(std::shared_ptr<sparkyLib::Shooter> shooter){
    m_shooter = shooter;
    m_withShooter = true;
    return *this;
}

Traveler &Traveler::withExpansion(std::shared_ptr<sparkyLib::Expansion> expand){
    m_withExpansion = true;
    m_expand = expand;
    return *this;
}

Traveler &Traveler::withRoller(std::shared_ptr<sparkyLib::Roller> roller){
    m_roller = roller;
    m_withRoller = true;
    return *this;
}
        
void Traveler::setPath(std::vector<t_motiondata> *path){
    m_path = path;
    m_count = 0;
}

void Traveler::saveLog(){
    log.close();
}

void Traveler::setHeadingCorrectionPID(sparkyLib::PIDGains pid){
    m_headingCorrection = true;
    m_pidHeading.setPID(pid);
}

void Traveler::setPurePursuitPID(double distance, sparkyLib::PIDGains pid){
    m_purePursuit = true;
    m_pidPurePursuit.setPID(pid);
}

bool Traveler::step(){
    str.str("");
    if(m_count >= m_path->size()){
        m_chassis->moveChassis(0,0);
        if(m_withShooter)
            m_shooter->stopShooter();
        if(m_withIntake)
            m_intake->moveVelocity(0);
        log.close();
        return true;
    }
    t_motiondata m = m_path->at(m_count);

    if(m_turning && m.flags.b_modes.turn){
        //turn to the next index angle
        m_pidTurn.resetPID();
        m_tidx = m_count + 1;
        m_turning = false;
    }

    if(!m_turning){
        m_turning = turnTo(((double)m_path->at(m_tidx).heading)/100.0);
        if(m_turning)
            m_count++;
        return false;
    }


    double lv = ((((double)m.lvel)/100.0) * 60.0 / (M_PI * (m_chassis->m_sizes.wheeldiameter)) / m_chassis->m_sizes.gearratio);
    double rv = ((((double)m.rvel)/100.0) * 60.0 / (M_PI * (m_chassis->m_sizes.wheeldiameter)) / m_chassis->m_sizes.gearratio);
    double x = ((double)m.xpos)/100.0;
    double y = ((double)m.ypos)/100.0;
    double h = ((double)m.heading)/100.0;
    //Are we accelerating?
    // bool accel = false;
    // bool deccel = false;
    // if(m_count != 0){
    //     if(!m.flags.b_modes.bw){
    //         if((abs(m.lvel) - abs(m_path->at(m_count-1).lvel)) > 20){
    //             accel = true;
    //         }
    //         if((abs(m.lvel) - abs(m_path->at(m_count-1).lvel)) < -20){
    //             deccel = true;
    //         }
    //     }
    //     else{
    //         if((abs(m.lvel) - abs(m_path->at(m_count-1).lvel)) < 20){
    //             accel = true;
    //         }
    //         if((abs(m.lvel) - abs(m_path->at(m_count-1).lvel)) > -20){
    //             deccel = true;
    //         }
    //     }
    // }
    // else
    //     accel = true;

    // if(accel){
    //     lv += 30;
    //     rv += 30;
    // }
    // if(deccel){
    //     lv -= 30;
    //     rv -= 30;
    // }
    sparkyLib::Pose path = sparkyLib::Pose({x,y,h});
    sparkyLib::Pose oPos = m_chassis->getRobotPosition();
    sparkyLib::Pose headPos = sparkyLib::Pose({x,y,h});
    int ahead = m_count;
    bool done = false;
    double ax, ay, ah, aDist, angle, signal, diff, radius, ang2;

    radius = oPos.distTo(path);
    if(radius > m_range){
        //Pure pursuit
        radius += m_distance; //distance to path + the lookahead distance
        //Loop through the path array to find the point at which the lookahead meets the path
        done = false;
        while(!done){
            ax = ((double)m_path->at(ahead).xpos)/100.0;
            ay = ((double)m_path->at(ahead).ypos)/100.0;
            ah = ((double)m_path->at(ahead).heading)/100.0;
            headPos = {ax,ay,ah};
            aDist = oPos.distTo(headPos);
            if(aDist > radius){
                done = true;
                break;
            }
            else
                done = false;
            std::cout << aDist << "," << radius << "," << ahead << "," << done << std::endl;
            if((m_path->at(ahead).lvel == 0) && (m_path->at(ahead).rvel == 0)){
                //If at the end of the path we need to artifically increase the path to avoid a sudden extreme turn in the event of slight over distance.
                ax = ax + radius * sin(degToRad(ah));
                ay = ay + radius * cos(degToRad(ah));
                headPos = {ax,ay,ah};
                break;
            }
            ahead++;
            if(ahead >= m_path->size()){
                ax = ax + radius * sin(degToRad(ah));
                ay = ay + radius * cos(degToRad(ah));
                headPos = {ax,ay,ah};
                ahead = m_path->size() - 1;
                done = true;
            }
        }
        
        angle = angleWrap180(oPos.angleTo(headPos) - oPos.theta);
        if(m.flags.b_modes.bw){
            if(angle < 0)
                angle += 180;
            else
                angle -= 180;
        }
        // signal = m_pidPurePursuit.calculatePID(angle / 90.0, 0, 1/90.0);
        // if(m.flags.b_modes.bw){
        //     lv -= signal * 100; //Modify the speed by max 50 RPM
        //     rv += signal * 100; //Modify the speed by max 50 RPM
        // }
        // else{
        //     lv -= signal * 100; //Modify the speed by max 50 RPM
        //     rv += signal * 100; //Modify the speed by max 50 RPM
        // }
        
        //Here I can check the angle to the path's point, if the angle is 180 the robot is a head
        //  If the angle is 0 the robot is behind.
        // ang2 = angleWrap180(oPos.angleTo(path));
        // if(fabs(ang2) < 10){
        //     lv += 50;
        //     rv += 50;
        // }
    }
    else {
        diff = oPos.theta - path.theta;
        signal = m_pidHeading.calculatePID(diff, 0, 0.5);
        // if(m.flags.b_modes.bw){
        //     lv -= signal * 15; //Modify the speed by max 50 RPM
        //     rv += signal * 15; //Modify the speed by max 50 RPM
        // }
        // else{
        //     lv += signal * 15; //Modify the speed by max 50 RPM
        //     rv -= signal * 15; //Modify the speed by max 50 RPM
        // }
    }

    if(m_withShooter){
        if(m.flags.b_modes.shoot){
            //Shoot the disks
            m_shoot1 = false;
        }
        if(!m_shoot1)
            m_shoot1 = m_shooter->shoot();
    }

    if(m_withExpansion){
        if(m.flags.b_modes.extend){
            m_expand->launch();
        }
    }

    if(m_withRoller) {
        if(m.flags.b_modes.roller){
            //move backward at a set speed + run rollers
            m_runRollers = true;
            m_delay = 3000;
            m_count++;
        }
        if (m_runRollers){
            m_delay--;
            m_chassis->moveChassis(-10, -10);
            m_roller->moveRoller(100);
            int rtn = m_roller->readVision();
            pros::lcd::print(4,"%i",rtn);
            if((((rtn != 1) && (rtn != 2)) || (rtn == m_allianceColour)) && m_delay != 0)
                m_runRollers = true;
            else {
                m_chassis->moveChassis(0,0);
                m_roller->stopRoller();
                m_runRollers = false;
            }
            return false;
        }
    }

    if(m_withIntake) {
        if(m.flags.b_modes.intake){
            //Run intake
            m_intake->moveVelocity(-600);
        }
        else{
            m_intake->moveVelocity(0);
        }
    }

    if(m_delay != 0){
        m_chassis->moveChassis(0,0);
        m_delay--;
        return false;
    }

    m_chassis->moveChassis(lv,rv);

    
    str << m_count << "," << x << "," << y << "," << h << "," << oPos.x << "," << oPos.y << "," << oPos.theta << "," << ax << "," << ay << "," << ahead << "," << signal << "," << lv << "," << rv << "," << oPos.distTo(path) << "," << ang2 << "," << m_chassis->getLeftVelocity() << "," << m_chassis->getRightVelocity(); 
    log.writeline(str.str());

    if(m.flags.b_modes.delay){
        m_delay = m.time / 20;
    }

    m_count++;
    return false;
}

bool Traveler::turnTo(double angle){
    sparkyLib::Pose oPos = m_chassis->getRobotPosition();
    double signal = m_pidTurn.calculatePID(oPos.theta, angle, 1);
    std::cout << oPos.theta << "," << angle << "," << signal << std::endl;
    m_chassis->moveChassis(signal * 600,-signal * 600);
    if(m_pidTurn.isComplete()){
        m_chassis->moveChassis(0,0);
        return true;
    }
    return false;
}