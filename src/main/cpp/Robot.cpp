// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "lib/RblUtils.h"
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

void Robot::RobotInit()
{
  m_gyro.Reset();
  m_gyro.Calibrate();
}
void Robot::RobotPeriodic() {}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit()
{
  frc::SmartDashboard::PutNumber("P", 0.008);
  frc::SmartDashboard::PutNumber("I", 0.0);
  frc::SmartDashboard::PutNumber("D", 0.05);
  frc::SmartDashboard::PutNumber("m_tau", 0.5);

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

  m_EncoderLeft.SetDistancePerPulse(1.0 / 2048.0);
  m_EncoderRight.SetDistancePerPulse(1.0 / 2048.0);

  // m_logCSV.open("/home/lvuser/", true);
  // m_logCSV.setItem(0, "m_FusAngle",5, &m_LogFusAngle);
  // m_logCSV.setItem(1, "m_gyroRateAverage",5, &m_LogGyroRateAverage);
  // m_logCSV.setItem(2, "m_gyrorate",5, &m_LogGyroRate);
  // m_logCSV.setItem(3, "m_AngleController.m_error",5, &m_AngleController.m_error);
  // m_logCSV.setItem(4, "m_VangleController.m_error",5, &m_VangleController.m_error);
  // m_logCSV.setItem(5, "m_vOutput",5, &m_vOutput);
  // m_logCSV.setItem(6, "m_AngleOutput",5, &m_AngleOutput);
  // m_logCSV.setItem(7, "m_Output",5, &m_Output);
  // m_logCSV.setItem(8, "m_EncoderMetre",5, &m_LogEncoderM);
  // m_logCSV.setItem(9, "m_AccelX",5, &m_LogAccelX);
  // m_logCSV.setItem(10, "m_AngleAccel",5, &m_LogAngleAccel);
}
void Robot::TeleopPeriodic()
{

  double x_g = m_accelerometer.GetX(); // -1/1
  double y_g = m_accelerometer.GetY();
  double angle_deg = m_gyro.GetAngle();
  double gyro_rate_degps = m_gyro.GetRate();

  m_AccelerometerX.set(x_g);
  m_AccelerometerY.set(y_g);
  m_GyroAngle.set(angle_deg);

  m_FusAngle.SetTau(frc::SmartDashboard::GetNumber("m_tau", 0.5));
  m_FusAngle.Update(NDEGtoRAD(gyro_rate_degps), y_g, x_g); // A la place de  ... m_FusAngle.Update(m_gyro.GetRate()/180.0*3.14159265,m_accelerometer.GetY(),m_accelerometer.GetX());
  m_gyroRateAverage.add(gyro_rate_degps);                  // m_gyroRateAverage.add(m_FusAngle.GetDelta()*180.0/3.14159265);

  frc::SmartDashboard::PutNumber("FuseAngle", NRADtoDEG(m_FusAngle.GetAngle()));
  frc::SmartDashboard::PutNumber("GyroRate", m_gyroRateAverage.get());

  if (m_joystickRight.GetRawButton(1))
  {
    m_FusAngle.AutoSetBias();

    m_AngleController.Reset();
    m_AngleController.SetSetpoint(0.0);
    m_AngleController.SetTolerance(6);

    m_VangleController.Reset();
    m_VangleController.SetSetpoint(0.0);
    m_VangleController.SetTolerance(4);
  }
  double p = frc::SmartDashboard::GetNumber("P", 0.008);
  double i = frc::SmartDashboard::GetNumber("I", 0.0);
  double d = frc::SmartDashboard::GetNumber("D", 0.05);

  m_AngleController.SetGains(p, i, d); // avant m_AngleController.SetGains(a,0.0,0.0);
  m_VangleController.SetGains(p * 0.0, i * 0.0, d * 0.0);
  m_AngleOutput = m_AngleController.Calculate(NRADtoDEG(m_FusAngle.GetAngle()));
  m_vOutput = m_VangleController.Calculate(m_gyroRateAverage.get());
  m_Output = m_vOutput + m_AngleOutput;

  m_Output = NCLAMP(-0.5, m_Output, 0.5);
  m_signeOutput = NSIGN(m_Output);
  m_Output = NMAX(NABS(m_Output), 0.0);
  m_Output = m_signeOutput * m_Output;
  frc::SmartDashboard::PutNumber("error", m_AngleController.m_error);
  frc::SmartDashboard::PutNumber("delta_error", m_VangleController.m_error);
  frc::SmartDashboard::PutNumber("m_Output", m_Output);
  frc::SmartDashboard::PutNumber("m_vOutput", m_vOutput);
  frc::SmartDashboard::PutNumber("m_AngleOutput", m_AngleOutput);

  if (m_joystickLeft.GetRawButton(1))
  {

    m_MotorRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, m_Output);
    m_MotorLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, m_Output);
  }
  else
  {
    m_MotorRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(-m_joystickLeft.GetY(), m_joystickRight.GetZ(), 0));
    m_MotorLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(-m_joystickLeft.GetY(), m_joystickRight.GetZ(), 1));
  }

  // m_LogFusAngle = m_FusAngle.GetAngle();
  // m_LogGyroRateAverage = m_gyroRateAverage.get();
  // m_LogGyroRate = m_gyro.GetRate();
  // m_LogEncoderM = ((m_EncoderLeft.GetDistance()*TRACTION_WHEEL_CIRCUMFERENCE)+(m_EncoderRight.GetDistance()*TRACTION_WHEEL_CIRCUMFERENCE))/2;
  // m_LogAngleAccel = m_FusAngle.m_angleAccel;
  // m_LogAccelX = m_accelerometer.GetX();

  // m_logCSV.write();
}

void Robot::DisabledInit()
{
  m_gyro.Reset();
  // m_logCSV.close();
}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main()
{
  return frc::StartRobot<Robot>();
}
#endif
