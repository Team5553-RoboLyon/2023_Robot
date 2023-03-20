// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Intake/ChangeIntakePosition.h"

ChangeIntakePosition::ChangeIntakePosition(Intake *pIntake) : m_pIntake(pIntake)
{
  AddRequirements(m_pIntake);
}

// Called when the command is initially scheduled.
void ChangeIntakePosition::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void ChangeIntakePosition::Execute()
{
  m_pIntake->ChangePosition();
}

// Called once the command ends or is interrupted.
void ChangeIntakePosition::End(bool interrupted) {}

// Returns true when the command should end.
bool ChangeIntakePosition::IsFinished()
{
  return false;
}
