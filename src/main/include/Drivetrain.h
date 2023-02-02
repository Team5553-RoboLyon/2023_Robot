#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/Encoder.h>
#include <frc/Doublesolenoid.h>
#include <ctre/Phoenix/motorcontrol/can/TalonFX.h>
#include <lib/NRollingAverage.h>
#include <lib/rate_limiter.h>
#include <frc/Compressor.h>
#include <ostream>
#include <fstream>

class Drivetrain : public frc2::SubsystemBase
{
public:
  Drivetrain();
  void Set(double speed);                                                                                       // faire autre chose y a moyen
  double GetSwitchGearVoltageRight(double w); // voir avec JM
  double GetSwitchGearVoltageLeft(double w);  // voir avec JM                                                                // ok
  void InvertBallShifter();
  void ActiveBallShifterV1();                                                   // ok
  void ActiveBallShifterV2();               
  void EnableBrakeMode(bool Change);                                            // ok
  void Drive(double joystickLeft, double joystickRight);                        // ok
  double Calcul_De_Notre_Brave_JM(double forward, double turn, bool wheelSide); // Si wheelSide 0: roue droite / Si wheelSide 1: roue gauche
  bool General(double switchTimeLock,double w);
  bool Up(double speedRobot, double accelerationRobot, double joystick, double deltaJoystick);
  bool KickDown(double speedRobot, double accelerationRobot, double joystick);
  bool CoastDown(double speedRobot);
  void SetSwitchTimeLock(double switchTimeLock);
  void SetVoltageTarget(double voltageTarget,double state);
  double SwitchUpRight(double coeff, double target,double w);
  double SwitchUpLeft(double coeff, double target,double w);
  double SwitchDownRight(double coeff, double target,double w);
  double SwitchDownLeft(double coeff, double target,double w);


  double m_SpeedEncoderRight;
  double m_SpeedEncoderLeft;

  double m_SpeedEncoderRightLast;
  double m_SpeedEncoderLeftLast;

  double m_SpeedMotorRight;
  double m_SpeedMotorLeft;

  double m_SpeedMotorRightLast;
  double m_SpeedMotorLeftLast;

  double m_SpeedRobotRight;
  double m_SpeedRobotLeft;

  double m_AccelerationMotorRight;
  double m_AccelerationMotorLeft;

  double m_AccelerationEncoderRight;
  double m_AccelerationEncoderLeft;

  double m_AccelerationRobotRight;
  double m_AccelerationRobotLeft;

  double m_SpeedRobot;
  double m_AccelerationRobot;

  double m_SwitchTimeLock;
  double m_W;

  double m_JoystickRightLast;
  
  double m_Joystick_V_Acceleration;

  double m_SwitchGearVoltageRight;
  double m_SwitchGearVoltageLeft;

  double m_Joystick_V_Limited;
  double m_Joystick_W_Limited;

  double m_Joystick_V_Pure;
  double m_Joystick_W_Pure;



  enum class State
  {
    lowGear,
    highGear
  };

  State m_State;

  RateLimiter m_rateLimiterFast;
  RateLimiter m_rateLimiterSlow;

private:
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxRight1{1};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxRight2{2};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxRight3{3};

  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxLeft1{4};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxLeft2{5};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxLeft3{6};

  frc::Encoder m_EncoderRight{0, 1,true};
  frc::Encoder m_EncoderLeft{2, 3,false};

  frc::DoubleSolenoid m_BallShifterSolenoidLeft{frc::PneumaticsModuleType::REVPH, 0, 1};
};