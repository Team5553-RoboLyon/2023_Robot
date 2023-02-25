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
  m_encoder.Reset();
  m_encoder.SetDistancePerPulse(1.0 / 2048.0);

  m_motor.SetInverted(true);
  m_motor.SetSmartCurrentLimit(40);
  m_motor.EnableVoltageCompensation(12);
  m_motor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
  frc::SmartDashboard::PutNumber("clamp", 0.0);
}
void Robot::TeleopPeriodic()
{
  if (m_encoder.GetDistance() < 0)
  {
    m_motor.Set(0.0);
  }
  m_encoderGetDistance = m_encoder.GetDistance();
  m_vitesse = (m_encoderGetDistance - m_lastDistance) / 0.02;
  if (m_stick.GetRawButton(1))
  {
    if (m_encoder.GetDistance() < 3)
    {
      m_motor.Set(-1.0);
    }
    else
    {
      m_motor.Set(0.0);
    }
  }
  else
  {
    m_motor.Set(m_stick.GetY());
  }
  frc::SmartDashboard::PutNumber("encoder", m_encoder.GetDistance());
  frc::SmartDashboard::PutNumber("vitesse", m_vitesse);
  // frc::SmartDashboard::PutNumber("");
  m_clamp = frc::SmartDashboard::GetNumber("clamp", 0.0);
  m_lastDistance = m_encoderGetDistance;
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
