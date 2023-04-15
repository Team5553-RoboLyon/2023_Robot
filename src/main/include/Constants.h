//
#define Length_Gripper 0.7                   // meters
#define Length_Elevator 0.96                 // meters
#define Angle_Elevator 35 * 3.14 / 180       // radians
#define Length_CenterElevator_To_Floor 0.4   // meters
#define Length_CenterElevator_To_FloorX 0.32 // meters
#define Coef_IncrementationX 0.005           // coefficient
#define Min_X 0.3                            // meters
#define Max_X 1.1                            // meters

// #########################    ID Joystick    #########################

#define ID_JOYSTICK_RIGHT 0
#define ID_JOYSTICK_LEFT 1
#define ID_JOYSTICK_COPILOTER 2

// #########################    ID Motor    #########################

#define ID_MOTOR_DRIVE_TRAIN_RIGHT 1
#define ID_MOTOR_DRIVE_TRAIN_RIGHT_2 2
#define ID_MOTOR_DRIVE_TRAIN_RIGHT_3 3
#define ID_MOTOR_DRIVE_TRAIN_LEFT 4
#define ID_MOTOR_DRIVE_TRAIN_LEFT_2 5
#define ID_MOTOR_DRIVE_TRAIN_LEFT_3 6

#define ID_MOTOR_TURRET 7
#define ID_MOTOR_ELEVATOR 8
#define ID_MOTOR_ARM 9
#define ID_MOTOR_GRIPPER 10
#define ID_MOTOR_INTAKE_RIGHT 11
#define ID_MOTOR_INTAKE_LEFT 12
#define ID_MOTOR_CONVEYOR 13

// #########################    ID Solenoid    #########################
#define ID_SOLENOID_SHIFTER_A 0
#define ID_SOLENOID_SHIFTER_B 1
#define ID_SOLENOID_INTAKE_A 3
#define ID_SOLENOID_INTAKE_B 2

// #########################    ID Encoder    #########################

#define ID_ENCODER_DRIVE_TRAIN_RIGHT_A 0
#define ID_ENCODER_DRIVE_TRAIN_RIGHT_B 1
#define ID_ENCODER_DRIVE_TRAIN_LEFT_A 2
#define ID_ENCODER_DRIVE_TRAIN_LEFT_B 3
#define ID_ENCODER_TURRET_A 6
#define ID_ENCODER_TURRET_B 7
#define ID_ENCODER_ELEVATOR_A 4
#define ID_ENCODER_ELEVATOR_B 5
#define ID_ENCODER_ARM_A 8
#define ID_ENCODER_ARM_B 9

// #########################    PID Value    #########################

// Turret

#define P_TURRET 0.02
#define I_TURRET 0.0
#define D_TURRET 0.0

// Elevator
#define P_ELEVATOR 2.25
#define I_ELEVATOR 0.0
#define D_ELEVATOR 0.0
#define ELEVATOR_HIGH_CONE 0.97
#define ELEVATOR_MIDDLE_CONE 0.80
#define ELEVATOR_HIGH_CUBE 0.98
#define ELEVATOR_MIDDLE_CUBE 0.43

// Arm
#define P_ARM 0.8 // 0.4
#define I_ARM 0.0
#define D_ARM 0.0

// #########################    VoltageCompensation    #########################

#define DRIVETRAIN_VOLTAGE_COMPENSATION 10.0
#define TURRET_VOLTAGE_COMPENSATION 8.0
#define ELEVATOR_VOLTAGE_COMPENSATION 8.0
#define ARM_VOLTAGE_COMPENSATION 8.0
#define GRIPPER_VOLTAGE_COMPENSATION 8.0
#define INTAKE_VOLTAGE_COMPENSATION 12.0
#define CONVEYOR_VOLTAGE_COMPENSATION 8.0

// #########################    SmartCurrentLimit    #########################

#define DRIVETRAIN_CURRENT_LIMIT 40
#define TURRET_CURRENT_LIMIT 40
#define ELEVATOR_CURRENT_LIMIT 40
#define ARM_CURRENT_LIMIT 40
#define GRIPPER_CURRENT_LIMIT 40
#define INTAKE_CURRENT_LIMIT 40
#define CONVEYOR_CURRENT_LIMIT 40

// #########################    Ramp    #########################

#define DRIVETRAIN_RAMP 0.0
#define TURRET_RAMP 0.0
#define ELEVATOR_RAMP 0.0
#define ARM_RAMP 0.0
#define GRIPPER_RAMP 0.0
#define INTAKE_RAMP 0.0
#define CONVEYOR_RAMP 0.0

// #########################    SetInvertedMotor    #########################

#define DRIVETRAIN_MOTOR_LEFT_INVERTED true
#define DRIVETRAIN_MOTOR_RIGHT_INVERTED false
#define TURRET_MOTOR_INVERTED true
#define ELEVATOR_MOTOR_INVERTED false
#define GRIPPER_MOTOR_INVERTED false
#define ARM_MOTOR_INVERTED false
#define INTAKE_MOTOR_LEFT_INVERTED false
#define INTAKE_MOTOR_RIGHT_INVERTED true
#define CONVEYOR_MOTOR_INVERTED false

// #########################    SetDistancePerPulse    #########################

#define TURRET_DISTANCE_PER_PULSE ((1.0 / 2048.0) * (14.0 / 54.0) * 360.0)     // en degré
#define ELEVATOR_DISTANCE_PER_PULSE ((1.0 / 7045.0) * 0.96)                    // en mètre
#define ARM_DISTANCE_PER_PULSE ((1.0 / 2048.0 * 2.0 * 3.14159265358979323846)) // en radian

// #########################   ID SUBSYSTEM   #########################

#define ID_SUBSYSTEM_DRIVE_TRAIN 1
#define ID_SUBSYSTEM_TURRET 2
#define ID_SUBSYSTEM_ELEVATOR 3
#define ID_SUBSYSTEM_ARM 4
#define ID_SUBSYSTEM_INTAKE 5

#define TURRET true
#define CAMERA_MAX_ERROR_DIFFERENCE 20
#define CAMERA_WIDTH 320
#define CAMERA_HEIGHT 240
#define CAMERA_TARGET_HIGH_PITCH 220 // TODO real value
#define CAMERA_TARGET_LOW_PITCH 180  // TODO real value
#define CAMERA_HIGH_LOW_DELIMITER 0
