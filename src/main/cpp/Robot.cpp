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
  m_motorLeft.ConfigFactoryDefault();
  m_motorRight.ConfigFactoryDefault();
  m_motorLeft.ConfigVoltageCompSaturation(12);
  m_motorRight.ConfigVoltageCompSaturation(12);
  m_motorLeft.SetInverted(true);
  m_motorRight.SetInverted(false);
  m_motorLeft.ConfigOpenloopRamp(0.3);
  m_motorRight.ConfigOpenloopRamp(0.3);
  m_motorRight.Follow(m_motorLeft);
  frc::SmartDashboard::PutNumber("speed", 0.0);
  frc::SmartDashboard::PutBoolean("oui", false);
}
void Robot::TeleopPeriodic()
{
  m_compressor.EnableDigital();
  m_speed = frc::SmartDashboard::GetNumber("speed", 0.0);

  if (m_joystick.GetRawButton(1))
  {
    m_motorLeft.Set(ctre::phoenix::motorcontrol::VictorSPXControlMode::PercentOutput, m_speed);
  }
  else
  {
    m_motorLeft.Set(ctre::phoenix::motorcontrol::VictorSPXControlMode::PercentOutput, 0.0);
  }
  // frc::SmartDashboard::GetBoolean("oui", false) == false
  if (m_joystick.GetRawButton(1))
  {
    m_solenoid.Set(frc::DoubleSolenoid::Value::kForward);
  }
  else
  {
    m_solenoid.Set(frc::DoubleSolenoid::Value::kReverse);
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
