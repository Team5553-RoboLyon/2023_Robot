// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include "subsystem/Elevator.h"
#include "subsystem/Arm.h"
#include "subsystem/Gripper.h"

class Copiloter : public frc2::SubsystemBase
{
public:
  Copiloter();

  /**
   * Will be called periodically whenever the CommandScheduler runs.
   */
  void TakeCubeRobot();
  void Periodic() override;

private:
  int m_count;
  enum class State
  {
    Init,
    High,
    Lowered,
    Taken,
    GoDown,
    Finish
  };
  State m_State;

  Arm m_Arm;
  Elevator m_Elevator;
  Gripper m_Gripper;

  // Components (e.g. motor controllers and sensors) should generally be
  // declared private and exposed only through public methods.
};
