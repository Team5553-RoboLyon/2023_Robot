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

void Robot::TeleopInit() {
  frc::SmartDashboard::PutNumber("P", 0);
  frc::SmartDashboard::PutNumber("I", 0);
  frc::SmartDashboard::PutNumber("D", 0);
  frc::SmartDashboard::PutNumber("Setpoint", 0);
  m_encoder.Reset();
  m_encoder.SetDistancePerPulse((90/22)*360);//conversion de la distance en degré en théorie
}
void Robot::TeleopPeriodic() {
  m_pidController.SetP(frc::SmartDashboard::GetNumber("P", 0));
  m_pidController.SetI(frc::SmartDashboard::GetNumber("I", 0));
  m_pidController.SetD(frc::SmartDashboard::GetNumber("D", 0));
  m_pidController.SetSetpoint(frc::SmartDashboard::GetNumber("Setpoint", 1));


  if (m_joystick.GetRawButton(1)) {
    m_motor.Set(m_pidController.Calculate(m_encoder.GetDistance()));
  } else {
    m_motor.Set(0);
  }
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
