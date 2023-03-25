// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Turret/TurnTurret90.h"

TurnTurret90::TurnTurret90(Turret *pTurret) : m_pTurret(pTurret)
{
  AddRequirements(m_pTurret);
}

// Called when the command is initially scheduled.
void TurnTurret90::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void TurnTurret90::Execute()
{
  m_pTurret->SetSetpoint(90);
}

// Called once the command ends or is interrupted.
void TurnTurret90::End(bool interrupted) {}

// Returns true when the command should end.
bool TurnTurret90::IsFinished()
{
  return false;
}
