// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Drop/DropHighCube.h"

DropHighCube::DropHighCube(Elevator *pElevator, Arm *pArm, Gripper *pGripper) : m_pElevator(pElevator), m_pArm(pArm), m_pGripper(pGripper)
{
  AddRequirements(m_pElevator);
  AddRequirements(m_pArm);
}

// Called when the command is initially scheduled.
void DropHighCube::Initialize()
{
  m_pElevator->SetSetpoint(ELEVATOR_HIGH_CUBE);
  m_pArm->SetSetpoint(NDEGtoRAD(85.0));
  m_pGripper->DropHighCube = true;
  m_pArm->m_high = true;
}

// Called repeatedly when this Command is scheduled to run
void DropHighCube::Execute()
{
  // m_pCopiloter->DropCubeHighExecute();
}

// Called once the command ends or is interrupted.
void DropHighCube::End(bool interrupted)
{
  // m_pCopiloter->DropCubeHighEnd();

  m_pElevator->SetSetpoint(0.0);
  m_pArm->SetSetpoint(NDEGtoRAD(115.0));
  m_pGripper->DropHighCube = false;
}

// Returns true when the command should end.
bool DropHighCube::IsFinished()
{
  return false;
}
