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
  m_pGripper->Open();
  m_pArm->SetSetpoint(NDEGtoRAD(-35.0)); // valeur théorique à vérifier
  // std::cout << "cube mes couilles" << std::endl;
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
    if (m_count > 60) // temps à réduire
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
void TakeCubeRobot::End(bool interrupted) {}

// Returns true when the command should end.
bool TakeCubeRobot::IsFinished()
{
  return false;
}
