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

#define NABS(a)         (((a) < 0) ? -(a) : (a))     // VALEUR ABSOLUE
#define NMAX(a, b)      (((a) > (b)) ? (a) : (b)) // Max
#define NMIN(a, b)      (((a) < (b)) ? (a) : (b)) // Min
#define NROUND(fval)	( ( (fval) >= 0.0f ) ? ((Ns32)((fval) + 0.5f)) : ((Ns32)((fval) - 0.5f)) )
#define NSIGN(a)        (((a)<0) ? -1:1)	
#define NCLAMP(mn,a,mx)	( ((a)<(mn)) ? (mn) : ((a)>(mx)) ? (mx) : (a) )
#define NLERP(a,b,t)	( a + (b - a)*t )

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


  double signe(double x);
  double Calcul_De_Notre_Brave_JM(double forward, double turn, bool wheelSide);

 private:

  frc::Joystick m_joystickRight{0};
  frc::Joystick m_joystickLeft{1};
  frc::BuiltInAccelerometer m_accelerometer{};
  frc::PIDController m_pidController{0, 0, 0};
  frc::ADXRS450_Gyro m_gyro;

  ctre::phoenix::motorcontrol::can::TalonFX m_MotorRight{1};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorRightFollower{2};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorRightFollower2{3};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorLeft{4};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorLeftFollower{5};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorLeftFollower2{6};

};
