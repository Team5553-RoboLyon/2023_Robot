// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

double Robot::Calcul_De_Notre_Brave_JM(double forward, double turn, bool wheelSide) // calcule la vitesse des roues
{
    double m_forward = forward;
    double m_turn = turn;

    double left_wheel = m_forward + m_turn * SIGMA;
    double right_wheel = m_forward - m_turn * SIGMA;

    double k;
    k = 1.0 / (NMAX(1, NMAX(NABS(left_wheel), NABS(right_wheel))));
    left_wheel *= k;
    right_wheel *= k;

    if (wheelSide == 0)
        return right_wheel;
    else
        return left_wheel;
}

void Robot::RobotInit() {
  frc::ADXRS450_Gyro(frc::SPI::Port::kOnboardCS0);
  m_gyro.Reset(); 
  m_gyro.Calibrate();
}
void Robot::RobotPeriodic() {}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {
  frc::SmartDashboard::PutNumber("P", 0);
  frc::SmartDashboard::PutNumber("I", 0);
  frc::SmartDashboard::PutNumber("D", 0);
  frc::SmartDashboard::PutNumber("Setpoint", 1);
  frc::SmartDashboard::PutNumber("m_tau", 0.075);

  m_MotorLeft.SetInverted(true);
  m_MotorLeftFollower.SetInverted(true);
  m_MotorLeftFollower2.SetInverted(true);

  m_MotorRight.SetInverted(false);
  m_MotorRightFollower.SetInverted(false);
  m_MotorRightFollower2.SetInverted(false);

  m_MotorLeftFollower.Follow(m_MotorLeft);
  m_MotorLeftFollower2.Follow(m_MotorLeft);

  m_MotorRightFollower.Follow(m_MotorRight);
  m_MotorRightFollower2.Follow(m_MotorRight);



  m_MotorLeft.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
  m_MotorLeftFollower.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
  m_MotorLeftFollower2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);

  m_MotorRight.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
  m_MotorRightFollower.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
  m_MotorRightFollower2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);

}
void Robot::TeleopPeriodic() {

  m_pidController.SetP(frc::SmartDashboard::GetNumber("P", 0));
  m_pidController.SetI(frc::SmartDashboard::GetNumber("I", 0));
  m_pidController.SetD(frc::SmartDashboard::GetNumber("D", 0));

  double x = m_accelerometer.GetX(); // -1/1
  double y =m_accelerometer.GetY();
  double z =m_accelerometer.GetZ();


  double angle = m_gyro.GetAngle();
  m_AccelerometerX_Average.add(x);
  double arcos_X = std::acos(m_AccelerometerX_Average.get())/3.14159265*180.0;
  m_AccelerometerX_Arcos_Average.add(arcos_X);

  m_AccelerometerX.set(x);
  m_AccelerometerY.set(y);
  m_Gyro_Angle.set(angle);

  m_FusAngle.Update(m_gyro.GetRate()/180.0*3.14159265,m_accelerometer.GetY(),m_accelerometer.GetX());
  m_FusAngle.SetTau(frc::SmartDashboard::GetNumber("m_tau",0.075));

  frc::SmartDashboard::PutNumber("FuseAngle",m_FusAngle.GetAngle()*180.0/3.14159265);
  frc::SmartDashboard::PutNumber("m_K",m_FusAngle.m_k);
  frc::SmartDashboard::PutNumber("m_dt",m_FusAngle.m_dt);
  frc::SmartDashboard::PutNumber("m_a",m_FusAngle.m_a);
  frc::SmartDashboard::PutNumber("m_angleAccel",m_FusAngle.m_angleAccel);

  frc::SmartDashboard::PutNumber("Accel_X",x);
  frc::SmartDashboard::PutNumber("Accel_Y",y);
  frc::SmartDashboard::PutNumber("Accel_Z",z);
  frc::SmartDashboard::PutNumber("Angle_Gyro",angle);
  frc::SmartDashboard::PutNumber("Angle_Accel_X",arcos_X);








  double output = m_pidController.Calculate(std::abs(x), frc::SmartDashboard::GetNumber("Setpoint", 1));
  double speed = signe(m_FusAngle.GetAngle())*std::clamp(output,-0.3,0.3);


  if (m_joystickRight.GetRawButton(1))
  {
    m_MotorRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
    m_MotorLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);

  }
  else
  {
    m_MotorRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(-m_joystickLeft.GetY(), m_joystickRight.GetZ(), 0));
    m_MotorLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(-m_joystickLeft.GetY(), m_joystickRight.GetZ(), 1));

  }
  if (m_joystickLeft.GetRawButton(1))
  {
    m_pidController.Reset();
  }

  frc::SmartDashboard::PutNumber("speed",speed);
  frc::SmartDashboard::PutNumber("Output", output);
  frc::SmartDashboard::PutNumber("error",m_pidController.GetPositionError());

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
