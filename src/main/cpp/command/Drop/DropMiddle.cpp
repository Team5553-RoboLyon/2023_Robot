// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Drop/DropMiddle.h"

DropMiddle::DropMiddle(Elevator *pElevator, Arm *pArm, Gripper *pGripper) : m_pElevator(pElevator), m_pArm(pArm), m_pGripper(pGripper)
{
  AddRequirements(m_pElevator);
  // AddRequirements(m_pArm);
}

// Called when the command is initially scheduled.
void DropMiddle::Initialize()
{
  m_pElevator->SetSetpoint(ELEVATOR_MIDDLE_CONE);
  m_pArm->SetSetpoint(NDEGtoRAD(105.0));
  m_pGripper->DropMidleCone = true;
  m_pArm->m_high = true;
  m_pArm->m_use = true;
}

// Called repeatedly when this Command is scheduled to run
void DropMiddle::Execute()
{
  // m_pCopiloter->DropConeMiddleExecute();
}

// Called once the command ends or is interrupted.
void DropMiddle::End(bool interrupted)
{
  // m_pCopiloter->DropConeMiddleEnd();
  m_pElevator->SetSetpoint(0.0);
  m_pArm->SetSetpoint(NDEGtoRAD(115.0));
  m_pGripper->DropMidleCone = false;
  m_pArm->m_use = false;
  m_pArm->m_high = false;
}

// Returns true when the command should end.
bool DropMiddle::IsFinished()
{
  return false;
}
