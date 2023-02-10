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
  //frc::ADXRS450_Gyro(frc::SPI::Port::kOnboardCS0);
  m_gyro.Reset(); 
  m_gyro.Calibrate();
}
void Robot::RobotPeriodic() {}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {
  // frc::SmartDashboard::PutNumber("P", 0);
  // frc::SmartDashboard::PutNumber("I", 0);
  // frc::SmartDashboard::PutNumber("D", 0);
  frc::SmartDashboard::PutNumber("Setpoint", 1);

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

  if (m_Gyro_Angle.m_delta > bruit)
  {
    m_Sum_Delta_Gyro_Angle += m_Gyro_Angle.m_delta;
  }
  else if (m_Gyro_Angle.m_delta < bruit)
  {
    m_Sum_Delta_Gyro_Angle += m_Gyro_Angle.m_delta;
  }  


 frc::SmartDashboard::PutNumber("Accel_X",x);
 frc::SmartDashboard::PutNumber("Accel_Y",y);
 frc::SmartDashboard::PutNumber("Accel_Z",z);
 frc::SmartDashboard::PutNumber("Angle_Gyro",angle);
 frc::SmartDashboard::PutNumber("Accel_X_Average",m_AccelerometerX_Average.get());
 frc::SmartDashboard::PutNumber("Accel_Arcos_X",arcos_X);
 frc::SmartDashboard::PutNumber("Accel_Arcos_X_Average",m_AccelerometerX_Arcos_Average.get());
 frc::SmartDashboard::PutNumber("Current_Dynamic_Accel_X",m_AccelerometerX.m_current);
 frc::SmartDashboard::PutNumber("Current_Dynamic_Accel_Y",m_AccelerometerY.m_current);
 frc::SmartDashboard::PutNumber("Current_Dynamic_Gryo",m_Gyro_Angle.m_current);
 frc::SmartDashboard::PutNumber("Delta_Dynamic_Accel_X",m_AccelerometerX.m_delta);
 frc::SmartDashboard::PutNumber("Delta_Dynamic_Accel_Y",m_AccelerometerY.m_delta);
 frc::SmartDashboard::PutNumber("Delta_Dynamic_Gryo",m_Gyro_Angle.m_delta);
 frc::SmartDashboard::PutNumber("Sum_Delta_Gyro_Angle",m_Sum_Delta_Gyro_Angle);
 frc::SmartDashboard::PutNumber("Signe",signe(m_Sum_Delta_Gyro_Angle));








  double output = m_pidController.Calculate(std::abs(x), frc::SmartDashboard::GetNumber("Setpoint", 1));
  double speed = signe(m_Sum_Delta_Gyro_Angle)*std::clamp(output,-0.3,0.3);


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
    m_gyro.Reset();
  }

  frc::SmartDashboard::PutNumber("speed",speed);
  // frc::SmartDashboard::PutNumber("Z", z);
  // frc::SmartDashboard::PutNumber("Y", y);
  // frc::SmartDashboard::PutNumber("X",x);
  // frc::SmartDashboard::PutNumber("m_AccelerometerX_Average",m_AccelerometerX_Average.get());
  // frc::SmartDashboard::PutNumber("m_AccelerometerX_arcos_Average",m_AccelerometerX_Arcos_Average.get());
  frc::SmartDashboard::PutNumber("Output", output);
  frc::SmartDashboard::PutNumber("error",m_pidController.GetPositionError());
  // frc::SmartDashboard::PutNumber("Angle", angle);
  // frc::SmartDashboard::PutNumber("arcos accelerometer X average",arcos_X);

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
