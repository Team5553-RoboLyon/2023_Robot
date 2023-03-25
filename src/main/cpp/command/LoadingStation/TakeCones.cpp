// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/LoadingStation/TakeCones.h"

TakeCones::TakeCones(Elevator *pElevator, Arm *pArm, Gripper *pGripper) : m_pElevator(pElevator), m_pArm(pArm), m_pGripper(pGripper)
{
  AddRequirements(m_pElevator);
  AddRequirements(m_pArm);
  AddRequirements(m_pGripper);
}

// Called when the command is initially scheduled.
void TakeCones::Initialize()
{
  m_pElevator->SetSetpoint(0.98);
  m_pArm->SetSetpoint(NDEGtoRAD(108.0));
  m_count = 0;
}

// Called repeatedly when this Command is scheduled to run
void TakeCones::Execute()
{
  if (m_pGripper->GetState())
  {
    m_count++;
  }
  if (m_pGripper->GetState() and m_count > 10)
  {
    m_pArm->SetSetpoint(NDEGtoRAD(130.0));
  }
}

// Called once the command ends or is interrupted.
void TakeCones::End(bool interrupted)
{
}

// Returns true when the command should end.
bool TakeCones::IsFinished()
{
  return false;
}
