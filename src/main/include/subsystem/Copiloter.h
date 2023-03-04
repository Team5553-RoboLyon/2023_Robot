// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include "Constants.h"
#include "arm.h"
#include "elevator.h"
#include "gripper.h"
#include "math.h"
#include "lib/RblUtils.h"

class Copiloter : public frc2::SubsystemBase
{
public:
  Copiloter();

  void Calcul();
  void UpdateX(double joystickX);
  void Periodic() override;
  double m_x;
  double m_theta;
  double m_h;
  Arm m_arm;
  Elevator m_elevator;

private:
};
