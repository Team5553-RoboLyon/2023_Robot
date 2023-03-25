// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/LoadingStation/TakeCones.h"

TakeCones::TakeCones(Gripper *pGripper, Elevator *pElevator, Arm *pArm) : m_pGripper(pGripper), m_pElevator(pElevator), m_pArm(pArm)
{
  AddRequirements(m_pGripper);
  AddRequirements(m_pElevator);
  AddRequirements(m_pArm);
}

// Called when the command is initially scheduled.
void TakeCones::Initialize()
{
  m_pElevator->SetSetpoint(50.0);
  m_pArm->SetSetpoint(NDEGtoRAD(90.0));
}

// Called repeatedly when this Command is scheduled to run
void TakeCones::Execute()
{
}

// Called once the command ends or is interrupted.
void TakeCones::End(bool interrupted)
{
  m_pGripper->ChangePosition();
}

// Returns true when the command should end.
bool TakeCones::IsFinished()
{
  return false;
}
