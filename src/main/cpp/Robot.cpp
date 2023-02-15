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
  m_logCSV.open("/home/lvuser/", true);
  frc::SmartDashboard::PutNumber("Setpoint", 0);
  m_encoder.Reset();
  m_encoder.SetDistancePerPulse((1.0/2048.0*(14.0/54.0)*360.0));//conversion de la distance en degré en théorie /2048.0*(90.0/22.0)
  m_encoderMotor.SetPosition(0.0);
  m_encoderMotor.SetPositionConversionFactor(((1.0/24.5)*(14.0/54.0)*360.0));

  m_motor.SetInverted(false);
  m_motor.SetSmartCurrentLimit(40);
  m_motor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);

  frc::SmartDashboard::PutNumber("coef",0.0);

  m_motor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);


  m_logCSV.setItem(0,"m_encoderThrougboreDeg",5, &m_encoderThrougboreDeg);
  m_logCSV.setItem(1,"m_encoderMotorDeg",5,&m_encoderMotorDeg);
  m_logCSV.setItem(2,"voltage",5, &m_voltage);
  m_logCSV.setItem(3,"current",5, &m_current);

}

void Robot::TeleopPeriodic() {
  m_voltage=5.0;
  m_current = m_motor.GetOutputCurrent();
  m_encoderThrougboreDeg = m_encoder.GetDistance();
  m_encoderMotorDeg=m_encoderMotor.GetPosition();
  m_clamp = frc::SmartDashboard::GetNumber("coef",0.0);
  frc::SmartDashboard::PutNumber("m_encoderThrougboreDeg", m_encoderThrougboreDeg);
  frc::SmartDashboard::PutNumber("m_encoderMotorDeg",m_encoderMotor.GetPosition());
  frc::SmartDashboard::PutNumber("voltage", m_voltage);
  frc::SmartDashboard::PutNumber("current", m_current);
  double speed=std::clamp(m_joystick.GetZ(),-m_clamp,m_clamp);
  frc::SmartDashboard::PutNumber("speed",speed);
  frc::SmartDashboard::PutNumber("joystick",m_joystick.GetZ());

  m_motor.Set(speed);
  // if (m_encoderThrougboreDeg<360.0)
  // {
  //   m_motor.SetVoltage(units::voltage::volt_t (m_voltage));
  // }
  // else
  // {
  //   m_motor.SetVoltage(units::voltage::volt_t (0.0));
  // }

  m_logCSV.write();
}

void Robot::DisabledInit() {
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
