// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

void Robot::RobotInit() {}
void Robot::RobotPeriodic()
{
  frc2::CommandScheduler::GetInstance().Run();
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit()
{
  m_Drivetrain.m_logCSV.open("/home/lvuser/", true);
}

void Robot::TeleopPeriodic()
{
  // m_Compressor.EnableDigital();

  if (m_JoystickLeft.GetRawButtonPressed(1))
  {
    m_Drivetrain.InvertBallShifter();
  }

  m_Drivetrain.Drive(
      m_JoystickRight.GetY(), -m_JoystickLeft.GetZ());
}

void Robot::DisabledInit()
{
  m_Drivetrain.m_logCSV.close();
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
