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
#include <lib/DynamicData.h>

#define VOLTAGE_REF   12.0     // tension de référence
#define MOTOR_WF_RPM  6380.0   // Free Speed théorique du moteur à la tension de reference (12V)
#define MOTOR_TS_NM   4.69     // Stall Torque théorique du moteur à la tension de reference (12V)

#define REDUC_V1 13.3
#define REDUC_V2 8.8

#define TRUST_GEARBOX_OUT_ENCODER 0.7

#define UP_SHIFTING_POINT_JOYSTICK_V                  0.8                     // Valeur minimum du joystick V pour passer en vitesse 2
#define UP_SHIFTING_POINT_JOYSTICK_V_VARIATION        0.0                     // Valeur minimum de la variation (=delta) du joystick V pour passer en vitesse 2
#define UP_SHIFTING_POINT_GEARBOXES_OUT_RPM           (6380.0*0.8/REDUC_V1)   // Valeur minimum de la vitesse de sortie de boites pour passer en vitesse 2
#define UP_SHIFTING_POINT_GEARBOXES_OUT_RPM2          0.0                     // Valeur minimum de l'accel.  de sortie de boites pour passer en vitesse 2

#define KICKDOWN_SHIFTING_POINT_GEARBOXES_OUT_RPM     (6380.0*0.4/REDUC_V1)   // Valeur max de la vitesse de sortie de boites pour retrograder en vitesse 1
#define KICKDOWN_SHIFTING_POINT_JOYSTICK_V            0.6                     // Valeur minimum du joystick V pour retrograder en vitesse 1 afin de re-accelerer fort
#define KICKDOWN_SHIFTING_POINT_JOYSTICK_V_VARIATION  0.2                     // Valeur minimum de la variation (=delta) du joystick V pour retrograder en vitesse 1
#define T_SWITCH 0.5

#define RESIST_TORQUE_NM 0.013   // hop plus de frottements
#define MAXSWITCHTIMELOCK 0.5                              // temps max pour le switch de vitesse

#define AXLETRACK 0.5098 // distance entre les roues
#define HALF_TRACKWIDTH (AXLETRACK / 2.0)


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
  
  bool isUpshiftingAllowed();
  bool isKickdownShiftingAllowed();
  bool isCoastdownShiftingAllowed();
 

  void SetVoltageTarget(double voltageTarget,double state);
  void SwitchUp(double w);
  void SwitchDown(double w);


  // Côté gauche
  DynamicData           m_GearboxLeftOutRawRpt;         // Vitesse instantanée de sortie de boite ( mesurée par le TroughBore Encoder )
  NdoubleRollingAverage m_GearboxLeftOutAveragedRpt;    // Vitesse moyenne de sortie de boite (Moyenne glissante)
  double                m_SuperMotorLeftRawRpm;         // Vitesse instantannée du supermoteur d'entrée de boite ( = Moyenne des vitesses en ticks / 100 ms mesurées par les 3 Encodeurs moteurs de la boite )
  NdoubleRollingAverage m_SuperMotorLeftAveragedRpm;    // Vitesse Moyenne du supermoteur d'entrée de boite (Moyenne glissante)
  double                m_GearboxLeftOutAdjustedRpm;    // Vitesse de la gearbox gauche en RPM ( combinaison linéaire de la vitesse moyenne de sortie de boite et de la vitesse moyenne du supermoteur en entrée de boite)

  // Côté droit
  DynamicData           m_GearboxRightOutRawRpt;        // Vitesse instantanée de sortie de boite ( mesurée par le TroughBore Encoder )
  NdoubleRollingAverage m_GearboxRightOutAveragedRpt;   // Vitesse moyenne de sortie de boite (Moyenne glissante)
  double                m_SuperMotorRightRawRpm;        // Vitesse instantannée du supermoteur d'entrée de boite ( = Moyenne des vitesses en ticks / 100 ms mesurées par les 3 Encodeurs moteurs de la boite )
  NdoubleRollingAverage m_SuperMotorRightAveragedRpm;   // Vitesse Moyenne du supermoteur d'entrée de boite (Moyenne glissante)
  double                m_GearboxRightOutAdjustedRpm;   // Vitesse de la gearbox droite en RPM ( combinaison linéaire de la vitesse moyenne de sortie de boite et de la vitesse moyenne du supermoteur en entrée de boite)
  
  double                m_GearboxesOutAdjustedRpm;      // Vitesse (translation) du robot exprimée en RPM
  NdoubleRollingAverage m_GearboxesOutAccelerationRpm2; // Acceleration (translation) du robot exprimée RPM²
 
  double                m_GearShiftingTimeLock;         // Temps de blocage du changement de vitesse
  double                m_CurrentGearboxRatio;          // Rapport (Reduction) de la vitesse engagée dans la  boite (V1 ou V2)


  DynamicData           m_JoystickRaw_V;
  DynamicData           m_JoystickRaw_W;
 
  double m_Joystick_V_Acceleration; // accélération joystick V

  double m_Joystick_V_Limited; // joystick V limité
  double m_Joystick_W_Limited; // joystick W limité


  double m_SwitchSpeed; // vitesse de changement de vitesse
  double m_Recul; // recul du robot




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