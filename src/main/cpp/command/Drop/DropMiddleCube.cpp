// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Drop/DropMiddleCube.h"

DropMiddleCube::DropMiddleCube(Elevator *pElevator, Arm *pArm) : m_pElevator(pElevator), m_pArm(pArm)
{
  AddRequirements(m_pElevator);
  AddRequirements(m_pArm);
}

// Called when the command is initially scheduled.
void DropMiddleCube::Initialize()
{
  m_pElevator->SetSetpoint(0.42);
  m_pArm->SetSetpoint(NDEGtoRAD(100.0));
}

// Called repeatedly when this Command is scheduled to run
void DropMiddleCube::Execute() {}

// Called once the command ends or is interrupted.
void DropMiddleCube::End(bool interrupted)
{
}

// Returns true when the command should end.
bool DropMiddleCube::IsFinished()
{
  return false;
}
