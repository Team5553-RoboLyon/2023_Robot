// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include "subsystem/DriveTrain.h"
#include "subsystem/Intake.h"
#include "subsystem/Gripper.h"
#include "subsystem/Arm.h"
#include "subsystem/Conveyor.h"
#include "subsystem/Elevator.h"
#include "subsystem/Turret.h"

/**
 * An example command.
 *
 * <p>Note that this extends CommandHelper, rather extending CommandBase
 * directly; this is crucially important, or else the decorator functions in
 * Command will *not* work!
 */
class CompleteInit
    : public frc2::CommandHelper<frc2::CommandBase, CompleteInit>
{
public:
  CompleteInit(Drivetrain *drivetrain, Intake *intake, Gripper *gripper, Arm *arm, Conveyor *conveyor, Elevator *elevator, Turret *turret);

  void Initialize() override;

  void Execute() override;

  void End(bool interrupted) override;

  bool IsFinished() override;

private:
  Drivetrain *m_drivetrain;
  Intake *m_intake;
  Gripper *m_gripper;
  Arm *m_arm;
  Conveyor *m_conveyor;
  Elevator *m_elevator;
  Turret *m_turret;
};
