// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Autonomous/AdvanceAutonomous.h"
#include <iostream>

AdvanceAutonomous::AdvanceAutonomous(Drivetrain *pDrivetrain, int target) : m_pDrivetrain(pDrivetrain), m_target(target)
{
  AddRequirements(m_pDrivetrain);
}

// Called when the command is initially scheduled.
void AdvanceAutonomous::Initialize()
{
  m_current = 0;
  m_pDrivetrain->Drive(0.2, 0.0);
  std::cout << "on passe" << std::endl;
}

// Called repeatedly when this Command is scheduled to run
void AdvanceAutonomous::Execute()
{
  m_current += 1;
  std::cout << m_current << std::endl;
}

// Called once the command ends or is interrupted.
void AdvanceAutonomous::End(bool interrupted)
{
  m_pDrivetrain->Drive(0.0, 0.0);
}

// Returns true when the command should end.
bool AdvanceAutonomous::IsFinished()
{
  if (m_current > m_target)
  {
    return true;
  }
  else
  {
    return false;
  }
}