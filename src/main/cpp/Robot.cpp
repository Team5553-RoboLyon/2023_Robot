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

void Robot::RobotInit() {
  m_gyro.Reset(); 
  m_gyro.Calibrate();

  m_kPmin = 0.0075;
  m_kPmax = 0.02;  
}
void Robot::RobotPeriodic() {}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {
  frc::SmartDashboard::PutNumber("P", 0.016);
  frc::SmartDashboard::PutNumber("I", 0.0);
  frc::SmartDashboard::PutNumber("D", 0.0);
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

  m_state=State::End;
  m_EncoderLeft.SetDistancePerPulse(1.0/2048.0);
  m_EncoderRight.SetDistancePerPulse(1.0/2048.0);

  m_logCSV.open("/home/lvuser/", true);
  m_logCSV.setItem(0, "m_FusAngle",5, &m_LogFusAngle);
  m_logCSV.setItem(1, "m_gyroRateAverage",5, &m_LogGyroRateAverage);
  m_logCSV.setItem(2, "m_gyrorate",5, &m_LogGyroRate);
  m_logCSV.setItem(3, "m_AngleController.m_error",5, &m_AngleController.m_error);
  m_logCSV.setItem(4, "m_VangleController.m_error",5, &m_VangleController.m_error);
  m_logCSV.setItem(5, "m_vOutput",5, &m_vOutput);
  m_logCSV.setItem(6, "m_AngleOutput",5, &m_AngleOutput);
  m_logCSV.setItem(7, "m_Output",5, &m_Output);
  m_logCSV.setItem(8, "m_EncoderMetre",5, &m_LogEncoderM);
  m_logCSV.setItem(9, "m_AccelX",5, &m_LogAccelX);
  m_logCSV.setItem(10, "m_AngleAccel",5, &m_LogAngleAccel);


}
void Robot::TeleopPeriodic() {
  double a      =0.0;
  double ratio_distance=0.0;


  double x_g              = m_accelerometer.GetX(); // -1/1
  double y_g              = m_accelerometer.GetY();
  double angle_deg        = m_gyro.GetAngle();
  double gyro_rate_degps  = m_gyro.GetRate();

  m_AccelerometerX.set(x_g);
  m_AccelerometerY.set(y_g);
  m_GyroAngle.set(angle_deg);

  m_FusAngle.SetTau(frc::SmartDashboard::GetNumber("m_tau",0.5));
  m_FusAngle.Update(NDEGtoRAD(gyro_rate_degps),y_g,x_g);    // A la place de  ... m_FusAngle.Update(m_gyro.GetRate()/180.0*3.14159265,m_accelerometer.GetY(),m_accelerometer.GetX());
  m_gyroRateAverage.addSample(gyro_rate_degps); //m_gyroRateAverage.add(m_FusAngle.GetDelta()*180.0/3.14159265);

  frc::SmartDashboard::PutNumber("FuseAngle",NRADtoDEG(m_FusAngle.GetAngle()));
  frc::SmartDashboard::PutNumber("GyroRate",m_gyroRateAverage.getMean());
  frc::SmartDashboard::PutNumber("anglegyro",m_gyro.GetAngle());
  // frc::SmartDashboard::PutNumber("m_K",m_FusAngle.m_k);
  // frc::SmartDashboard::PutNumber("Accel_X",x);
  // frc::SmartDashboard::PutNumber("Accel_Y",y);
  // frc::SmartDashboard::PutNumber("Angle_Gyro",angle);


 

  if (m_joystickRight.GetRawButton(1))
  {
    m_FusAngle.AutoSetBias();
      // reset de la posiition de ref des encodeurs.
      //m_encoderOrigin       = NABS(m_EncoderLeft.GetDistance()*TRACTION_WHEEL_CIRCUMFERENCE);   // a la place de  ... m_encoder_origine=abs(m_EncoderLeft.GetDistance()*(3.0*0.0254)*(2.0*3.14159265));
      //m_refDistance         = CHARGE_STATION_WIDTH;                                             // a la place de  ... m_distanceAparcourir  = CHARGE_STATION_WIDTH;
      // m_traveledDistance    = 0.0;                                                              // a la place de  ... m_distanceParcourue   = 0.0;
                                                                                                // on supprime ....   m_distanceRestante    = m_distanceAparcourir-m_distanceParcourue;
      m_AngleController.Reset();
      m_AngleController.SetSetpoint(0.0);   
      m_AngleController.SetTolerance(4);
                                              // à la place de ... m_AngleController.SetSetpoint(0);
      m_VangleController.Reset();
      m_VangleController.SetSetpoint(0.0); 
      m_VangleController.SetTolerance(1);                                               
      // m_errorSign = NSIGN(m_AngleController.m_error);                                     // à la place de ... m_signe_error = NSIGN(m_AngleController.m_error);
      m_TiltTracker.initialize(m_FusAngle.GetAngle(),(m_EncoderLeft.GetDistance()+m_EncoderRight.GetDistance())*TRACTION_WHEEL_CIRCUMFERENCE/2.0,CHARGE_STATION_WIDTH);
  }
  double p=frc::SmartDashboard::GetNumber("P",0.012);
  double i =frc::SmartDashboard::GetNumber("I",0.0);
  double d =frc::SmartDashboard::GetNumber("D",0.0);

  bool tilt = m_TiltTracker.Update(0.02,(m_EncoderLeft.GetDistance()+m_EncoderRight.GetDistance())*TRACTION_WHEEL_CIRCUMFERENCE/2.0,m_FusAngle.GetAngle(),m_gyroRateAverage.getMean());
  m_AngleController.SetGains(0.03,0,0); // avant m_AngleController.SetGains(a,0.0,0.0);
  m_VangleController.SetGains(0.005,i*10.0,d*10.0);
  m_AngleOutput = m_AngleController.Calculate(NRADtoDEG(m_FusAngle.GetAngle()));
  m_vOutput = m_VangleController.Calculate(m_gyroRateAverage.getMean());
  m_Output =  m_TiltTracker.m_k*m_AngleOutput;// voutput
  
  frc::SmartDashboard::PutNumber("k_anticipation",m_TiltTracker.m_k);
  frc::SmartDashboard::PutNumber("k*angleoutput",m_TiltTracker.m_k*m_AngleOutput);
  frc::SmartDashboard::PutNumber("delta_angle",m_FusAngle.m_delta);
  m_Output = NCLAMP(-0.3,m_Output,0.3);
  frc::SmartDashboard::PutNumber("output",m_Output);
  frc::SmartDashboard::PutNumber("a",a);
  frc::SmartDashboard::PutNumber("error",m_AngleController.m_error);
  frc::SmartDashboard::PutNumber("delta_error",m_VangleController.m_error);
  frc::SmartDashboard::PutNumber("ratio_distance",ratio_distance);
  frc::SmartDashboard::PutNumber("m_encoderRight",m_EncoderRight.GetDistance());
  frc::SmartDashboard::PutNumber("m_encoderLeft",m_EncoderLeft.GetDistance());
  frc::SmartDashboard::PutNumber("encoder",((m_EncoderLeft.GetDistance()*TRACTION_WHEEL_CIRCUMFERENCE)+(m_EncoderRight.GetDistance()*TRACTION_WHEEL_CIRCUMFERENCE))/2);
  frc::SmartDashboard::PutBoolean("tilt",tilt);

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

  frc::SmartDashboard::PutNumber("m_Output", m_Output);
  frc::SmartDashboard::PutNumber("m_vOutput", m_vOutput);
  frc::SmartDashboard::PutNumber("m_AngleOutput", m_AngleOutput);
  m_LogFusAngle = m_FusAngle.GetAngle();
  m_LogGyroRateAverage = m_gyroRateAverage.getMean();
  m_LogGyroRate = m_gyro.GetRate();
  m_LogEncoderM = ((m_EncoderLeft.GetDistance()*TRACTION_WHEEL_CIRCUMFERENCE)+(m_EncoderRight.GetDistance()*TRACTION_WHEEL_CIRCUMFERENCE))/2;
  m_LogAngleAccel = m_FusAngle.m_angleAccel;
  m_LogAccelX = m_accelerometer.GetX();

 
  

  m_logCSV.write();


}


void Robot::DisabledInit() {
  m_gyro.Reset();
  m_logCSV.close();
}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}


#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
