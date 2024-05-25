#pragma once

#include "ctrcvu/ctrclib.hpp"
#include "okapi/api.hpp"
#include <vector>

//Robot definitions
#define SPARKY  0
#define RIZ 1
#define AVENGER 2

#define ROBOT   AVENGER

#if ROBOT == AVENGER
//Motors
#define MTR_LEFT_PORT_A             17
#define MTR_LEFT_PORT_B             18
#define MTR_LEFT_PORT_C             19
#define MTR_LEFT_PORT_D             20
#define MTR_LEFT_PORT_E             0
#define MTR_RIGHT_PORT_A            12
#define MTR_RIGHT_PORT_B            13
#define MTR_RIGHT_PORT_C            14
#define MTR_RIGHT_PORT_D            15
#define MTR_RIGHT_PORT_E            0

#define MTR_LEFT_REVERSED_A         false
#define MTR_LEFT_REVERSED_B         true
#define MTR_LEFT_REVERSED_C         false
#define MTR_LEFT_REVERSED_D         true
#define MTR_LEFT_REVERSED_E         true

#define MTR_RIGHT_REVERSED_A        true
#define MTR_RIGHT_REVERSED_B        false
#define MTR_RIGHT_REVERSED_C        true
#define MTR_RIGHT_REVERSED_D        false
#define MTR_RIGHT_REVERSED_E        false

//0 = left; 1 = right
#define MTR_LEFT_SIDE_A             false
#define MTR_LEFT_SIDE_B             false
#define MTR_LEFT_SIDE_C             false
#define MTR_LEFT_SIDE_D             false
#define MTR_LEFT_SIDE_E             false
#define MTR_RIGHT_SIDE_A            true
#define MTR_RIGHT_SIDE_B            true
#define MTR_RIGHT_SIDE_C            true
#define MTR_RIGHT_SIDE_D            true
#define MTR_RIGHT_SIDE_E            true

//Rotation sensors
#define DRIVE_ROT_LEFT              11
#define DRIVE_ROT_RIGHT             16

#define MTR_GEARSET                 okapi::AbstractMotor::gearset::blue
#define MTR_GEAR_RATIO              (3.0/4.0)

#define MTR_SLEW                    0
#define MTR_FORWARD_SCALE           1
#define MTR_TURN_SCALE              1

//PIDs
#define MTR_LEFT_KP                 0.0002
#define MTR_LEFT_KI                 0
#define MTR_LEFT_KD                 0.0001
#define MTR_LEFT_KF                 0

#define MTR_RIGHT_KP                0.0002
#define MTR_RIGHT_KI                0
#define MTR_RIGHT_KD                0.0001
#define MTR_RIGHT_KF                0

//Odometry
#define ODOM_ROT_PORT_LEFT          11
#define ODOM_ROT_PORT_RIGHT         16
#define ODOM_ROT_PORT_MIDDLE        0

#define ODOM_ROT_REVERSED_LEFT      true
#define ODOM_ROT_REVERSED_RIGHT     false
#define ODOM_ROT_REVERSED_MIDDLE    false

#define ODOM_SCALE_LEFTRADIUS       5.1927
#define ODOM_SCALE_RIGHTRADIUS      5.1927
#define ODOM_SCALE_MIDDLERADIUS     0.0

#define ODOM_SCALE_LEFTDIAMETER     2.75
#define ODOM_SCALE_RIGHTDIAMETER    2.75
#define ODOM_SCALE_MIDDLEDIAMETER   0.0

//Shooter
#define SHOOT_LEFTMTR_PORT          7
#define SHOOT_RIGHTMTR_PORT         8
#define SHOOT_LEFTMTR_REVERSED      true
#define SHOOT_RIGHTMTR_REVERSED     true
#define SHOOT_PNEUMATIC             'A'

#define SHOOT_STARTPOS              0
#define SHOOT_ENDPOS                0

//Intake
#define INTAKE_MTR_PORT             10
#define INTAKE_MTR_REVERSED         false

//Roller
#define ROLLER_SNR_PORT             15
#define ROLLER_MTR_PORT             9

