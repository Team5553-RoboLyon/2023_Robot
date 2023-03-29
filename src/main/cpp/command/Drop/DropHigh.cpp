// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Drop/DropHigh.h"

DropHigh::DropHigh(Elevator *pElevator, Arm *pArm) : m_pElevator(pElevator), m_pArm(pArm)
{
  AddRequirements(m_pElevator);
  AddRequirements(m_pArm);
}

// Called when the command is initially scheduled.
void DropHigh::Initialize()
{
  m_pElevator->SetSetpoint(0.97);
  m_pArm->SetSetpoint(NDEGtoRAD(125.0));
}

// Called repeatedly when this Command is scheduled to run
void DropHigh::Execute()
{
}

// Called once the command ends or is interrupted.
void DropHigh::End(bool interrupted)
{
}

// Returns true when the command should end.
bool DropHigh::IsFinished()
{
  return false;
}
