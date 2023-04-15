// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Gripper/Eject.h"

Eject::Eject(Gripper *pGripper) : m_pGripper(pGripper)
{
  AddRequirements(m_pGripper);
}

// Called when the command is initially scheduled.
void Eject::Initialize()
{
  m_pGripper->count = 0;
}

// Called repeatedly when this Command is scheduled to run
void Eject::Execute()
{
  m_pGripper->count++;

  m_pGripper->Spit();
}

// Called once the command ends or is interrupted.
void Eject::End(bool interrupted)
{
  m_pGripper->m_gripperTake = true;
  m_pGripper->Stop();
  m_pGripper->count = 0;
}

// Returns true when the command should end.
bool Eject::IsFinished()
{
  return false;
}
