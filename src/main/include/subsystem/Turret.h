// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include "rev/CANSparkMax.h"
#include <frc/Encoder.h>
#include "lib/Pid.h"

class Turret : public frc2::SubsystemBase
{
public:
  Turret();

  void SetSetpoint(double setpoint);
  void Periodic() override;
  void SetGains(double p, double i, double d);
  double GetEncoder();

private:
  rev::CANSparkMax m_turretMotor{7, rev::CANSparkMaxLowLevel::MotorType::kBrushless};
  frc::Encoder m_turretEncoder{0, 21};
  // Pid m_turretPid{0, 0.1, 0.1, 0.1};
  Pid m_turretPid{0, 0.0, 0.0, 0.0};
};
