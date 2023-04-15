// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include "rev/CANSparkMax.h"
#include "Constants.h"
class Gripper : public frc2::SubsystemBase
{
public:
  Gripper();

  void Take(double speed);
  void Spit();
  void ChangePosition();
  void Old(double speed);
  void Stop();
  void Reset();
  void Periodic() override;
  bool m_gripperTake = false;
  bool DropHighCube = false;
  bool DropMidleCone=false;
  int count = 0;

private:
  rev::CANSparkMax m_gripperMotor{ID_MOTOR_GRIPPER, rev::CANSparkMaxLowLevel::MotorType::kBrushless};
};
