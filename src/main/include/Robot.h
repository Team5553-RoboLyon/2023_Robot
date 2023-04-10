// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Joystick.h>

#include <AHRS.h>
#include "frc/I2C.h"
#include "frc/SerialPort.h"
#include "frc/ADXRS450_Gyro.h"
#include "frc/SPI.h"
#include <ctre/phoenix/motorcontrol/can/TalonFX.h>

class Robot : public frc::TimedRobot
{
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

  void SimulationInit() override;
  void SimulationPeriodic() override;

  void Drive(double forward, double turn);

private:
  ctre::phoenix::motorcontrol::can::TalonFX m_leftMotor{4};
  ctre::phoenix::motorcontrol::can::TalonFX m_rightMotor{1};
  ctre::phoenix::motorcontrol::can::TalonFX m_leftMotor2{5};
  ctre::phoenix::motorcontrol::can::TalonFX m_rightMotor2{2};
  ctre::phoenix::motorcontrol::can::TalonFX m_leftMotor3{6};
  ctre::phoenix::motorcontrol::can::TalonFX m_rightMotor3{3};

  AHRS m_gyro{frc::SerialPort::Port::kUSB};
  bool autoBalanceXMode = false;
  bool autoBalanceYMode = false;
  double forward = 0.0;
  double turn = 0.0;
  frc::Joystick m_stickLeft{0};
  frc::Joystick m_stickRight{1};
};
