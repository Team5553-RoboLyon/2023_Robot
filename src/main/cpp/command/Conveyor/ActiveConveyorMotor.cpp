// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Conveyor/ActiveConveyorMotor.h"

ActiveConveyorMotor::ActiveConveyorMotor(Conveyor *pConveyor) : m_pConveyor(pConveyor)
{
  AddRequirements(m_pConveyor);
}

// Called when the command is initially scheduled.
void ActiveConveyorMotor::Initialize()
{
  m_count = 0;
}

// Called repeatedly when this Command is scheduled to run
void ActiveConveyorMotor::Execute()
{
  m_count++;
  m_pConveyor->SetSpeed(1.0);
}

// Called once the command ends or is interrupted.
void ActiveConveyorMotor::End(bool interrupted)
{
  m_pConveyor->SetSpeed(0.0);
}

// Returns true when the command should end.
bool ActiveConveyorMotor::IsFinished()
{
  return false;
}
