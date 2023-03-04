// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include "rev/CANSparkMax.h"
#include <frc/Encoder.h>
#include "lib/Pid.h"
#include "lib/RblUtils.h"

class Elevator : public frc2::SubsystemBase
{
public:
  Elevator();

  void SetSetpoint(double setpoint);
  void Periodic() override;
  void SetGains(double p, double i, double d);
  double GetEncoder();
  void Set(double speed);

  Pid m_elevatorPid{0, 2.25, 0.0, 0.0};
  // Pid m_elevatorPid{0, 0.0, 0.0, 0.0};

private:
  rev::CANSparkMax m_elevatorMotor{8, rev::CANSparkMaxLowLevel::MotorType::kBrushless};
  frc::Encoder m_elevatorEncoder{1, 2, true};
};
