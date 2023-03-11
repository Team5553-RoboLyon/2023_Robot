// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Elevator/MoveElevator.h"

MoveElevator::MoveElevator(std::function<double()> move, Elevator *pElevator) : m_move(move), m_pElevator(pElevator)
{
  AddRequirements(m_pElevator);
}

// Called when the command is initially scheduled.
void MoveElevator::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void MoveElevator::Execute()
{
  double move = m_move() * 0.96;
  std::cout << move << "move" << std::endl;
  m_pElevator->SetSetpoint(move);
}

// Called once the command ends or is interrupted.
void MoveElevator::End(bool interrupted) {}

// Returns true when the command should end.
bool MoveElevator::IsFinished()
{
  return false;
}
