// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"
// cc

void Robot::RobotInit() {}
void Robot::RobotPeriodic()
{
  frc2::CommandScheduler::GetInstance().Run();
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit()
{
  m_motorRight.RestoreFactoryDefaults();
  m_motorRight.EnableVoltageCompensation(8);
  m_motorRight.SetSmartCurrentLimit(40);
  m_motorRight.SetInverted(false);
  m_motorRight.SetOpenLoopRampRate(0.3);
  frc::SmartDashboard::PutNumber("speed", 0.0);
  frc::SmartDashboard::PutBoolean("oui", false);
}
void Robot::TeleopPeriodic()
{

  m_speed = frc::SmartDashboard::GetNumber("speed", 0.0);

  m_motorRight.Set(m_speed);
}

void Robot::DisabledInit() {}
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
