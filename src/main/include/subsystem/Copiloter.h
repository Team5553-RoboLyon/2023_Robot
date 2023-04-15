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
  void TakeCubeRobotInit();
  void TakeCone();
  void DropConeHighExecute();
  void DropConeMiddleExecute();
  void DropCubeHighExecute();
  void DropCubeMiddleExecute();

  void DropConeHighEnd();
  void DropConeMiddleEnd();
  void DropCubeHighEnd();
  void DropCubeMiddleEnd();

  enum class StateTakeCubeRobot
  {
    Init,
    High,
    Lowered,
    Taken,
    GoDown,
    Finish
  };
  StateTakeCubeRobot m_StateTakeCubeRobot;

  enum class StateTakeCone
  {
    Init,
    Taked,
    High
  };
  StateTakeCone m_StateTakeCone;

private:
  int m_count;
  int m_countCone;

  Arm *m_pArm;
  Elevator *m_pElevator;
  Gripper *m_pGripper;

  // Components (e.g. motor controllers and sensors) should generally be
  // declared private and exposed only through public methods.
};
