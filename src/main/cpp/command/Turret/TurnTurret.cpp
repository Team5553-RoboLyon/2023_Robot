// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Turret/TurnTurret.h"
#include "lib/utils.h"

TurnTurret::TurnTurret(std::function<double()> turn, Turret *pTurret) : m_turn(turn), m_pTurret(pTurret)
{
  AddRequirements(m_pTurret);
}

// Called when the command is initially scheduled.
void TurnTurret::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void TurnTurret::Execute()
{
  double turn = m_turn() * 90;
  std::cout << turn << "m_turn" << std::endl;
  m_pTurret->SetSetpoint(turn);
}

// Called once the command ends or is interrupted.
void TurnTurret::End(bool interrupted) {}

// Returns true when the command should end.
bool TurnTurret::IsFinished()
{
  return false;
}
