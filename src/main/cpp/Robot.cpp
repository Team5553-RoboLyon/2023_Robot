// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"
//cc

void Robot::RobotInit() {}
void Robot::RobotPeriodic() {
  frc2::CommandScheduler::GetInstance().Run();
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {  
  m_motorLeft.RestoreFactoryDefaults();
  m_motorRight.RestoreFactoryDefaults();
  m_motorLeft.SetSmartCurrentLimit(40);
  m_motorRight.SetSmartCurrentLimit(40);
  m_motorLeft.SetInverted(true);
  m_motorRight.SetInverted(true);
  m_motorLeft.SetOpenLoopRampRate(0.3);
  m_motorRight.SetOpenLoopRampRate(0.3);

}
void Robot::TeleopPeriodic() {
  if (m_stick.GetRawButton(1))
  {
    m_motorLeft.Set(1);
    m_motorRight.Set(0.8);
  }
  else
  {
    m_motorLeft.Set(0);
    m_motorRight.Set(0);
  }
  



}

void Robot::DisabledInit() {}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}

void Robot::SimulationInit() {}
void Robot::SimulationPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
