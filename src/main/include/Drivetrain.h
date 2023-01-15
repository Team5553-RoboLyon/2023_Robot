#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/Encoder.h>
#include <frc/Doublesolenoid.h>
#include <ctre/Phoenix/motorcontrol/can/TalonFX.h>
#include <frc/Compressor.h>

class Drivetrain : public frc2::SubsystemBase
{
public:
  Drivetrain();
  void Set(double speed);                                                                                       // faire autre chose y a moyen
  double GetSwitchGearVoltageRight(double w, double voltageReference, double reduction, double finalReduction); // voir avec JM
  double GetSwitchGearVoltageLeft(double w, double voltageReference, double reduction, double finalReduction);  // voir avec JM                                                                // ok
  void InvertBallShifter();
  void ActiveBallShifterV1();                                                   // ok
  void ActiveBallShifterV2();                                                   // ok                                                                                    // ok
  void EnableBrakeMode(bool Change);                                            // ok
  void Drive(double joystickLeft, double joystickRight);                        // ok
  double Calcul_De_Notre_Brave_JM(double forward, double turn, bool wheelSide); // Si wheelSide 0: roue droite / Si wheelSide 1: roue gauche

  double w;
  double T;
  double Ts = 4.69;
  double wfRef = 6380;
  double m_count = 0;
  double m_coun = 0;

  enum State
  {
    Pilote_V1,
    Auto_V1toV2,
    Auto_V1toV2_Inter,
    Pilote_V2,
    Auto_V2toV1,
    Auto_V2toV1_Inter
  };
  Drivetrain::State m_state = Drivetrain::State::Pilote_V1;

private:
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxRight1{1};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxRight2{2};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxRight3{3};

  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxLeft1{4};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxLeft2{5};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxLeft3{6};

  // frc::Encoder m_EncoderRight{0, 1};
  // frc::Encoder m_EncoderLeft{2, 3};

  frc::DoubleSolenoid m_BallShifterSolenoidLeft{frc::PneumaticsModuleType::REVPH, 0, 1};
};