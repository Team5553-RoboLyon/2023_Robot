// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include "rev/CANSparkMax.h"

class Gripper : public frc2::SubsystemBase
{
public:
  Gripper();

  /**
   * Will be called periodically whenever the CommandScheduler runs.
   */
  void Periodic() override;

private:
  rev::CANSparkMax m_gripperMotor{12, rev::CANSparkMaxLowLevel::MotorType::kBrushless};
};
