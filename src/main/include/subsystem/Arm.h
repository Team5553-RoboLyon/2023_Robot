// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include "rev/CANSparkMax.h"
#include <frc/Encoder.h>
#include "lib/Pid.h"

class Arm : public frc2::SubsystemBase
{
public:
  Arm();

  void SetSetpoint(double setpoint);
  void Periodic() override;
  void SetGains(double p, double i, double d);
  double GetEncoder();

private:
  rev::CANSparkMax m_armMotor{1, rev::CANSparkMaxLowLevel::MotorType::kBrushless};
  frc::Encoder m_armEncoder{0, 1};
  Pid m_armPid{0, 0.1, 0.1, 0.1};
};
