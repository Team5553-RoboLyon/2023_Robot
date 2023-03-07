// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/DropHigh.h"

DropHigh::DropHigh(Gripper *pGripper, Elevator *pElevator, Arm *pArm) : m_pGripper(pGripper), m_pElevator(pElevator), m_pArm(pArm)
{
  AddRequirements(m_pGripper);
  AddRequirements(m_pElevator);
  AddRequirements(m_pArm);
}

// Called when the command is initially scheduled.
void DropHigh::Initialize()
{
}

// Called repeatedly when this Command is scheduled to run
void DropHigh::Execute()
{
  m_pElevator->SetSetpoint(0.0);
  m_pArm->SetSetpoint(0.0);
}

// Called once the command ends or is interrupted.
void DropHigh::End(bool interrupted)
{
  m_pGripper->ChangePosition();
}

// Returns true when the command should end.
bool DropHigh::IsFinished()
{
  return false;
}
