// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Intake/ActiveIntakeMotor.h"
#include <iostream>
ActiveIntakeMotor::ActiveIntakeMotor(Intake *pIntake) : m_pIntake(pIntake)
{
  AddRequirements(m_pIntake);
}

// Called when the command is initially scheduled.
void ActiveIntakeMotor::Initialize()
{
  m_pIntake->Open();
}

// Called repeatedly when this Command is scheduled to run
void ActiveIntakeMotor::Execute()
{
  m_pIntake->SetSpeed(1.0);
  std::cout << "on" << std::endl;
}

// Called once the command ends or is interrupted.
void ActiveIntakeMotor::End(bool interrupted)
{
  m_pIntake->SetSpeed(0.0);
  m_pIntake->Close();
}

// Returns true when the command should end.
bool ActiveIntakeMotor::IsFinished()
{
  return false;
}
