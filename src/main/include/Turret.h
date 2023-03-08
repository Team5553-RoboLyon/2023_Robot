// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include <rev/CANSparkMax.h>

class Turret : public frc2::SubsystemBase
{
public:
  Turret();

  void Set(double speed);
  void Periodic() override;

private:
  rev::CANSparkMax m_TurretMotor{7, rev::CANSparkMax::MotorType::kBrushless};
};
