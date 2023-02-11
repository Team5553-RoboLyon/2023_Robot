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
#define CHARGE_STATION_WIDTH 1.0 // en metre

#define TRACTION_WHEEL_DIAMETER             (6.0*0.0254)          // Diametre des roues du robot 
#define TRACTION_WHEEL_RADIUS               (3.0*0.0254)          // rayon des roues du robot 
#define TRACTION_WHEEL_CIRCUMFERENCE        (3.0*0.0254*NF64_2PI) // circonference  des roues du robot 


#include <frc/TimedRobot.h>
#include <frc/Joystick.h>
#include <rev/CANSparkMax.h>
#include <ctre/phoenix/motorcontrol/can/TalonFX.h>
#include <frc/controller/PIDController.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc/BuiltInAccelerometer.h>
#include <frc/ADXRS450_Gyro.h>
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
  // frc::PIDController m_pidController{0, 0, 0};
  frc::ADXRS450_Gyro m_gyro{frc::SPI::Port::kOnboardCS0};

  ctre::phoenix::motorcontrol::can::TalonFX m_MotorRight{1};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorRightFollower{2};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorRightFollower2{3};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorLeft{4};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorLeftFollower{5};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorLeftFollower2{6};


  NdoubleRollingAverage m_AccelerometerX_Average{25};
  NdoubleRollingAverage m_AccelerometerX_Arcos_Average{25};

  NdoubleRollingAverage m_error{10};

  Dynamic m_AccelerometerX;
  Dynamic m_AccelerometerY;
  Dynamic m_Gyro_Angle;

  double m_Sum_Delta_Gyro_Angle=0.0;

  frc::Encoder m_EncoderRight{0,1};
  frc::Encoder m_EncoderLeft{2,3};

  Angle_AG m_FusAngle{0.02,0.075};
  Pid m_PidController{0.0,0.01,0.0005,0.0};

// ARGH !!!!!!!!!!!!!!!!!!!!!!! 
// Pas d'affectation dans la déclaration !!! 
  /*
  double m_kPmin = 0.0075;              !!! MAL
  double m_kPmax = 0.02;                !!! MAL

  double m_distanceParcourue = 0.0;     !!! MAL
  double m_distanceRestante = 0.0;      !!! MAL
  double m_distanceAparcourir = 0.0;    !!! MAL
  */
  double m_kPmin;
  double m_kPmax;

  double m_selfConfidenceMin;   //à la place de  ... double m_kPmin;
  double m_selfConfidenceMax;   //à la place de  ... double m_kPmax;

  double m_traveledDistance;    //à la place de  ... double m_distanceParcourue;
  double m_refDistance;         //à la place de  ... double m_distanceAparcourir;
  // rien ....................    à la place de  ... double m_distanceRestante;

  double m_errorSign;           //à la place de  ... double m_signe_error;
  double m_encoderOrigin;       //à la place de  ... double m_encoder_origin;
};
