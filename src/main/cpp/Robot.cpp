// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"
// cc
// cc
void Robot::ChangePosition()
{
  if (m_solenoid.Get() == frc::DoubleSolenoid::Value::kForward)
  {
    m_solenoid.Set(frc::DoubleSolenoid::Value::kReverse);
  }
  else
  {
    m_solenoid.Set(frc::DoubleSolenoid::Value::kForward);
  }
}

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
  if (m_stick.GetRawButtonPressed(1))
  {
    m_voltage += 0.2;
  }

  m_compressor.EnableDigital();
  m_clamp = frc::SmartDashboard::GetNumber("clamp", 0.0);
  // if (m_encoder.GetDistance() < 0)
  // {
  //   m_motor.Set(0.0);
  // }
  m_encoderGetDistance = m_encoder.GetDistance();
  m_vitesse = (m_encoderGetDistance - m_lastDistance) / 0.02;
  if (m_stick.GetRawButton(1))
  {
    if (m_encoder.GetDistance() < 2)
    {
      m_motor.Set(-0.8);
    }
    else
    {
      m_motor.Set(0.0);
    }
  }
  else
  {
    m_motor.Set(m_stick.GetY() * m_clamp);
  }
  frc::SmartDashboard::PutNumber("encoder", m_encoder.GetDistance());
  frc::SmartDashboard::PutNumber("vitesse", m_vitesse);
  frc::SmartDashboard::PutNumber("current", m_motor.GetOutputCurrent());
  m_lastDistance = m_encoderGetDistance;

  // if (m_stick.GetRawButtonPressed(1))
  // {
  //   m_solenoid.Set(frc::DoubleSolenoid::Value::kReverse);
  // }

  // if (m_stick.GetRawButtonPressed(2))
  // {
  //   m_solenoid.Set(frc::DoubleSolenoid::Value::kForward);
  // }
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
