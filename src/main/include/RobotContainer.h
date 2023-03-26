// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/Joystick.h>
#include <frc2/command/Command.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/button/JoystickButton.h>
#include <frc2/command/SequentialCommandGroup.h>
#include <frc2/command/ParallelCommandGroup.h>
#include <cameraserver/CameraServer.h>
#include <frc/Compressor.h>

// ################### SUBSYSTEMS ###################

#include "subsystem/Turret.h"
#include "subsystem/Arm.h"
#include "subsystem/Elevator.h"
#include "subsystem/Gripper.h"
#include "subsystem/Drivetrain.h"
#include "subsystem/Intake.h"
#include "subsystem/Conveyor.h"

// ################### COMMANDS ###################
// Intake
#include "command/Intake/ActiveIntakeMotor.h"
#include "command/Intake/ReverseIntakeMotor.h"
#include "command/Intake/ChangeIntakePosition.h"

// Conveyor
#include "command/Conveyor/ReverseConveyorMotor.h"
#include "command/Conveyor/ActiveConveyorMotor.h"

// Gripper
#include "command/Gripper/Catch.h"

// Drop
#include "command/Drop/DropHigh.h"
#include "command/Drop/DropMiddle.h"
#include "command/Drop/DropHighCube.h"
#include "command/Drop/DropMiddleCube.h"

// Copiloter
// #include "command/AutoCopiloter.h"

// Drivetrain
#include "command/Drivetrain/Drive.h"

// Turret
#include "command/Turret/TurnTurret.h"
#include "command/Turret/TurnTurret90.h"

// Elevator
#include "command/Elevator/MoveElevator.h"
#include "command/LoadingStation/TakeCones.h"
#include "command/TakeInRobot/TakeCubeRobot.h"

// Arm

#include "command/Arm/ActiveArmMotor.h"

// ################### AUTONOMOUS ###################

#include "command/autonomous/AdvanceAutonomous.h"

class RobotContainer
{
public:
  RobotContainer();

  void ConfigureButtonBindings();

  frc2::Command *GetAutonomousCommand();

  // Copiloter m_copiloter;
  Turret m_turret;
  Drivetrain m_drivetrain;
  Conveyor m_conveyor;
  Gripper m_gripper;
  Arm m_arm;
  Elevator m_elevator;
  Intake m_intake;

  frc::Joystick m_joystickRight{ID_JOYSTICK_RIGHT};
  frc::Joystick m_joystickLeft{ID_JOYSTICK_LEFT};
  frc::Joystick m_joystickCopilot{ID_JOYSTICK_COPILOTER};

private:
  cs::UsbCamera m_CameraPilote;

  frc::Compressor m_compressor{frc::PneumaticsModuleType::REVPH};

  // autonome

  frc2::SequentialCommandGroup m_autonomousGroupCommand = frc2::SequentialCommandGroup(
      AdvanceAutonomous(&m_drivetrain, 100), DropHigh(&m_elevator, &m_arm));
};