//Expansion
#define EXP_PORT                    'A'


//Defualt ports for robots
#elif ROBOT == RIZ

//Motors
#define MTR_LEFT_PORT_A             11
#define MTR_LEFT_PORT_B             1
#define MTR_LEFT_PORT_C             2
#define MTR_LEFT_PORT_D             17
#define MTR_LEFT_PORT_E             0
#define MTR_RIGHT_PORT_A            16
#define MTR_RIGHT_PORT_B            20
#define MTR_RIGHT_PORT_C            7
#define MTR_RIGHT_PORT_D            10
#define MTR_RIGHT_PORT_E            0

#define MTR_LEFT_REVERSED_A         true
#define MTR_LEFT_REVERSED_B         true
#define MTR_LEFT_REVERSED_C         true
#define MTR_LEFT_REVERSED_D         true
#define MTR_LEFT_REVERSED_E         true
#define MTR_RIGHT_REVERSED_A        false
#define MTR_RIGHT_REVERSED_B        false
#define MTR_RIGHT_REVERSED_C        false
#define MTR_RIGHT_REVERSED_D        false
#define MTR_RIGHT_REVERSED_E        false

//0 = left; 1 = right
#define MTR_LEFT_SIDE_A             false
#define MTR_LEFT_SIDE_B             false
#define MTR_LEFT_SIDE_C             false
#define MTR_LEFT_SIDE_D             false
#define MTR_LEFT_SIDE_E             false
#define MTR_RIGHT_SIDE_A            true
#define MTR_RIGHT_SIDE_B            true
#define MTR_RIGHT_SIDE_C            true
#define MTR_RIGHT_SIDE_D            true
#define MTR_RIGHT_SIDE_E            true

//Rotation sensors
#define DRIVE_ROT_LEFT              14
#define DRIVE_ROT_RIGHT             6

#define MTR_GEARSET                 okapi::AbstractMotor::gearset::blue
#define MTR_GEAR_RATIO              (3.0/4.0)

#define MTR_SLEW                    0
#define MTR_FORWARD_SCALE           0.5
#define MTR_TURN_SCALE              0.5

//PIDs
#define MTR_LEFT_KP                 0.0005
#define MTR_LEFT_KI                 0
#define MTR_LEFT_KD                 0.0012
#define MTR_LEFT_KF                 0
#define MTR_RIGHT_KP                0.0005
#define MTR_RIGHT_KI                0
#define MTR_RIGHT_KD                0.0012
#define MTR_RIGHT_KF                0

//Odometry
#define ODOM_ROT_PORT_LEFT          14
#define ODOM_ROT_PORT_RIGHT         6
#define ODOM_ROT_PORT_MIDDLE        0

#define ODOM_ROT_REVERSED_LEFT      true
#define ODOM_ROT_REVERSED_RIGHT     false
#define ODOM_ROT_REVERSED_MIDDLE    false

#define ODOM_SCALE_LEFTRADIUS       5.200095
#define ODOM_SCALE_RIGHTRADIUS      5.200095
#define ODOM_SCALE_MIDDLERADIUS     0.0

#define ODOM_SCALE_LEFTDIAMETER     3.265
#define ODOM_SCALE_RIGHTDIAMETER    3.265
#define ODOM_SCALE_MIDDLEDIAMETER   0.0

//Shooter
#define SHOOT_LEFTMTR_PORT          3
#define SHOOT_RIGHTMTR_PORT         4
#define SHOOT_LEFTMTR_REVERSED      true
#define SHOOT_RIGHTMTR_REVERSED     false
#define SHOOT_PNEUMATIC             'A'

#define SHOOT_STARTPOS              0
#define SHOOT_ENDPOS                0

//Intake
#define INTAKE_MTR_PORT             5
#define INTAKE_MTR_REVERSED         false

//Roller
#define ROLLER_SNR_PORT             15
#define ROLLER_MTR_PORT             19

//Expansion
#define EXP_PORT                    'B'

#else

