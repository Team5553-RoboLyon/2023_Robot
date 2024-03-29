// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Drop/DropMiddleCube.h"

DropMiddleCube::DropMiddleCube(Elevator *pElevator, Arm *pArm, Gripper *pGripper) : m_pElevator(pElevator), m_pArm(pArm), m_pGripper(pGripper)
{
  AddRequirements(m_pElevator);
  AddRequirements(m_pArm);
}

// Called when the command is initially scheduled.
void DropMiddleCube::Initialize()
{
  m_pElevator->SetSetpoint(ELEVATOR_MIDDLE_CUBE);
  m_pArm->SetSetpoint(NDEGtoRAD(97.0));
  m_pGripper->DropHighCube = true;
}

// Called repeatedly when this Command is scheduled to run
void DropMiddleCube::Execute()
{
  // m_pCopiloter->DropCubeMiddleExecute();
}

// Called once the command ends or is interrupted.
void DropMiddleCube::End(bool interrupted)
{
  m_pElevator->SetSetpoint(0.0);
  m_pArm->SetSetpoint(NDEGtoRAD(90.0));
  m_pGripper->DropHighCube = false;
  // m_pCopiloter->DropCubeMiddleEnd();
}

// Returns true when the command should end.
bool DropMiddleCube::IsFinished()
{
  return false;
}
