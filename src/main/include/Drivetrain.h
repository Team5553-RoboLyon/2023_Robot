#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/Encoder.h>
#include <frc/Doublesolenoid.h>
#include <ctre/Phoenix/motorcontrol/can/TalonFX.h>
#include <lib/NRollingAverage.h>
#include <lib/rate_limiter.h>
#include <lib/NLCsv.h>
#include <frc/Compressor.h>
#include <ostream>
#include <fstream>

class Drivetrain : public frc2::SubsystemBase
{
public:
  Drivetrain();
  void Set(double speed);                                                                                       // faire autre chose y a moyen
  double GetSwitchGearVoltage(double w); // voir avec JM                                                              // ok
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
  void SwitchUp(double w);
  void SwitchDown(double w);


  double m_EncoderRight_V; // vitesses encodeur droit 
  double m_EncoderLeft_V; // vitesses encodeur gauche

  double m_EncoderRightLast_V; // ancienne vitesses encodeur droit
  double m_EncoderLeftLast_V; // ancienne vitesses encodeur gauche

  double m_MotorRight_V; // vitesses moteur droit 
  double m_MotorLeft_V; // vitesses moteur gauche 
 
  double m_MotorRightLast_V; // ancienne vitesses moteur droit 
  double m_MotorLeftLast_V;  // ancienne vitesses moteur gauche 

  double m_RobotRight_W; // vitesses droit mixé entre encodeur moteur et axe
  double m_RobotLeft_W; // vitesses gauche mixé entre encodeur moteur et axe

  double m_MotorAccelerationRight; // accélération moteur droit
  double m_MotorAccelerationLeft; // accélération moteur gauche

  double m_EncoderAccelerationRight; // accélération encodeur droit
  double m_EncoderAccelerationLeft; // accélération encodeur gauche

  double m_RobotAccelerationRight; // accélération robot droit mixé entre encodeur moteur et axe
  double m_RobotAccelerationLeft; // accélération robot gauche mixé entre encodeur moteur et axe

  double m_Robot_W; // vitesse robot moyenne entre les deux cotés
  double m_RobotAcceleration; // accélération robot moyenne entre les deux cotés

  double m_SwitchTimeLock; // temps de blocage du changement de vitesse
  double m_Robot_Angular_Rotate; // vitesse angulaire du robot

  double m_Joystick_V_Last; // ancienne valeur joystick V
  
  double m_Joystick_V_Acceleration; // accélération joystick V

  double m_Joystick_V_Limited; // joystick V limité
  double m_Joystick_W_Limited; // joystick W limité

  double m_Joystick_V_Pure; // joystick V pur
  double m_Joystick_W_Pure; // joystick W pur

  double m_SwitchSpeed; // vitesse de changement de vitesse
  double m_Recul; // recul du robot

  double m_Etat; // état du robot



  enum class State
  {
    lowGear,
    highGear
  };

  State m_State;

  RateLimiter m_rateLimiter_V_Fast; // rate limiter joystick V
  RateLimiter m_rateLimiter_V_Slow; // rate limiter du rate limiter joystick V

  RateLimiter m_rateLimiter_W_Fast; // rate limiter joystick W
  RateLimiter m_rateLimiter_W_Slow; // rate limiter du rate limiter joystick W

  NLCSV m_logCSV{8}; // log csv



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