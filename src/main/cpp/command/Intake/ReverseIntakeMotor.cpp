// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Intake/ReverseIntakeMotor.h"

ReverseIntakeMotor::ReverseIntakeMotor(Intake *pIntake) : m_pIntake(pIntake)
{
  AddRequirements(m_pIntake);
}

// Called when the command is initially scheduled.
void ReverseIntakeMotor::Initialize()
{
  m_pIntake->Open();
}

// Called repeatedly when this Command is scheduled to run
void ReverseIntakeMotor::Execute()
{
  m_pIntake->SetSpeed(-0.6);
}

// Called once the command ends or is interrupted.
void ReverseIntakeMotor::End(bool interrupted)
{
  m_pIntake->Close();
  m_pIntake->SetSpeed(0.0);
}

// Returns true when the command should end.
bool ReverseIntakeMotor::IsFinished()
{
  return false;
}
