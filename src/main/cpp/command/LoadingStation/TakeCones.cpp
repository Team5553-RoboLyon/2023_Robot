// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/LoadingStation/TakeCones.h"

TakeCones::TakeCones(Elevator *pElevator, Arm *pArm, Gripper *pGripper) : m_pElevator(pElevator), m_pArm(pArm), m_pGripper(pGripper)
{
  AddRequirements(m_pElevator);
  AddRequirements(m_pArm);
}

// Called when the command is initially scheduled.
void TakeCones::Initialize()
{
  m_pGripper->Take(0.6);
  m_pElevator->SetSetpoint(0.90);
  m_pArm->SetSetpoint(NDEGtoRAD(90.0));
}

// Called repeatedly when this Command is scheduled to run
void TakeCones::Execute()
{
  // m_pCopiloter->TakeCone();
  // m_pGripper->Take(0.6);
  // switch (m_State)
  // {
  // case State::open:
  //   m_pArm->SetSetpoint(NDEGtoRAD(90.0));

  //   break;
  // case State::close:
  //   m_count++;
  //   if (m_count > 10)
  //   {
  //     m_count = 0;
  //     m_State = State::high;
  //   }

  //   break;
  // case State::high:
  //   m_pArm->SetSetpoint(NDEGtoRAD(129.0));

  //   break;
  // }

  // if (m_pGripper->GetState())
  // {
  //   m_count++;
  // }
  // if (m_pGripper->GetState() and m_count > 10)
  // {
  //   m_pArm->SetSetpoint(NDEGtoRAD(130.0));
  // }
}

// Called once the command ends or is interrupted.
void TakeCones::End(bool interrupted)
{
  m_pElevator->SetSetpoint(0.0);
  m_pArm->SetSetpoint(NDEGtoRAD(115.0));
  m_pGripper->Old(0.1);
  m_pGripper->m_gripperTake = false;
}

// Returns true when the command should end.
bool TakeCones::IsFinished()
{
  return false;
}
