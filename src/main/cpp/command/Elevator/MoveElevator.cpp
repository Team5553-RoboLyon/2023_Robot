// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Elevator/MoveElevator.h"

MoveElevator::MoveElevator(std::function<double()> move, Elevator *pElevator) : m_move(move), m_pElevator(pElevator)
{
  AddRequirements(m_pElevator);
}

// Called when the command is initially scheduled.
void MoveElevator::Initialize()
{
}

// Called repeatedly when this Command is scheduled to run
void MoveElevator::Execute()
{
  // double move = m_move() * 0.96;

  // m_pElevator->SetSetpoint(NABS(move));
  m_count++;
  if (!m_pElevator->IsAuto)
  {
    if (m_pElevator->IsWaiting and m_count > 100)
    {
      m_pElevator->SetSetpoint((0.0));
      m_count = 0;
      m_pElevator->IsWaiting = false;
    }
    if (!m_pElevator->IsWaiting)
    {
      m_pElevator->SetSetpoint(NABS(0.0));
    }
  }
}

// Called once the command ends or is interrupted.
void MoveElevator::End(bool interrupted) {}

// Returns true when the command should end.
bool MoveElevator::IsFinished()
{
  return false;
}
