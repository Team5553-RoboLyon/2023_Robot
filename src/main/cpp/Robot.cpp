// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"
#include <iostream>
#include <frc/Timer.h>
#include "units/time.h"

#define kOffBalanceThresholdDegrees 10.0f
#define kOnBalanceThresholdDegrees 5.0f
// cc
// cc

void Robot::Drive(double forward, double turn)
{
  m_leftMotor.Set(ctre::phoenix::motorcontrol::TalonFXControlMode::PercentOutput, forward + turn);
  m_rightMotor.Set(ctre::phoenix::motorcontrol::TalonFXControlMode::PercentOutput, forward - turn);
  m_leftMotor2.Set(ctre::phoenix::motorcontrol::TalonFXControlMode::PercentOutput, forward + turn);
  m_rightMotor2.Set(ctre::phoenix::motorcontrol::TalonFXControlMode::PercentOutput, forward - turn);
  m_leftMotor3.Set(ctre::phoenix::motorcontrol::TalonFXControlMode::PercentOutput, forward + turn);
  m_rightMotor3.Set(ctre::phoenix::motorcontrol::TalonFXControlMode::PercentOutput, forward - turn);
}

void Robot::RobotInit()
{
}

void Robot::RobotPeriodic()
{
}
// on fait

void Robot::AutonomousInit()
{
}
void Robot::AutonomousPeriodic()
{
}

void Robot::TeleopInit()
{
  m_gyro.Reset();
  m_gyro.Calibrate();

  m_leftMotor.ConfigFactoryDefault();
  m_rightMotor.ConfigFactoryDefault();
  m_leftMotor2.ConfigFactoryDefault();
  m_rightMotor2.ConfigFactoryDefault();
  m_leftMotor3.ConfigFactoryDefault();
  m_rightMotor3.ConfigFactoryDefault();

  m_leftMotor.SetInverted(true);
  m_leftMotor2.SetInverted(true);
  m_leftMotor3.SetInverted(true);
  m_rightMotor.SetInverted(false);
  m_rightMotor2.SetInverted(false);
  m_rightMotor3.SetInverted(false);

  m_leftMotor.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
  m_rightMotor.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
  m_leftMotor2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
  m_rightMotor2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
  m_leftMotor3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
  m_rightMotor3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
}
void Robot::TeleopPeriodic()
{

  std::cout << "navX" << m_gyro.GetAngle() << std::endl;
  frc::SmartDashboard::PutNumber("navX", m_gyro.GetAngle());

  double pitchAngleDegrees = m_gyro.GetPitch();
  double rollAngleDegrees = m_gyro.GetRoll();

  if (!autoBalanceXMode && (fabs(pitchAngleDegrees) >= kOffBalanceThresholdDegrees))
  {
    autoBalanceXMode = true;
  }
  else if (autoBalanceXMode && (fabs(pitchAngleDegrees) <= fabs(kOnBalanceThresholdDegrees)))
  {
    autoBalanceXMode = false;
  }

  if (!autoBalanceYMode && (fabs(pitchAngleDegrees) >= fabs(kOffBalanceThresholdDegrees)))
  {
    autoBalanceYMode = true;
  }
  else if (autoBalanceYMode && (fabs(pitchAngleDegrees) <= fabs(kOnBalanceThresholdDegrees)))
  {
    autoBalanceYMode = false;
  }

  // Control drive system automatically,
  // driving in reverse direction of pitch/roll angle,
  // with a magnitude based upon the angle

  if (autoBalanceXMode)
  {
    double pitchAngleRadians = pitchAngleDegrees * (M_PI / 180.0);
    forward = sin(pitchAngleRadians) * -1;
  }
  else
  {
    forward = m_stickLeft.GetY();
  }
  if (autoBalanceYMode)
  {
    double rollAngleRadians = rollAngleDegrees * (M_PI / 180.0);
    turn = sin(rollAngleRadians) * -1;
  }
  else
  {
    turn = m_stickRight.GetX();
  }
  // Use the joystick X axis for lateral movement,
  // Y axis for forward movement, and Z axis for rotation.
  Drive(forward, turn);
  frc::Wait(units::second_t(0.005)); // wait 5ms to avoid hogging CPU cycles
}

void Robot::DisabledInit()
{
}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}

void Robot::SimulationInit() {}
void Robot::SimulationPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main()
{
  return frc::StartRobot<Robot>();
}
#endif
