// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Arm/ActiveArmMotor.h"
#include "lib/RblUtils.h"

ActiveArmMotor::ActiveArmMotor(std::function<double()> move, Arm *pArm) : m_Move(move), m_pArm(pArm)
{
  AddRequirements(m_pArm);
}

// Called when the command is initially scheduled.
void ActiveArmMotor::Initialize()
{
}

// Called repeatedly when this Command is scheduled to run
void ActiveArmMotor::Execute()
{
  double move;

  if (m_Move() > 0)
  {
    move = m_Move() * 43.0 + 90.0;
  }
  else
  {
    move = m_Move() * 123.0 + 90.0;
  }

  m_pArm->SetSetpoint(NDEGtoRAD(move));
}

// Called once the command ends or is interrupted.
void ActiveArmMotor::End(bool interrupted) {}

// Returns true when the command should end.
bool ActiveArmMotor::IsFinished()
{
  return false;
}
