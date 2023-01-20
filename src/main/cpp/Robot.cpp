// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

double Robot::signe(double x) {
  if (x > 0) {
    return 1;
  } else if (x < 0) {
    return -1;
  } else {
    return 0;
  }
}

void Robot::RobotInit() {}
void Robot::RobotPeriodic() {}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {
  frc::SmartDashboard::PutNumber("P", 0.5);
  frc::SmartDashboard::PutNumber("I", 0);
  frc::SmartDashboard::PutNumber("D", 0);
  frc::SmartDashboard::PutNumber("Setpoint", 1);
}
void Robot::TeleopPeriodic() {
  m_pidController.SetP(frc::SmartDashboard::GetNumber("P", 0.5));
  m_pidController.SetI(frc::SmartDashboard::GetNumber("I", 0));
  m_pidController.SetD(frc::SmartDashboard::GetNumber("D", 0));
  double z = std::abs(m_accelerometer.GetZ()); // -1/1
  double y =m_accelerometer.GetY();

  double output = m_pidController.Calculate(z, frc::SmartDashboard::GetNumber("Setpoint", 1));

  double angle = m_gyro.GetAngle();

  frc::SmartDashboard::PutNumber("Z", z);
  frc::SmartDashboard::PutNumber("Y", y);
  frc::SmartDashboard::PutNumber("Output", output);
  frc::SmartDashboard::PutNumber("Angle", angle);

  double speed = signe(y)*output;
  frc::SmartDashboard::PutNumber("speed",speed);
  m_MotorRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
  m_MotorRightFollower.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
  m_MotorRightFollower2.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
  m_MotorLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);

}

void Robot::DisabledInit() {}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}


#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