//Motors
#define MTR_LEFT_PORT_A             19
#define MTR_LEFT_PORT_B             13
#define MTR_LEFT_PORT_C             0
#define MTR_LEFT_PORT_D             0
#define MTR_LEFT_PORT_E             0
#define MTR_RIGHT_PORT_A            14
#define MTR_RIGHT_PORT_B            20
#define MTR_RIGHT_PORT_C            0
#define MTR_RIGHT_PORT_D            0
#define MTR_RIGHT_PORT_E            0

#define MTR_LEFT_REVERSED_A         true
#define MTR_LEFT_REVERSED_B         true
#define MTR_LEFT_REVERSED_C         true
#define MTR_LEFT_REVERSED_D         true
#define MTR_LEFT_REVERSED_E         true
#define MTR_RIGHT_REVERSED_A        false
#define MTR_RIGHT_REVERSED_B        false
#define MTR_RIGHT_REVERSED_C        false
#define MTR_RIGHT_REVERSED_D        false
#define MTR_RIGHT_REVERSED_E        false

//0 = left; 1 = right
#define MTR_LEFT_SIDE_A             0
#define MTR_LEFT_SIDE_B             0
#define MTR_LEFT_SIDE_C             0
#define MTR_LEFT_SIDE_D             0
#define MTR_LEFT_SIDE_E             0
#define MTR_RIGHT_SIDE_A            1
#define MTR_RIGHT_SIDE_B            1
#define MTR_RIGHT_SIDE_C            1
#define MTR_RIGHT_SIDE_D            1
#define MTR_RIGHT_SIDE_E            1

#define MTR_GEARSET                 okapi::AbstractMotor::gearset::green
#define MTR_GEAR_RATIO              1

//Rotation sensors
#define DRIVE_ROT_LEFT              9
#define DRIVE_ROT_RIGHT             9

//PIDs
#define MTR_LEFT_KP                 0.0015
#define MTR_LEFT_KI                 0.000
#define MTR_LEFT_KD                 0.0015
#define MTR_LEFT_KF                 0
#define MTR_RIGHT_KP                0.0015
#define MTR_RIGHT_KI                0.000
#define MTR_RIGHT_KD                0.0015
#define MTR_RIGHT_KF                0

#define MTR_SLEW                    0
#define MTR_FORWARD_SCALE           1
#define MTR_TURN_SCALE              1

//Odometry
#define ODOM_ROT_PORT_LEFT          16
#define ODOM_ROT_PORT_RIGHT         18
#define ODOM_ROT_PORT_MIDDLE        0

#define ODOM_ROT_REVERSED_LEFT      true
#define ODOM_ROT_REVERSED_RIGHT     false
#define ODOM_ROT_REVERSED_MIDDLE    false

#define ODOM_SCALE_LEFTRADIUS       5.252
#define ODOM_SCALE_RIGHTRADIUS      5.252
#define ODOM_SCALE_MIDDLERADIUS     0

#define ODOM_SCALE_LEFTDIAMETER     2.77
#define ODOM_SCALE_RIGHTDIAMETER    2.77
#define ODOM_SCALE_MIDDLEDIAMETER   0

//Shooter
#define SHOOT_LEFTMTR_PORT          1
#define SHOOT_RIGHTMTR_PORT         3
#define SHOOT_LEFTMTR_REVERSED      true
#define SHOOT_RIGHTMTR_REVERSED     true
#define SHOOT_PNEUMATIC             1

#define SHOOT_LEFTROT_PORT          1
#define SHOOT_RIGHTROT_PORT         1
#define SHOOT_LEFTROT_REVERSED      0
#define SHOOT_RIGHTROT_REVERSED     1

#define SHOOT_STARTPOS              1
#define SHOOT_ENDPOS                1

//Intake
#define INTAKE_MTR_PORT             1
#define INTAKE_MTR_REVERSED         0
#define INTAKE_SENSOR               'B'

//Expansion
#define EXP_PORT                    1

#endif


//Settings array for robots
int loadDefaultSettings(std::shared_ptr<sparkyLib::Configuration> settings);