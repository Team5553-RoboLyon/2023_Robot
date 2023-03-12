//
#define Length_Gripper 0.7                   // meters
#define Length_Elevator 0.96                 // meters
#define Angle_Elevator 35 * 3.14 / 180       // radians
#define Length_CenterElevator_To_Floor 0.4   // meters
#define Length_CenterElevator_To_FloorX 0.32 // meters
#define Coef_IncrementationX 0.005           // coefficient
#define Min_X 0.3                            // meters
#define Max_X 1.1                            // meters

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
#define ID_MOTOR_INTAKE 10
#define ID_MOTOR_INTAKE_FOLLOWER 11

// #########################    ID Solenoid    #########################
#define ID_SOLENOID_SHIFTER_A 0
#define ID_SOLENOID_SHIFTER_B 1
#define ID_SOLENOID_INTAKE_A 2
#define ID_SOLENOID_INTAKE_B 3
#define ID_SOLENOID_GRIPPER_A 4
#define ID_SOLENOID_GRIPPER_B 5

// #########################    ID Encoder    #########################

// #define ID_ENCODER_DRIVE_TRAIN_RIGHT_A 0
#define ID_ENCODER_DRIVE_TRAIN_RIGHT_B 1
#define ID_ENCODER_DRIVE_TRAIN_LEFT_A 2
#define ID_ENCODER_DRIVE_TRAIN_LEFT_B 3
#define ID_ENCODER_TURRET_A 0
#define ID_ENCODER_TURRET_B 21
#define ID_ENCODER_ELEVATOR_A 22
#define ID_ENCODER_ELEVATOR_B 23
#define ID_ENCODER_ARM_A 8
#define ID_ENCODER_ARM_B 9

// #########################    ID Hall    #########################

// Turret
#define ID_HALL_TURRET_LEFT 0
#define ID_HALL_TURRET_RIGHT 1

// Elevator
#define ID_HALL_ELEVATOR_UP 10

// #########################    PID Value    #########################

// Turret

#define P_TURRET 0.02
#define I_TURRET 0.0
#define D_TURRET 0.0

// Elevator
#define P_ELEVATOR 2.25
#define I_ELEVATOR 0.0
#define D_ELEVATOR 0.0

// Arm
#define P_ARM 0.4
#define I_ARM 0.0
#define D_ARM 0.0

// #########################    VoltageCompensation    #########################

#define DRIVETRAIN_VOLTAGE_COMPENSATION 10.0
#define TURRET_VOLTAGE_COMPENSATION 8.0
#define ELEVATOR_VOLTAGE_COMPENSATION 8.0
#define ARM_VOLTAGE_COMPENSATION 8.0
#define INTAKE_VOLTAGE_COMPENSATION 12.0

// #########################    SmartCurrentLimit    #########################

#define DRIVETRAIN_CURRENT_LIMIT 40
#define TURRET_CURRENT_LIMIT 40
#define ELEVATOR_CURRENT_LIMIT 40
#define ARM_CURRENT_LIMIT 40
#define INTAKE_CURRENT_LIMIT 40

// #########################    Ramp    #########################

#define DRIVETRAIN_RAMP 0.0
#define TURRET_RAMP 0.0
#define ELEVATOR_RAMP 0.0
#define ARM_RAMP 0.0
#define INTAKE_RAMP 0.0

// #########################    SetInvertedMotor    #########################

#define DRIVETRAIN_MOTOR_LEFT_INVERTED true
#define DRIVETRAIN_MOTOR_RIGHT_INVERTED false
#define TURRET_MOTOR_INVERTED true
#define ELEVATOR_MOTOR_INVERTED false
#define ARM_MOTOR_INVERTED false
#define INTAKE_MOTOR_LEFT_INVERTED false
#define INTAKE_MOTOR_RIGHT_INVERTED true

// #########################    SetDistancePerPulse    #########################

#define TURRET_DISTANCE_PER_PULSE ((1.0 / 2048.0) * (14.0 / 54.0) * 360.0)
#define ELEVATOR_DISTANCE_PER_PULSE ((1.0 / 7045.0) * 0.96) //(1.0 / 2048.0) * 1.0 / 3.44 * 0.96
#define ARM_DISTANCE_PER_PULSE ((1.0 / 2048.0 * 2.0 * 3.14159265358979323846))