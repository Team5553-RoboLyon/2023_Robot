// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/TakeInRobot/TakeCubeRobot.h"
#include "lib/RblUtils.h"

// ###################### A TESTER ######################

TakeCubeRobot::TakeCubeRobot(Elevator *pElevator, Arm *pArm, Gripper *pGripper) : m_pElevator(pElevator), m_pArm(pArm), m_pGripper(pGripper)
{
  AddRequirements(m_pElevator);
  AddRequirements(m_pArm);
  AddRequirements(m_pGripper);
}

// Called when the command is initially scheduled.
void TakeCubeRobot::Initialize()
{
  m_count = 0;
  m_pElevator->SetSetpoint(0.80);
  m_pArm->SetSetpoint(NDEGtoRAD(-25.0)); // valeur théorique à vérifier
  m_State = State::High;
  // descend elevateur

  // ferme pince

  // remonte elevateur
}

// Called repeatedly when this Command is scheduled to run
void TakeCubeRobot::Execute()
{
  m_count++;
  switch (m_State)
  {
  case State::High:
    if (m_pArm->GetEncoder() < NDEGtoRAD(-20.0)) // temps à réduire
    {
      m_pElevator->SetSetpoint(0.50); // valeur théorique à vérifier
      m_count = 0;
      m_State = State::Lowered;
    }

    break;
  case State::Lowered:
    if (m_pElevator->GetEncoder() < 0.60) // temps à réduire
    {
      m_pGripper->Take(0.5);
      m_count = 0;
      m_State = State::Taken;
    }
    break;
  case State::Taken:
    if (m_count > 50) // temps à réduire
    {
      m_pElevator->SetSetpoint(0.95);
      m_count = 0;
      m_State = State::GoDown;
    }
    break;
  case State::GoDown:
    if (m_pElevator->GetEncoder() > 0.60) // temps à réduire
    {
      m_pArm->SetSetpoint(NDEGtoRAD(90.0));
      m_count = 0;
      m_State = State::Finish;
    }
    break;
  case State::Finish:
    if (m_pArm->GetEncoder() > NDEGtoRAD(60.0)) // temps à réduire
    {
      m_pElevator->SetSetpoint(0.0);
    }
    break;
  default:
    break;
  };
}

// Called once the command ends or is interrupted.
void TakeCubeRobot::End(bool interrupted)
{
  switch (m_State)
  {
  case State::High:
    m_pElevator->SetSetpoint(0.80);
    m_pArm->SetSetpoint(NDEGtoRAD(90.0));
    m_pElevator->IsWaiting = true;
    break;
  case State::Lowered:
    m_pElevator->SetSetpoint(0.80);
    m_pArm->SetSetpoint(NDEGtoRAD(90.0));
    m_pElevator->IsWaiting = true;
    break;
  case State::Taken:
    m_pElevator->SetSetpoint(0.80);
    m_pArm->SetSetpoint(NDEGtoRAD(90.0));
    m_pElevator->IsWaiting = true;
    break;
  case State::GoDown:
    // retour défaut
    break;
  case State::Finish:
    // retour défaut
    break;
  default:
    break;
  }
}

// Returns true when the command should end.
bool TakeCubeRobot::IsFinished()
{
  // m_count = 0;
  // m_count++;
  // switch (m_State)
  // {
  // case (State::High):
  //   m_pElevator->SetSetpoint(0.80);
  //   if (m_count > 80)
  //   {
  //     m_pArm->SetSetpoint(NDEGtoRAD(90.0));
  //     return true;
  //   }
  //   break;
  // case (State::Lowered):
  //   m_pElevator->SetSetpoint(0.80);
  //   if (m_count > 80)
  //   {
  //     m_pArm->SetSetpoint(NDEGtoRAD(90.0));
  //     return true;
  //   }
  //   break;
  // case (State::Taken):
  //   m_pElevator->SetSetpoint(0.80);
  //   if (m_count > 80)
  //   {
  //     m_pArm->SetSetpoint(NDEGtoRAD(90.0));
  //     return true;
  //   }
  //   break;
  // case (State::GoDown):
  //   m_pArm->SetSetpoint(NDEGtoRAD(90.0));
  //   if (m_count > 80)
  //   {
  //     return true;
  //   }
  //   break;

  // default:
  //   break;
  // }
  return false;
}

