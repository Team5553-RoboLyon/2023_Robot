// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include "rev/CANSparkMax.h"
#include <frc/Encoder.h>
#include "lib/Pid.h"
#include "Constants.h"

class Arm : public frc2::SubsystemBase
{
public:
  Arm();

  void SetSetpoint(double setpoint);
  void Periodic() override;
  void SetGains(double p, double i, double d);
  void SetMotorArm(double speed);
  double GetEncoder();
  void Reset();

  double m_speed;
  Pid m_armPid{0, P_ARM, I_ARM, D_ARM};
  bool m_high = true;

private:
  rev::CANSparkMax m_armMotor{ID_MOTOR_ARM, rev::CANSparkMaxLowLevel::MotorType::kBrushless};
  frc::Encoder m_armEncoder{ID_ENCODER_ARM_A, ID_ENCODER_ARM_B, true};
};
