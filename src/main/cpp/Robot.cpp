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
  //**************************LOGGING**************************
  m_logCSV.open("home/lvuser/", true);
  m_voltage = units::volt_t{0.0};

  //*******************************************************
  m_encoder.Reset();
  m_encoder.SetDistancePerPulse(1.0 / 2048.0);

  m_motor.SetInverted(false);
  m_motor.SetSmartCurrentLimit(40);
  m_motor.EnableVoltageCompensation(12);
  m_motor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
  frc::SmartDashboard::PutNumber("clamp", 0.0);

  m_logCSV.setItem(1, "voltage", 5, &m_voltageDouble);
  m_logCSV.setItem(2, "current", 5, &m_current);
  m_logCSV.setItem(3, "encoder", 5, &m_encoderGetDistance);
  m_logCSV.setItem(4, "vitesse_encodeur", 5, &m_vitesse);
  m_logCSV.setItem(5, "getAppliedOutput", 5, &m_appliedOutput);
  m_logCSV.setItem(6, "busVoltage", 5, &m_busVoltage);
}
void Robot::TeleopPeriodic()
{
  //******************smartdashboard******************

  m_clamp = frc::SmartDashboard::GetNumber("clamp", 0.0);
  m_encoderGetDistance = m_encoder.GetDistance();
  m_vitesse = (m_encoderGetDistance - m_lastDistance) / 0.02;
  m_current = m_motor.GetOutputCurrent();
  frc::SmartDashboard::PutNumber("encoder", m_encoderGetDistance);
  frc::SmartDashboard::PutNumber("vitesse", m_vitesse);
  frc::SmartDashboard::PutNumber("current", m_current);
  frc::SmartDashboard::PutNumber("getAppliedOutput", m_appliedOutput);
  frc::SmartDashboard::PutNumber("busVoltage", m_busVoltage);
  m_lastDistance = m_encoderGetDistance;
  m_busVoltage = m_motor.GetBusVoltage();
  m_appliedOutput = m_motor.GetAppliedOutput();

  //******************LOGGING******************

  frc::SmartDashboard::PutNumber("voltage", m_voltageDouble);
  if (m_stick.GetRawButtonPressed(2))
  {
    m_voltage += units::volt_t{0.1};
    m_voltageDouble += 0.1;
  }
  if (m_encoderGetDistance < 3.4)
  {
    if (m_stick.GetRawButton(1))
    {
      m_motor.SetVoltage(m_voltage);
    }
    else
    {
      m_motor.Set(-m_stick.GetY() * 0.2);
    }
  }
  else
  {
    m_motor.Set(0.0);
  }

  m_logCSV.write();
  //********************************************************************************

  // m_compressor.EnableDigital();

  // if (m_stick.GetRawButton(1))
  // {
  //   if (m_encoder.GetDistance() < 2)
  //   {
  //     m_motor.Set(-0.8);
  //   }
  //   else
  //   {
  //     m_motor.Set(0.0);
  //   }
  // }
  // else
  // {
  //   m_motor.Set(m_stick.GetY() * m_clamp);
  // }

  // if (m_stick.GetRawButtonPressed(1))
  // {
  //   m_solenoid.Set(frc::DoubleSolenoid::Value::kReverse);
  // }

  // if (m_stick.GetRawButtonPressed(2))
  // {
  //   m_solenoid.Set(frc::DoubleSolenoid::Value::kForward);
  // }
}

void Robot::DisabledInit()
{
  m_logCSV.close();
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
