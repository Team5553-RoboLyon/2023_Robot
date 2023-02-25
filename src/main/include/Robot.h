// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/TimedRobot.h>
#include "rev/CANSparkMax.h"
#include <frc/Joystick.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Encoder.h>
#include <frc/doublesolenoid.h>
#include <frc/Compressor.h>

class Robot : public frc::TimedRobot
{
public:
  void RobotInit() override;
  void RobotPeriodic() override;

  void AutonomousInit() override;
  void AutonomousPeriodic() override;

  void TeleopInit() override;
  void TeleopPeriodic() override;

  void DisabledInit() override;
  void DisabledPeriodic() override;

  void TestInit() override;
  void TestPeriodic() override;

  void SimulationInit() override;
  void SimulationPeriodic() override;

  void ChangePosition();

private:
  rev::CANSparkMax m_motor{1, rev::CANSparkMax::MotorType::kBrushless};
  frc::Joystick m_stick{0};
  frc::Encoder m_encoder{0, 1};
  frc::DoubleSolenoid m_solenoid{frc::PneumaticsModuleType::REVPH, 12, 13};
  frc::Compressor m_compressor{frc::PneumaticsModuleType::REVPH};
  double m_clamp;
  double m_lastDistance;
  double m_encoderGetDistance;
  double m_vitesse;
};
