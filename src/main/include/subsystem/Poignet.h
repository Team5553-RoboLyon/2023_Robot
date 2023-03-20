// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include "ctre/phoenix/motorcontrol/can/TalonSRX.h"
#include "Constants.h"
#include "lib/rate_limiter.h"
#include "lib/Pid.h"
#include <frc/Encoder.h>
#include "lib/RblUtils.h"

class Poignet : public frc2::SubsystemBase
{
public:
  Poignet();
  void SetSetpoint(double setpoint);
  void Periodic() override;
  void SetGains(double p, double i, double d);
  double GetEncoder();

  RateLimiter m_PoignetPidRate;
  Pid m_poignetPid{0, P_POIGNET, I_POIGNET, D_POIGNET};

private:
  ctre::phoenix::motorcontrol::can::TalonSRX m_poignetMotor{ID_MOTOR_POIGNET};
  frc::Encoder m_poignetEncoder{ID_ENCODER_POIGNET_A, ID_ENCODER_POIGNET_B, true};
};
