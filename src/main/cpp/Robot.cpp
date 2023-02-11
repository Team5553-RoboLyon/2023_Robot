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
  frc::SmartDashboard::PutNumber("P", 0.0);
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

  m_AngleController.SetTolerance(4);
  m_state=State::End;
  m_EncoderLeft.SetDistancePerPulse(1.0/2048.0);
  m_EncoderRight.SetDistancePerPulse(1.0/2048.0);


}
void Robot::TeleopPeriodic() {
  double a      =0.0;
  double ratio_distance=0.0;


  double x      = m_accelerometer.GetX(); // -1/1
  double y      =m_accelerometer.GetY();
  double angle  = m_gyro.GetAngle();

  m_AccelerometerX.set(x);
  m_AccelerometerY.set(y);
  m_Gyro_Angle.set(angle);
  m_DeltaAngle_Average.add(m_FusAngle.GetDelta()*180.0/3.14159265);

  
  m_FusAngle.SetTau(frc::SmartDashboard::GetNumber("m_tau",0.5));
  m_FusAngle.Update(NDEGtoRAD(m_gyro.GetRate()),m_accelerometer.GetY(),m_accelerometer.GetX());    // A la place de  ... m_FusAngle.Update(m_gyro.GetRate()/180.0*3.14159265,m_accelerometer.GetY(),m_accelerometer.GetX());
 
  // frc::SmartDashboard::PutNumber("FuseAngle",m_FusAngle.GetAngle()*180.0/3.14159265);
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
      m_AngleController.SetSetpoint(0.0);                                                 // à la place de ... m_AngleController.SetSetpoint(0);
      m_VangleController.Reset();
      m_VangleController.SetSetpoint(0.0);                                                
      // m_errorSign = NSIGN(m_AngleController.m_error);                                     // à la place de ... m_signe_error = NSIGN(m_AngleController.m_error);
      m_TiltTracker.initialize(m_EncoderLeft.GetDistance()*TRACTION_WHEEL_CIRCUMFERENCE,m_EncoderRight.GetDistance()*TRACTION_WHEEL_CIRCUMFERENCE,m_FusAngle.GetAngle(),CHARGE_STATION_WIDTH);
      m_state= State::Adjusting;
  }
  
  frc::SmartDashboard::PutNumber("m_traveledDistance",m_traveledDistance);              // à la place de ... frc::SmartDashboard::PutNumber("m_distanceParcourue",m_distanceParcourue);
  frc::SmartDashboard::PutNumber("m_refDistance",m_refDistance);                        // à la place de ... frc::SmartDashboard::PutNumber("m_distanceRestante",m_distanceRestante); 
  switch (m_state)
  {
    case  Adjusting:
      std::cout<<("Adjusting")<<std::endl;
      // if(m_TiltTracker.DetectTiltPoint(m_EncoderLeft.GetDistance()*TRACTION_WHEEL_CIRCUMFERENCE,m_EncoderRight.GetDistance()*TRACTION_WHEEL_CIRCUMFERENCE,m_FusAngle.GetAngle()))
      // {
      // }
      if (NSIGN(m_AngleController.m_error)!=m_errorSign)
      {
        m_errorSign=-m_errorSign;
        m_refDistance=m_traveledDistance/2.0;
        m_kPmax = m_kPmin + (m_kPmax-m_kPmin)*0.9;

        
        m_traveledDistance=0.0;
        m_encoderOrigin=NABS(m_EncoderLeft.GetDistance()*TRACTION_WHEEL_CIRCUMFERENCE);
      }

      m_traveledDistance = NABS(m_EncoderLeft.GetDistance()*TRACTION_WHEEL_CIRCUMFERENCE)-m_encoderOrigin;
      //m_distanceRestante =m_distanceAparcourir-m_distanceParcourue;
      
      a = (m_refDistance - m_traveledDistance)/m_refDistance;
      ratio_distance = NCLAMP(0.0,a,1.0);
     // avant a = NLERP(m_kPmin,m_kPmax,ratio_distance);
      //m_AngleController.SetGains(m_kPmax,0.0,0.0); // avant m_AngleController.SetGains(a,0.0,0.0);
    
      break;

      default:
        break;
  }
  double p=frc::SmartDashboard::GetNumber("P",0.008);
  double i =frc::SmartDashboard::GetNumber("I",0.0001);
  double d =frc::SmartDashboard::GetNumber("D",0.07);
  m_TiltTracker.DetectTiltPoint(m_EncoderLeft.GetDistance()*TRACTION_WHEEL_CIRCUMFERENCE,m_EncoderRight.GetDistance()*TRACTION_WHEEL_CIRCUMFERENCE,m_FusAngle.GetAngle());
  m_AngleController.SetGains(p,i,d); // avant m_AngleController.SetGains(a,0.0,0.0);
  m_VangleController.SetGains(p*8.0,i*10.0,d*10.0);
  double Angleoutput = m_AngleController.Calculate(m_FusAngle.GetAngle()*180.0/3.14159265);
  double Voutput = m_VangleController.Calculate(m_DeltaAngle_Average.get());
  double output = K_ANTICIPATION*Voutput+(1-K_ANTICIPATION)*Angleoutput;
  output = NCLAMP(-0.3,output,0.3);
  frc::SmartDashboard::PutNumber("a",a);
  frc::SmartDashboard::PutNumber("error",m_AngleController.m_error);
  frc::SmartDashboard::PutNumber("delta_error",m_VangleController.m_error);
  frc::SmartDashboard::PutNumber("ratio_distance",ratio_distance);
  frc::SmartDashboard::PutNumber("m_tiltA",m_TiltTracker.m_tiltA);
  frc::SmartDashboard::PutNumber("m_tiltB",m_TiltTracker.m_tiltB);
  frc::SmartDashboard::PutNumber("encoder",(m_EncoderLeft.GetDistance()*TRACTION_WHEEL_CIRCUMFERENCE+m_EncoderRight.GetDistance()*TRACTION_WHEEL_CIRCUMFERENCE)/2);


  if (m_joystickLeft.GetRawButton(1))
  {
   
    m_MotorRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, output);
    m_MotorLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, output);
  }
  else
  {
    m_MotorRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(-m_joystickLeft.GetY(), m_joystickRight.GetZ(), 0));
    m_MotorLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(-m_joystickLeft.GetY(), m_joystickRight.GetZ(), 1));
      
  }

  frc::SmartDashboard::PutNumber("Output", output);
  frc::SmartDashboard::PutNumber("Voutput", Voutput);
  frc::SmartDashboard::PutNumber("Angleoutput", Angleoutput);


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