/*


############################# CODE QUI MARCHE #############################


#include "command/TakeInRobot/TakeCubeRobot.h"
#include "lib/RblUtils.h"

// ###################### A TESTER ######################

TakeCubeRobot::TakeCubeRobot(Elevator *pElevator, Arm *pArm, Gripper *pGripper) : m_pElevator(pElevator), m_pArm(pArm), m_pGripper(pGripper)
{
  AddRequirements(m_pElevator);
  AddRequirements(m_pArm);
  AddRequirements(m_pGripper);
}

// Called when the command is initially scheduled.
void TakeCubeRobot::Initialize()
{
  m_count = 0;
  m_pElevator->SetSetpoint(0.80);
  m_pGripper->Open();
  m_pArm->SetSetpoint(NDEGtoRAD(-35.0)); // valeur théorique à vérifier
  m_State = State::High;
  // descend elevateur

  // ferme pince

  // remonte elevateur
}

// Called repeatedly when this Command is scheduled to run
void TakeCubeRobot::Execute()
{
  m_count++;
  switch (m_State)
  {
  case State::High:
    if (m_count > 80) // temps à réduire
    {
      m_pElevator->SetSetpoint(0.35); // valeur théorique à vérifier
      m_count = 0;
      m_State = State::Lowered;
    }
    break;
  case State::Lowered:
    if (m_count > 45) // temps à réduire
    {
      m_pGripper->Close();
      m_count = 0;
      m_State = State::Taken;
    }
    break;
  case State::Taken:
    if (m_count > 50) // temps à réduire
    {
      m_pElevator->SetSetpoint(0.95);
      m_count = 0;
      m_State = State::GoDown;
    }
    break;
  case State::GoDown:
    if (m_count > 50) // temps à réduire
    {
      m_pArm->SetSetpoint(NDEGtoRAD(90.0));
      m_count = 0;
      m_State = State::Finish;
    }
    break;
  case State::Finish:
    if (m_count > 90) // temps à réduire
    {
      m_pElevator->SetSetpoint(0.0);
    }
    break;
  default:
    break;
  };
}

// Called once the command ends or is interrupted.
void TakeCubeRobot::End(bool interrupted)
{
  switch (m_State)
  {
  case State::High:
    m_pElevator->SetSetpoint(0.80);
    m_pArm->SetSetpoint(NDEGtoRAD(90.0));
    m_pElevator->IsWaiting = true;
    break;
  case State::Lowered:
    m_pElevator->SetSetpoint(0.80);
    m_pArm->SetSetpoint(NDEGtoRAD(90.0));
    m_pElevator->IsWaiting = true;
    break;
  case State::Taken:
    m_pElevator->SetSetpoint(0.80);
    m_pArm->SetSetpoint(NDEGtoRAD(90.0));
    m_pElevator->IsWaiting = true;
    break;
  case State::GoDown:
    // retour défaut
    break;
  case State::Finish:
    // retour défaut
    break;
  default:
    break;
  }
}

// Returns true when the command should end.
bool TakeCubeRobot::IsFinished()
{
  // m_count = 0;
  // m_count++;
  // switch (m_State)
  // {
  // case (State::High):
  //   m_pElevator->SetSetpoint(0.80);
  //   if (m_count > 80)
  //   {
  //     m_pArm->SetSetpoint(NDEGtoRAD(90.0));
  //     return true;
  //   }
  //   break;
  // case (State::Lowered):
  //   m_pElevator->SetSetpoint(0.80);
  //   if (m_count > 80)
  //   {
  //     m_pArm->SetSetpoint(NDEGtoRAD(90.0));
  //     return true;
  //   }
  //   break;
  // case (State::Taken):
  //   m_pElevator->SetSetpoint(0.80);
  //   if (m_count > 80)
  //   {
  //     m_pArm->SetSetpoint(NDEGtoRAD(90.0));
  //     return true;
  //   }
  //   break;
  // case (State::GoDown):
  //   m_pArm->SetSetpoint(NDEGtoRAD(90.0));
  //   if (m_count > 80)
  //   {
  //     return true;
  //   }
  //   break;

  // default:
  //   break;
  // }
  return false;
}


*/