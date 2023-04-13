// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include "ctre/phoenix/motorcontrol/can/TalonSRX.h"
#include "Constants.h"

class Conveyor : public frc2::SubsystemBase
{
public:
  Conveyor();
  void SetSpeed(double speed);
  void Reset();

private:
  ctre::phoenix::motorcontrol::can::TalonSRX m_conveyorMotor{ID_MOTOR_CONVEYOR};

  // Components (e.g. motor controllers and sensors) should generally be
  // declared private and exposed only through public methods.
};
