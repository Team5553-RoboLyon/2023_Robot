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

void Robot::RobotInit() {
  // frc::ADXRS450_Gyro(frc::SPI::Port::kOnboardCS0);
  m_gyro.Reset();
  m_gyro.Calibrate();
}
void Robot::RobotPeriodic() {}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {
  frc::SmartDashboard::PutNumber("P", 1.5);
  frc::SmartDashboard::PutNumber("I", 0);
  frc::SmartDashboard::PutNumber("D", 0);
  frc::SmartDashboard::PutNumber("Setpoint", 1);
}
void Robot::TeleopPeriodic() {
  m_pidController.SetP(frc::SmartDashboard::GetNumber("P", 1.5));
  m_pidController.SetI(frc::SmartDashboard::GetNumber("I", 0));
  m_pidController.SetD(frc::SmartDashboard::GetNumber("D", 0));
  double z = std::abs(m_accelerometer.GetX()); // -1/1
  double y =m_accelerometer.GetY();

  double output = m_pidController.Calculate(z, frc::SmartDashboard::GetNumber("Setpoint", 1));


  frc::SmartDashboard::PutNumber("Z", z);
  frc::SmartDashboard::PutNumber("Y", y);
  frc::SmartDashboard::PutNumber("Output", output);
  frc::SmartDashboard::PutNumber("Angle", y);
  double angle = m_gyro.GetAngle();
  frc::SmartDashboard::PutNumber("Angle", angle);
  // frc::SmartDashboard::PutNumber("arcos accélé",std::acos(y)/3.14159265*180.0);

  double speed = signe(y)*output;
  frc::SmartDashboard::PutNumber("speed",speed);
  // if (m_joystick.GetRawButton(1))
  // {
  //   m_MotorRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
  //   m_MotorRightFollower.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
  //   m_MotorRightFollower2.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
  //   m_MotorLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
  //   m_MotorLeftFollower.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
  //   m_MotorLeftFollower2.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
  // }
  // else
  // {
  //   m_MotorRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0.0);
  //   m_MotorRightFollower.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0.0);
  //   m_MotorRightFollower2.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0.0);
  //   m_MotorLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0.0);
  //   m_MotorLeftFollower.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0.0);
  //   m_MotorLeftFollower2.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0.0);
  // }
  

}

void Robot::DisabledInit() {
  m_gyro.Reset();
}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}


#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
