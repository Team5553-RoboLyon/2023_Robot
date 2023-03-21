// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Arm/ActiveArmMotor.h"

ActiveArmMotor::ActiveArmMotor(std::function<double()> move, Arm *pArm) : m_move(move), m_pArm(pArm)
{
  AddRequirements(m_pArm);
}

// Called when the command is initially scheduled.
void ActiveArmMotor::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void ActiveArmMotor::Execute()
{
  double move = m_move();
  m_pArm->SetSetpoint(move);
}

// Called once the command ends or is interrupted.
void ActiveArmMotor::End(bool interrupted) {}

// Returns true when the command should end.
bool ActiveArmMotor::IsFinished()
{
  return false;
}
