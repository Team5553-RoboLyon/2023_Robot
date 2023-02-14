// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once
#define SIGMA 0.5

#define AMAX 5.1 // Acceleration Max  au PIF .. à définir aux encodeurs
#define VMAX 3.4 // vitesse Max  théorique (3,395472 sur JVN-DT) .. à vérifier aux encodeurs
#define WMAX                       \
    (((2.0 * VMAX) / AXLETRACK) / \
     1.7) // vitesse angulaire Max theorique	.. à modifier avec Garice


#define NOISE   0.1                                             // à la place de ....   #define bruit 0.1 
#define CHARGE_STATION_WIDTH 1.75 // en metre

#define TRACTION_WHEEL_DIAMETER             (6.0*0.0254)          // Diametre des roues du robot 
#define TRACTION_WHEEL_RADIUS               (3.0*0.0254)          // rayon des roues du robot 
#define TRACTION_WHEEL_CIRCUMFERENCE        (3.0*0.0254*NF64_2PI) // circonference  des roues du robot 
#define K_ANTICIPATION 0.4

#include <frc/TimedRobot.h>
#include <frc/Joystick.h>
#include <rev/CANSparkMax.h>
#include <ctre/phoenix/motorcontrol/can/TalonFX.h>
#include <frc/controller/PIDController.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc/BuiltInAccelerometer.h>
#include <frc/ADXRS450_Gyro.h>
#include <ChargeStationTiltTracker.h>
#include <frc/SPI.h>
#include <lib/NRollingAverage.h>
#include <lib/rate_limiter.h>
#include <lib/Dynamic.h>
#include <frc/Encoder.h>
#include <lib/Angle_AG.h>
#include <lib/Pid.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <lib/NLCsv.h>

//gyro

class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;
  void RobotPeriodic() override;

  void AutonomousInit() override;
  void AutonomousPeriodic() override;

  void TeleopInit() override;
  void TeleopPeriodic() override;

  void DisabledInit() override;
  void DisabledPeriodic() override;

  void TestInit() override;
  void TestPeriodic() override;


  // double signe(double x);
  double Calcul_De_Notre_Brave_JM(double forward, double turn, bool wheelSide);

  double GetAngle();

  //machine à état
  enum State
  {
    Init,
    Adjusting,
    End
  };

  State m_state=State::End;

 private:

  frc::Joystick m_joystickRight{0};
  frc::Joystick m_joystickLeft{1};
  frc::BuiltInAccelerometer m_accelerometer{};
  // frc::PIDController m_AngleController{0, 0, 0};
  frc::ADXRS450_Gyro m_gyro{frc::SPI::Port::kOnboardCS0};

  ctre::phoenix::motorcontrol::can::TalonFX m_MotorRight{1};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorRightFollower{2};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorRightFollower2{3};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorLeft{4};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorLeftFollower{5};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorLeftFollower2{6};


  NdoubleRollingAverage m_AccelerometerX_Average{25};
  NdoubleRollingAverage m_AccelerometerX_Arcos_Average{25};
  NdoubleRollingAverage m_gyroRateAverage{25};

  NdoubleRollingAverage m_error{10};

  Dynamic m_AccelerometerX;
  Dynamic m_AccelerometerY;
  Dynamic m_GyroAngle;

  double m_Sum_Delta_Gyro_Angle=0.0;

  frc::Encoder m_EncoderRight{0,1,true};
  frc::Encoder m_EncoderLeft{2,3,false};

  Angle_AG m_FusAngle{0.02,0.075};
  Pid m_VangleController{0.0,0.01,0.0005,0.0};
  Pid m_AngleController{0.0,0.01,0.0005,0.0};

  TiltTracker m_TiltTracker{0.05,1.0,0.2};//{80.0,0.5,0.4}; ça marche

  double m_kPmin;
  double m_kPmax;

  double m_selfConfidenceMin;   
  double m_selfConfidenceMax;   

  double m_traveledDistance;    
  double m_refDistance;         
  

  double m_errorSign;         
  double m_encoderOrigin; 

  double m_vOutput;
  double m_Output;
  double m_AngleOutput;
  double m_LogFusAngle;
  double m_LogGyroRate;
  double m_LogGyroRateAverage;
  double m_LogAngleAccel;
  double m_LogEncoderM;
  double m_LogAccelX;


  NLCSV m_logCSV{11};    
};
