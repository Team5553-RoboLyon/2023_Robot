// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/Joystick.h>
#include <frc2/command/Command.h>
#include <frc2/command/InstantCommand.h>
#include "subsystem/Turret.h"
#include "subsystem/Arm.h"
#include "subsystem/Elevator.h"
#include "subsystem/Gripper.h"
#include "subsystem/Drivetrain.h"
#include "subsystem/Copiloter.h"
#include "command/AutoCopiloter.h"
#include "command/Drive.h"

class RobotContainer
{
public:
  RobotContainer();
  Copiloter m_copiloter;
  Turret m_turret;
  Drivetrain m_drivetrain;
  // Arm m_arm;
  // Elevator m_elevator;
  // Gripper m_gripper;

  RateLimiter m_JoystickPrelimited_V_Robot; // joystick V rate limiter 1
  RateLimiter m_JoystickLimited_V_Robot;    // joystick V rate limiter 2

private:
  frc::Joystick m_joystickRight{0};
  frc::Joystick m_joystickLeft{1};
};
