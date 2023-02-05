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
#include <lib/MoveData.h>

#define REDUC_V1 13.3
#define REDUC_V2 8.8
#define T_SWITCH 0.5
#define VOLTAGE_REF 12.0                                    // tension de référence
#define RESIST_TORQUE_NM 0.013   // hop plus de frottements
#define MAXSWITCHTIMELOCK 0.5                              // temps max pour le switch de vitesse
#define AXLETRACK 0.5098 // distance entre les roues
#define HALF_TRACKWIDTH (AXLETRACK / 2.0f)
#define TRUST_GEARBOX_OUT_ENCODER 0.7
#define WF_MOTOR_RPM 6380     // vitesse de rotation théorique du moteur
#define TICK_DT 0.02 // durée d'un tick en seconde
#define SIGMA 0.5          // sigma pour le rate limiter
#define AVERAGE_SAMPLES_NUMBER 5 // nombre de samples pour la moyenne



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
  bool General(double switchTimeLock,double GearboxRightRpm, double GearboxLeftRpm);
  bool Up(double speedRobot, double accelerationRobot, double joystick, double deltaJoystick);
  bool KickDown(double speedRobot, double accelerationRobot, double joystick);
  bool CoastDown(double speedRobot);
  void SetSwitchTimeLock(double switchTimeLock);
  void SetVoltageTarget(double voltageTarget,double state);
  void SwitchUp(double w);
  void SwitchDown(double w);



  MoveData m_GearboxRightOutRpt; // encodeur droit en en tours/tick (RPT)
  MoveData m_GearboxLeftOutRpt; // encodeur gauche

  MoveData m_SuperMotorRightRpm; // moteur droit
  MoveData m_SuperMotorLeftRpm; // moteur gauche

  NdoubleRollingAverage m_GearboxRightAveragedRpt; 
  NdoubleRollingAverage m_GearboxLeftAveragedRpt;

  NdoubleRollingAverage m_SuperMotorLeftAveragedRpm;
  NdoubleRollingAverage m_SuperMotorRightAveragedRpm;

  double m_GearboxOutRightRpm; // vitesses droit mixé entre encodeur moteur et axe
  double m_GearboxOutLeftRpm; // vitesses gauche mixé entre encodeur moteur et axe

  double m_RobotAccelerationRight; // accélération robot droit mixé entre encodeur moteur et axe
  double m_RobotAccelerationLeft; // accélération robot gauche mixé entre encodeur moteur et axe

  double m_Gearboxes_Acceleration; // accélération robot moyenne entre les deux cotés

  double m_SwitchTimeLock; // temps de blocage du changement de vitesse
  double m_Robot_W; // vitesse angulaire du robot

  double m_Joystick_V_Last; // ancienne valeur joystick V
  
  double m_Joystick_V_Acceleration; // accélération joystick V

  double m_Joystick_V_Limited; // joystick V limité
  double m_Joystick_W_Limited; // joystick W limité

  double m_Joystick_V_Pure; // joystick V pur
  double m_Joystick_W_Pure; // joystick W pur

  double m_SwitchSpeed; // vitesse de changement de vitesse
  double m_Recul; // recul du robot

  double m_CurrentGearboxReductionFactor; // Reduction de la vitesse engagée dans la  boite (V1 ou V2)



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

  //NLCSV m_logCSV{8}; // log csv



private:
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorRight1{1};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorRight2{2};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorRight3{3};

  ctre::phoenix::motorcontrol::can::TalonFX m_MotorLeft1{4};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorLeft2{5};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorLeft3{6};

  frc::Encoder m_EncoderRight{0, 1,true};
  frc::Encoder m_EncoderLeft{2, 3,false};

  frc::DoubleSolenoid m_BallShifterSolenoidLeft{frc::PneumaticsModuleType::REVPH, 0, 1};
};