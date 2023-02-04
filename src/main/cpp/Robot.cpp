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

  m_motor.SetInverted(true);
  m_motor.SetSmartCurrentLimit(40);
  m_motor.SetOpenLoopRampRate(0.5);

  frc::SmartDashboard::PutNumber("coef",0.2);

}
void Robot::TeleopPeriodic() {
  m_clamp = frc::SmartDashboard::GetNumber("coef",0.2);
  m_pidController.SetP(frc::SmartDashboard::GetNumber("P", 0));
  m_pidController.SetI(frc::SmartDashboard::GetNumber("I", 0));
  m_pidController.SetD(frc::SmartDashboard::GetNumber("D", 0));
  m_pidController.SetSetpoint(frc::SmartDashboard::GetNumber("Setpoint", 1));


  m_motor.Set(std::clamp(m_joystick.GetZ(), -m_clamp, m_clamp));
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
