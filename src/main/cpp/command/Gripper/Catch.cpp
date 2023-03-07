// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Gripper/Catch.h"

Catch::Catch(Gripper *pGripper) : m_pGripper(pGripper)
{
  AddRequirements(m_pGripper);
}

// Called when the command is initially scheduled.
void Catch::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void Catch::Execute()
{
  m_pGripper->ChangePosition();
}

// Called once the command ends or is interrupted.
void Catch::End(bool interrupted) {}

// Returns true when the command should end.
bool Catch::IsFinished()
{
  return false;
}
