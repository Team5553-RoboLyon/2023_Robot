// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/TakeInRobot/TakeCubeRobot.h"

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
  m_pElevator->SetSetpoint(80.0);
  m_pGripper->Open();
  m_pArm->SetSetpoint(0.0);
  std::cout << "cube mes couilles" << std::endl;
  // descend elevateur

  // ferme pince

  // remonte elevateur
}

// Called repeatedly when this Command is scheduled to run
void TakeCubeRobot::Execute()
{
  m_count++;
  if (m_count > 100)
  {
    m_pGripper->Close();
    m_pElevator->SetSetpoint(50.0);
    m_pArm->SetSetpoint(90.0);
  }
}

// Called once the command ends or is interrupted.
void TakeCubeRobot::End(bool interrupted) {}

// Returns true when the command should end.
bool TakeCubeRobot::IsFinished()
{
  return false;
}
