// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include "rev/CANSparkMax.h"
#include <frc/Encoder.h>
#include "lib/Pid.h"
#include "lib/RblUtils.h"
#include "Constants.h"
#include "lib/HallSecurity.h"
#include "lib/rate_limiter.h"

class Elevator : public frc2::SubsystemBase
{
public:
  Elevator();

  void SetSetpoint(double setpoint);
  void Periodic() override;
  void SetGains(double p, double i, double d);
  double GetEncoder();
  bool GetEnabled();
  void Set(double speed);

  Pid m_elevatorPid{0, P_ELEVATOR, I_ELEVATOR, D_ELEVATOR};
  RateLimiter m_ElevatorPidRate;

  bool m_enabled = true;

private:
  rev::CANSparkMax m_elevatorMotor{ID_MOTOR_ELEVATOR, rev::CANSparkMaxLowLevel::MotorType::kBrushless};
  frc::Encoder m_elevatorEncoder{ID_ENCODER_ELEVATOR_A, ID_ENCODER_ELEVATOR_B, true};
  // HallSecurity m_elevatorHall{ID_HALL_ELEVATOR_UP};
};
