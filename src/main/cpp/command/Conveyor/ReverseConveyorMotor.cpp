// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Conveyor/ReverseConveyorMotor.h"

ReverseConveyorMotor::ReverseConveyorMotor(Conveyor *pConveyor) : m_pConveyor(pConveyor)
{
  AddRequirements(m_pConveyor);
}

// Called when the command is initially scheduled.
void ReverseConveyorMotor::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void ReverseConveyorMotor::Execute()
{
  m_pConveyor->SetSpeed(-1.0);
}

// Called once the command ends or is interrupted.
void ReverseConveyorMotor::End(bool interrupted)
{
  m_pConveyor->SetSpeed(0.0);
}

// Returns true when the command should end.
bool ReverseConveyorMotor::IsFinished()
{
  return false;
}
