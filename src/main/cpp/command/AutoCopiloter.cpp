// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/AutoCopiloter.h"

AutoCopiloter::AutoCopiloter(std::function<double()> forward, Copiloter *pCopiloter) : m_Forward(forward), m_pCopiloter(pCopiloter)
{
  AddRequirements(m_pCopiloter);
}

// Called when the command is initially scheduled.
void AutoCopiloter::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void AutoCopiloter::Execute()
{
  double forward = m_Forward();
  m_pCopiloter->UpdateX(forward);
  std::cout << "on passe" << std::endl;
}

// Called once the command ends or is interrupted.
void AutoCopiloter::End(bool interrupted) {}

// Returns true when the command should end.
bool AutoCopiloter::IsFinished()
{
  return false;
}
