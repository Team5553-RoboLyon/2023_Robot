// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Pilote/DownCone.h"
#include "lib/RblUtils.h"

DownCone::DownCone(Arm *pArm)
{
  AddRequirements(m_pArm);
}

// Called when the command is initially scheduled.
void DownCone::Initialize()
{
  if (m_pArm->m_use)
  {
    if (m_pArm->m_high)
    {
      m_pArm->SetSetpoint(NDEGtoRAD(80.0));
    }
    else
    {
      m_pArm->SetSetpoint(NDEGtoRAD(120.0));
    }
  }

  if (m_pArm->m_high)
  {
    m_pArm->m_high = false;
  }
  else
  {
    m_pArm->m_high = true;
  }
}

// Called repeatedly when this Command is scheduled to run
void DownCone::Execute()
{
}

// Called once the command ends or is interrupted.
void DownCone::End(bool interrupted) {}

// Returns true when the command should end.
bool DownCone::IsFinished()
{
  return false;
}
