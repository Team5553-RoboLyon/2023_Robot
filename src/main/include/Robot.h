// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/TimedRobot.h>
#include <frc/Joystick.h>
#include <rev/CANSparkMax.h>
#include <ctre/phoenix/motorcontrol/can/TalonFX.h>
#include <frc/controller/PIDController.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc/BuiltInAccelerometer.h>
//gyro
// #include <frc/ADXRS450_Gyro.h>

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

 private:

  frc::Joystick m_joystick{0};
  frc::BuiltInAccelerometer m_accelerometer{};
  // frc::ADXRS450_Gyro m_gyro{};
  frc::PIDController m_pidController{(frc::SmartDashboard::PutNumber("P", frc::SmartDashboard::GetNumber("P", 0.0))), (frc::SmartDashboard::PutNumber("I", frc::SmartDashboard::GetNumber("I", 0.0))), (frc::SmartDashboard::PutNumber("D", frc::SmartDashboard::GetNumber("D", 0.0))) };
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorRight{1};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorRightFollower{2};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorRightFollower2{3};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorLeft{4};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorLeftFollower{5};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorLeftFollower2{6};

};
