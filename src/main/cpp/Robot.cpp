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

  m_PidController.SetTolerance(1.5);
  m_state=State::End;
  m_EncoderLeft.SetDistancePerPulse(1.0/2048.0);


}
void Robot::TeleopPeriodic() {


  



  double a=0.0;
  double ratio_distance=0.0;


  double x = m_accelerometer.GetX(); // -1/1
  double y =m_accelerometer.GetY();

  double angle = m_gyro.GetAngle();

  m_AccelerometerX.set(x);
  m_AccelerometerY.set(y);
  m_Gyro_Angle.set(angle);

  m_FusAngle.Update(m_gyro.GetRate()/180.0*3.14159265,m_accelerometer.GetY(),m_accelerometer.GetX());
  m_FusAngle.SetTau(frc::SmartDashboard::GetNumber("m_tau",0.5));

  // frc::SmartDashboard::PutNumber("FuseAngle",m_FusAngle.GetAngle()*180.0/3.14159265);
  // frc::SmartDashboard::PutNumber("m_K",m_FusAngle.m_k);
  // frc::SmartDashboard::PutNumber("Accel_X",x);
  // frc::SmartDashboard::PutNumber("Accel_Y",y);
  // frc::SmartDashboard::PutNumber("Angle_Gyro",angle);


  double output = m_PidController.Calculate(m_FusAngle.GetAngle()*180.0/3.14159265);
  double speed = std::clamp(output,-0.3,0.3);

  if (m_joystickRight.GetRawButton(1))
  {
      m_distanceAparcourir = CHARGE_STATION_WIDTH;// à mettre
      m_distanceParcourue = 0.0;
      m_distanceRestante=m_distanceAparcourir-m_distanceParcourue;
      m_encoder_origine=abs(m_EncoderLeft.GetDistance()*(3.0*0.0254)*(2.0*3.14159265));
      m_PidController.Reset();
      m_PidController.SetSetpoint(0);
      m_signe_error = signe(m_PidController.m_error);
      m_state= State::Adjusting;
  }
    frc::SmartDashboard::PutNumber("m_distanceParcourue",m_distanceParcourue);
    frc::SmartDashboard::PutNumber("m_distanceRestante",m_distanceRestante);
switch (m_state)
    {
    case  Adjusting:
      std::cout<<("Adjusting")<<std::endl;
      if (signe(m_PidController.m_error)!=m_signe_error)
      {
        m_signe_error=-m_signe_error;
        m_distanceAparcourir=m_distanceParcourue/2.0;
        m_kPmax=m_kPmin+(m_kPmax-m_kPmin)*0.9;

        
        m_distanceParcourue=0.0;
        m_encoder_origine=NABS(m_EncoderLeft.GetDistance()*(3.0*0.0254)*(2.0*3.14159265));
      }

      m_distanceParcourue = NABS(m_EncoderLeft.GetDistance()*(3.0*0.0254)*(2.0*3.14159265))-m_encoder_origine;
      m_distanceRestante =m_distanceAparcourir-m_distanceParcourue;
      
      ratio_distance = NCLAMP(0.0,m_distanceRestante,m_distanceAparcourir)/m_distanceAparcourir;
      a = NLERP(m_kPmin,m_kPmax,ratio_distance);
      m_PidController.SetGains(a,0.0,0.0);
    
      break;
    }


    frc::SmartDashboard::PutNumber("a",a);
    frc::SmartDashboard::PutNumber("ratio_distance",ratio_distance);

  if (m_joystickLeft.GetRawButton(1))
  {
   
    m_MotorRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
    m_MotorLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
  }
  else
  {
    m_MotorRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(-m_joystickLeft.GetY(), m_joystickRight.GetZ(), 0));
    m_MotorLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(-m_joystickLeft.GetY(), m_joystickRight.GetZ(), 1));
      
  }
    
    

  frc::SmartDashboard::PutNumber("speed",speed);
  frc::SmartDashboard::PutNumber("Output", output);

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
