// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

#define HCLAMP(mn, a, mx) ((a) < (mn) ? (mn) : (a) > (mx) ? (mx) \
                                                          : (a))
// cc
// cc
void Robot::RobotInit() {}
void Robot::RobotPeriodic() {}
// on fait

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit()
{
  m_pidController.Reset(0.0, 0.0, 0.0);
  m_encoder.Reset();
  m_encoder.SetDistancePerPulse((1.0 / 2048.0 * (14.0 / 54.0) * 360.0)); // conversion de la distance en degré en théorie /2048.0*(90.0/22.0)

  m_motor.SetInverted(false);
  m_motor.SetSmartCurrentLimit(40);
  m_motor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
  frc::SmartDashboard::PutNumber("P", 0.0);
  frc::SmartDashboard::PutNumber("I", 0.0);
  frc::SmartDashboard::PutNumber("D", 0.0);
  frc::SmartDashboard::PutNumber("setpoint", 0.0);
}

void Robot::TeleopPeriodic()
{

  double encoderGetDistance = m_encoder.GetDistance();
  double outpout = m_pidController.Calculate(encoderGetDistance);
  frc::SmartDashboard::PutNumber("encoder", encoderGetDistance);
  frc::SmartDashboard::PutNumber("output", m_pidController.m_output);
  m_pidController.SetSetpoint(frc::SmartDashboard::GetNumber("setpoint", 0.0));
  m_pidController.SetGains(frc::SmartDashboard::GetNumber("P", 0.0), frc::SmartDashboard::GetNumber("I", 0.0), frc::SmartDashboard::GetNumber("D", 0.0));
  m_motor.Set(outpout);
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
