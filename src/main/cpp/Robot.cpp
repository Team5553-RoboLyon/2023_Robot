// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"
// cc
// cc
void Robot::RobotInit() {}
void Robot::RobotPeriodic() {}
// on fait

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit()
{
  frc::SmartDashboard::PutNumber("Pelevator", 0);
  frc::SmartDashboard::PutNumber("Parm", 0);
  frc::SmartDashboard::PutNumber("Pturret", 0);
  frc::SmartDashboard::PutNumber("Ielevator", 0);
  frc::SmartDashboard::PutNumber("Iarm", 0);
  frc::SmartDashboard::PutNumber("Iturret", 0);
  frc::SmartDashboard::PutNumber("Delevator", 0);
  frc::SmartDashboard::PutNumber("Darm", 0);
  frc::SmartDashboard::PutNumber("Dturret", 0);
}
void Robot::TeleopPeriodic()
{
  m_robotContainer.m_elevator.SetGains(frc::SmartDashboard::GetNumber("Pelevator", 0),
                                       frc::SmartDashboard::GetNumber("Ielevator", 0),
                                       frc::SmartDashboard::GetNumber("Delevator", 0));

  m_robotContainer.m_arm.SetGains(frc::SmartDashboard::GetNumber("Parm", 0),
                                  frc::SmartDashboard::GetNumber("Iarm", 0),
                                  frc::SmartDashboard::GetNumber("Darm", 0));

  m_robotContainer.m_turret.SetGains(frc::SmartDashboard::GetNumber("Pturret", 0),
                                     frc::SmartDashboard::GetNumber("Iturret", 0),
                                     frc::SmartDashboard::GetNumber("Dturret", 0));
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
