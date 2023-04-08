// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/Command.h>
#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Joystick.h>
#include <ctre/phoenix/motorcontrol/can/TalonSRX.h>
#include <frc/DoubleSolenoid.h>
#include <frc/Compressor.h>
#include <rev/CANSparkMax.h>

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

private:
  frc::Joystick m_joystick{0};
  rev::CANSparkMax m_motorRight{ 14,rev::CANSparkMaxLowLevel::MotorType::kBrushless};
  double m_speed = 0.0;
};
