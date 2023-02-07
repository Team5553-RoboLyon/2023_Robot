#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/Encoder.h>
#include <frc/Doublesolenoid.h>
#include <ctre/Phoenix/motorcontrol/can/TalonFX.h>
#include <frc/Compressor.h>
#include <lib/NLCsv.h>
#include <ostream>
#include <fstream>

class Drivetrain : public frc2::SubsystemBase
{
public:
  Drivetrain();
  void Set(double speed);                                                                                       // faire autre chose y a moyen                                                          // ok                                                       // ok
  void Drive(double joystickLeft, double joystickRight);                        // ok
  double Calcul_De_Notre_Brave_JM(double forward, double turn, bool wheelSide); // Si wheelSide 0: roue droite / Si wheelSide 1: roue gauche
  void SetBallShifterV1();

  double m_EncoderRightLast;
  double m_EncoderLeftLast;
  double m_EncoderRightCurrent;
  double m_EncoderLeftCurrent;

private:
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxRight1{1};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxRight2{2};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxRight3{3};

  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxLeft1{4};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxLeft2{5};
  ctre::phoenix::motorcontrol::can::TalonFX m_MotorGearboxLeft3{6};

  frc::Encoder m_EncoderRight{0, 1,true};
  frc::Encoder m_EncoderLeft{2, 3,false};

  frc::Compressor m_Compressor{1, frc::PneumaticsModuleType::REVPH};
  frc::DoubleSolenoid m_BallShifter{frc::PneumaticsModuleType::REVPH, 0, 1};

};