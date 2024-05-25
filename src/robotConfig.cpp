#include "robotConfig.hpp"

using namespace sparkyLib;

/* Setting Array:
    int id;
    SET_TYPE type;
    std::string name;   //Easy identification of the setting
    int value;          //The current value of the setting, ints may reference other data like strings
    int previous;       //The previous value
    int factory;        //The default value if reset
    int decimals;       //Number of decimal places of the value if needed
    int min;            //For modifying the value this is the minimum it can be
    int max;            //For modifying the value this is the maximum it can be
    std::string details;//Description of what the setting does
*/

int loadDefaultSettings(std::shared_ptr<sparkyLib::Configuration> settings){
    // settings->addString("Sparky");
    // settings->addString("Razzle 1");
    // settings->addString("Razzle 2");

    // settings->addSetting({0,Configuration::STRING,"Name",0,0,0,0,0,0,"Name of the robot"});
    // settings->addSetting({1,Configuration::INTEGER,"Inline_Mtrs",4,4,4,0,0,10,"Number of motors for drive"});
    // //settings->addSetting({44,Configuration::INTEGER,"Inline_Gearset",MTR_GEARSET,MTR_GEARSET,MTR_GEARSET,0,0,2,"Drive gear box"});
    // settings->addSetting({45,Configuration::INTEGER,"Inline_GearRatio",(int)(MTR_GEAR_RATIO * 1000),(int)(MTR_GEAR_RATIO * 1000),(int)(MTR_GEAR_RATIO * 1000),3,0,100000,"Drive gear ratio"});

    // settings->addSetting({2,Configuration::INTEGER,"Inline_MtrPort_1",MTR_LEFT_PORT_A,MTR_LEFT_PORT_A,MTR_LEFT_PORT_A,0,0,21,"Motor Port"});
    // settings->addSetting({3,Configuration::INTEGER,"Inline_MtrPort_2",MTR_LEFT_PORT_B,MTR_LEFT_PORT_B,MTR_LEFT_PORT_B,0,0,21,"Motor Port"});
    // settings->addSetting({4,Configuration::INTEGER,"Inline_MtrPort_3",MTR_LEFT_PORT_C,MTR_LEFT_PORT_C,MTR_LEFT_PORT_C,0,0,21,"Motor Port"});
    // settings->addSetting({5,Configuration::INTEGER,"Inline_MtrPort_4",MTR_LEFT_PORT_D,MTR_LEFT_PORT_D,MTR_LEFT_PORT_D,0,0,21,"Motor Port"});
    // settings->addSetting({6,Configuration::INTEGER,"Inline_MtrPort_5",MTR_LEFT_PORT_E,MTR_LEFT_PORT_E,MTR_LEFT_PORT_E,0,0,21,"Motor Port"});
    // settings->addSetting({7,Configuration::INTEGER,"Inline_MtrPort_6",MTR_RIGHT_PORT_A,MTR_RIGHT_PORT_A,MTR_RIGHT_PORT_A,0,0,21,"Motor Port"});
    // settings->addSetting({8,Configuration::INTEGER,"Inline_MtrPort_7",MTR_RIGHT_PORT_B,MTR_RIGHT_PORT_B,MTR_RIGHT_PORT_B,0,0,21,"Motor Port"});
    // settings->addSetting({9,Configuration::INTEGER,"Inline_MtrPort_8",MTR_RIGHT_PORT_C,MTR_RIGHT_PORT_C,MTR_RIGHT_PORT_C,0,0,21,"Motor Port"});
    // settings->addSetting({10,Configuration::INTEGER,"Inline_MtrPort_9",MTR_RIGHT_PORT_D,MTR_RIGHT_PORT_D,MTR_RIGHT_PORT_D,0,0,21,"Motor Port"});
    // settings->addSetting({11,Configuration::INTEGER,"Inline_MtrPort_10",MTR_RIGHT_PORT_E,MTR_RIGHT_PORT_E,MTR_RIGHT_PORT_E,0,0,21,"Motor Port"});

    // settings->addSetting({12,Configuration::INTEGER,"Inline_MtrRev_1",MTR_LEFT_REVERSED_A,MTR_LEFT_REVERSED_A,MTR_LEFT_REVERSED_A,0,0,1,"Motor is reversed"});
    // settings->addSetting({13,Configuration::INTEGER,"Inline_MtrRev_2",MTR_LEFT_REVERSED_B,MTR_LEFT_REVERSED_B,MTR_LEFT_REVERSED_B,0,0,1,"Motor is reversed"});
    // settings->addSetting({14,Configuration::INTEGER,"Inline_MtrRev_3",MTR_LEFT_REVERSED_C,MTR_LEFT_REVERSED_C,MTR_LEFT_REVERSED_C,0,0,1,"Motor is reversed"});
    // settings->addSetting({15,Configuration::INTEGER,"Inline_MtrRev_4",MTR_LEFT_REVERSED_D,MTR_LEFT_REVERSED_D,MTR_LEFT_REVERSED_D,0,0,1,"Motor is reversed"});
    // settings->addSetting({16,Configuration::INTEGER,"Inline_MtrRev_5",MTR_LEFT_REVERSED_E,MTR_LEFT_REVERSED_E,MTR_LEFT_REVERSED_E,0,0,1,"Motor is reversed"});
    // settings->addSetting({17,Configuration::INTEGER,"Inline_MtrRev_6",MTR_RIGHT_REVERSED_A,MTR_RIGHT_REVERSED_A,MTR_RIGHT_REVERSED_A,0,0,1,"Motor is reversed"});
    // settings->addSetting({18,Configuration::INTEGER,"Inline_MtrRev_7",MTR_RIGHT_REVERSED_B,MTR_RIGHT_REVERSED_B,MTR_RIGHT_REVERSED_B,0,0,1,"Motor is reversed"});
    // settings->addSetting({19,Configuration::INTEGER,"Inline_MtrRev_8",MTR_RIGHT_REVERSED_C,MTR_RIGHT_REVERSED_C,MTR_RIGHT_REVERSED_C,0,0,1,"Motor is reversed"});
    // settings->addSetting({20,Configuration::INTEGER,"Inline_MtrRev_9",MTR_RIGHT_REVERSED_D,MTR_RIGHT_REVERSED_D,MTR_RIGHT_REVERSED_D,0,0,1,"Motor is reversed"});
    // settings->addSetting({21,Configuration::INTEGER,"Inline_MtrRev_10",MTR_RIGHT_REVERSED_E,MTR_RIGHT_REVERSED_E,MTR_RIGHT_REVERSED_E,0,0,1,"Motor is reversed"});

    // settings->addSetting({22,Configuration::INTEGER,"Inline_MtrSide_1",MTR_LEFT_SIDE_A,MTR_LEFT_SIDE_A,MTR_LEFT_SIDE_A,0,0,1,"Motor is on the left side"});
    // settings->addSetting({23,Configuration::INTEGER,"Inline_MtrSide_2",MTR_LEFT_SIDE_B,MTR_LEFT_SIDE_B,MTR_LEFT_SIDE_B,0,0,1,"Motor is on the left side"});
    // settings->addSetting({24,Configuration::INTEGER,"Inline_MtrSide_3",MTR_LEFT_SIDE_C,MTR_LEFT_SIDE_C,MTR_LEFT_SIDE_C,0,0,1,"Motor is on the left side"});
    // settings->addSetting({25,Configuration::INTEGER,"Inline_MtrSide_4",MTR_LEFT_SIDE_D,MTR_LEFT_SIDE_D,MTR_LEFT_SIDE_D,0,0,1,"Motor is on the left side"});
    // settings->addSetting({26,Configuration::INTEGER,"Inline_MtrSide_5",MTR_LEFT_SIDE_E,MTR_LEFT_SIDE_E,MTR_LEFT_SIDE_E,0,0,1,"Motor is on the left side"});
    // settings->addSetting({27,Configuration::INTEGER,"Inline_MtrSide_6",MTR_RIGHT_SIDE_A,MTR_RIGHT_SIDE_A,MTR_RIGHT_SIDE_A,0,0,1,"Motor is on the left side"});
    // settings->addSetting({28,Configuration::INTEGER,"Inline_MtrSide_7",MTR_RIGHT_SIDE_B,MTR_RIGHT_SIDE_B,MTR_RIGHT_SIDE_B,0,0,1,"Motor is on the left side"});
    // settings->addSetting({29,Configuration::INTEGER,"Inline_MtrSide_8",MTR_RIGHT_SIDE_C,MTR_RIGHT_SIDE_C,MTR_RIGHT_SIDE_C,0,0,1,"Motor is on the left side"});
    // settings->addSetting({30,Configuration::INTEGER,"Inline_MtrSide_9",MTR_RIGHT_SIDE_D,MTR_RIGHT_SIDE_D,MTR_RIGHT_SIDE_D,0,0,1,"Motor is on the left side"});
    // settings->addSetting({31,Configuration::INTEGER,"Inline_MtrSide_10",MTR_RIGHT_SIDE_E,MTR_RIGHT_SIDE_E,MTR_RIGHT_SIDE_E,0,0,1,"Motor is on the left side"});

    // settings->addSetting({32,Configuration::DOUBLE,"Inline_Left_P",(int)(MTR_LEFT_KP * 100000),(int)(MTR_RIGHT_KP * 100000),(int)(MTR_RIGHT_KP * 100000),5,-1,1,"Drive PID kp value"});
    // settings->addSetting({33,Configuration::DOUBLE,"Inline_Left_I",(int)(MTR_LEFT_KI * 100000),(int)(MTR_RIGHT_KI * 100000),(int)(MTR_RIGHT_KI * 100000),5,-1,1,"Drive PID ki value"});
    // settings->addSetting({34,Configuration::DOUBLE,"Inline_Left_D",(int)(MTR_LEFT_KD * 100000),(int)(MTR_RIGHT_KD * 100000),(int)(MTR_RIGHT_KD * 100000),5,-1,1,"Drive PID kd value"});
    // settings->addSetting({35,Configuration::DOUBLE,"Inline_Left_F",(int)(MTR_LEFT_KF * 100000),(int)(MTR_RIGHT_KF * 100000),(int)(MTR_RIGHT_KF * 100000),5,-1,1,"Drive PID kf value"});

    // settings->addSetting({36,Configuration::DOUBLE,"Inline_Right_P",(int)(MTR_RIGHT_KP * 100000),(int)(MTR_RIGHT_KP * 100000),(int)(MTR_RIGHT_KP * 100000),5,-1,1,"Drive PID kp value"});
    // settings->addSetting({37,Configuration::DOUBLE,"Inline_Right_I",(int)(MTR_RIGHT_KI * 100000),(int)(MTR_RIGHT_KI * 100000),(int)(MTR_RIGHT_KI * 100000),5,-1,1,"Drive PID ki value"});
    // settings->addSetting({38,Configuration::DOUBLE,"Inline_Right_D",(int)(MTR_RIGHT_KD * 100000),(int)(MTR_RIGHT_KD * 100000),(int)(MTR_RIGHT_KD * 100000),5,-1,1,"Drive PID kd value"});
    // settings->addSetting({39,Configuration::DOUBLE,"Inline_Right_F",(int)(MTR_RIGHT_KF * 100000),(int)(MTR_RIGHT_KF * 100000),(int)(MTR_RIGHT_KF * 100000),5,-1,1,"Drive PID kf value"});

    // settings->addSetting({40,Configuration::DOUBLE,"Inline_Slew",MTR_SLEW,MTR_SLEW,MTR_SLEW,0,0,12000,"Drive Left Slewrate"});
    
    // settings->addSetting({42,Configuration::INTEGER,"Inline_Left_Enocder",DRIVE_ROT_LEFT,DRIVE_ROT_LEFT,DRIVE_ROT_LEFT,0,0,21,"Drive Rotational Sensor"});
    // settings->addSetting({43,Configuration::INTEGER,"Inline_Right_Enocder",DRIVE_ROT_RIGHT,DRIVE_ROT_RIGHT,DRIVE_ROT_RIGHT,0,0,21,"Drive Rotational Sensor"});

    // //Odometry Settings
    // settings->addSetting({100,Configuration::INTEGER,"Odom_RotPort_Left",ODOM_ROT_PORT_LEFT,ODOM_ROT_PORT_LEFT,ODOM_ROT_PORT_LEFT,0,0,21,"Odometry Rot Port"});
    // settings->addSetting({101,Configuration::INTEGER,"Odom_RotPort_Right",ODOM_ROT_PORT_RIGHT,ODOM_ROT_PORT_RIGHT,ODOM_ROT_PORT_RIGHT,0,0,21,"Odometry Rot Port"});
    // settings->addSetting({102,Configuration::INTEGER,"Odom_RotPort_Middle",ODOM_ROT_PORT_MIDDLE,ODOM_ROT_PORT_MIDDLE,ODOM_ROT_PORT_MIDDLE,0,0,21,"Odometry Rot Port"});

    // settings->addSetting({103,Configuration::INTEGER,"Odom_RotRev_Left",ODOM_ROT_REVERSED_LEFT,ODOM_ROT_REVERSED_LEFT,ODOM_ROT_REVERSED_LEFT,0,0,1,"Odometry Rot Reversed"});
    // settings->addSetting({104,Configuration::INTEGER,"Odom_RotRev_Right",ODOM_ROT_REVERSED_RIGHT,ODOM_ROT_REVERSED_RIGHT,ODOM_ROT_REVERSED_RIGHT,0,0,1,"Odometry Rot Reversed"});
    // settings->addSetting({105,Configuration::INTEGER,"Odom_RotRev_Middle",ODOM_ROT_REVERSED_MIDDLE,ODOM_ROT_REVERSED_MIDDLE,ODOM_ROT_REVERSED_MIDDLE,0,0,1,"Odometry Rot Reversed"});

    // settings->addSetting({106,Configuration::DOUBLE,"Odom_Scale_LeftRadius",(int)(ODOM_SCALE_LEFTRADIUS * 100000),(int)(ODOM_SCALE_LEFTRADIUS * 100000),(int)(ODOM_SCALE_LEFTRADIUS * 100000),5,0,100,"Odometry Scale Wheel Radius"});
    // settings->addSetting({107,Configuration::DOUBLE,"Odom_Scale_RightRadius",(int)(ODOM_SCALE_RIGHTRADIUS * 100000),(int)(ODOM_SCALE_RIGHTRADIUS * 100000),(int)(ODOM_SCALE_RIGHTRADIUS * 100000),5,0,100,"Odometry Scale Wheel Radius"});
    // settings->addSetting({108,Configuration::DOUBLE,"Odom_Scale_MiddleRadius",(int)(ODOM_SCALE_MIDDLERADIUS * 100000),(int)(ODOM_SCALE_MIDDLERADIUS * 100000),(int)(ODOM_SCALE_MIDDLERADIUS * 100000),5,0,100,"Odometry Scale Wheel Radius"});

    // settings->addSetting({109,Configuration::DOUBLE,"Odom_Scale_LeftDia",(int)(ODOM_SCALE_LEFTDIAMETER * 100000),(int)(ODOM_SCALE_LEFTDIAMETER * 100000),(int)(ODOM_SCALE_LEFTDIAMETER * 100000),5,0,100,"Odometry Scale Wheelbase"});
    // settings->addSetting({110,Configuration::DOUBLE,"Odom_Scale_RightDia",(int)(ODOM_SCALE_RIGHTDIAMETER * 100000),(int)(ODOM_SCALE_RIGHTDIAMETER * 100000),(int)(ODOM_SCALE_RIGHTDIAMETER * 100000),5,0,100,"Odometry Scale Wheelbase"});
    // settings->addSetting({111,Configuration::DOUBLE,"Odom_Scale_MiddleDia",(int)(ODOM_SCALE_MIDDLEDIAMETER * 100000),(int)(ODOM_SCALE_MIDDLEDIAMETER * 100000),(int)(ODOM_SCALE_MIDDLEDIAMETER * 100000),5,0,100,"Odometry Scale Wheelbase"});

    // //Shooter
    // settings->addSetting({200,Configuration::INTEGER,"Shoot_PortMtr_left",SHOOT_LEFTMTR_PORT,SHOOT_LEFTMTR_PORT,SHOOT_LEFTMTR_PORT,0,0,21,"Shooter Mtr Port"});
    // settings->addSetting({201,Configuration::INTEGER,"Shoot_PortMtr_Right",SHOOT_RIGHTMTR_PORT,SHOOT_RIGHTMTR_PORT,SHOOT_RIGHTMTR_PORT,0,0,21,"Shooter Mtr Port"});

    // settings->addSetting({202,Configuration::INTEGER,"Shoot_RevMtr_Left",SHOOT_LEFTMTR_REVERSED,SHOOT_LEFTMTR_REVERSED,SHOOT_LEFTMTR_REVERSED,0,0,1,"Shooter Mtr Rev"});
    // settings->addSetting({203,Configuration::INTEGER,"Shoot_RevMtr_Right",SHOOT_RIGHTMTR_REVERSED,SHOOT_RIGHTMTR_REVERSED,SHOOT_RIGHTMTR_REVERSED,0,0,1,"Shooter Mtr Rev"});
    // settings->addSetting({204,Configuration::INTEGER,"Shoot_Pneu_Port",SHOOT_PNEUMATIC,SHOOT_PNEUMATIC,SHOOT_PNEUMATIC,0,0,1,"Shooter Pneu Port"});
    
    // settings->addSetting({205,Configuration::INTEGER,"Shoot_PortRot_left",SHOOT_LEFTROT_PORT,SHOOT_LEFTROT_PORT,SHOOT_LEFTROT_PORT,0,0,21,"Shooter Rot Port"});
    // settings->addSetting({206,Configuration::INTEGER,"Shoot_PortRot_Right",SHOOT_RIGHTROT_PORT,SHOOT_RIGHTROT_PORT,SHOOT_RIGHTROT_PORT,0,0,21,"Shooter Rot Port"});

    // settings->addSetting({207,Configuration::INTEGER,"Shoot_RevRot_Left",SHOOT_LEFTROT_REVERSED,SHOOT_LEFTROT_REVERSED,SHOOT_LEFTROT_REVERSED,0,0,1,"Shooter Rot Rev"});
    // settings->addSetting({208,Configuration::INTEGER,"Shoot_RevRot_Right",SHOOT_RIGHTROT_REVERSED,SHOOT_RIGHTROT_REVERSED,SHOOT_RIGHTROT_REVERSED,0,0,1,"Shooter Rot Rev"});
    
    // settings->addSetting({209,Configuration::INTEGER,"Shoot_StartPos",SHOOT_STARTPOS,SHOOT_STARTPOS,SHOOT_STARTPOS,0,0,360000,"Shooter Rot Rev"});
    // settings->addSetting({210,Configuration::INTEGER,"Shoot_StartPos",SHOOT_ENDPOS,SHOOT_ENDPOS,SHOOT_ENDPOS,0,0,360000,"Shooter Rot Rev"});
    
    // //Intake
    // settings->addSetting({220,Configuration::INTEGER,"Intake_Port",INTAKE_MTR_PORT,INTAKE_MTR_PORT,INTAKE_MTR_PORT,0,0,21,"Intake Mtr Port"});
    // settings->addSetting({221,Configuration::INTEGER,"Intake_Rev",INTAKE_MTR_REVERSED,INTAKE_MTR_REVERSED,INTAKE_MTR_REVERSED,0,0,1,"Intake Mtr Reversed"});
    
    // //Expansion
    // settings->addSetting({240,Configuration::INTEGER,"Expand_Port",EXP_PORT,EXP_PORT,EXP_PORT,0,0,21,"Expansion Port"});
    
    return 1;
}