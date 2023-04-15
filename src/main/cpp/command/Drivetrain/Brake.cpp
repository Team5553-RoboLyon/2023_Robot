// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Drivetrain/Brake.h"

Brake::Brake(Drivetrain *drivetrain) : m_pDrivetrain(drivetrain)
{
}

// Called when the command is initially scheduled.
void Brake::Initialize()
{
  AddRequirements(m_pDrivetrain);
  lastLeftDist = m_pDrivetrain->getLeftDistance();
  lastRightDist = m_pDrivetrain->getRightDistance();
}

// Called repeatedly when this Command is scheduled to run
void Brake::Execute()
{
  m_pDrivetrain->SetRaw(1 * (lastLeftDist - m_pDrivetrain->getLeftDistance()), 1 * (lastRightDist - m_pDrivetrain->getRightDistance()));
  lastLeftDist = m_pDrivetrain->getLeftDistance();
  lastRightDist = m_pDrivetrain->getRightDistance();
}

// Called once the command ends or is interrupted.
void Brake::End(bool interrupted) {}

// Returns true when the command should end.
bool Brake::IsFinished()
{
  return false;
}
