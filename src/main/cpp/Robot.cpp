// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

void Robot::RobotInit()
{
}
void Robot::RobotPeriodic() {}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit()
{
  frc::SmartDashboard::PutNumber("vitesse", 0.0);
}
void Robot::TeleopPeriodic()
{
  double speed = frc::SmartDashboard::GetNumber("vitesse", 0.0);
  if (m_joystick.GetRawButton(1))
  {
    m_moteur.Set(speed);
  }

  else
  {
    m_moteur.Set(0.0);
  }
}

void Robot::DisabledInit()
{
}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main()
{
  return frc::StartRobot<Robot>();
}
#endif
