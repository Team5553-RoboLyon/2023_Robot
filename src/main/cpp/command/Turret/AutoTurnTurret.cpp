// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Turret/AutoTurnTurret.h"

AutoTurnTurret::AutoTurnTurret(
#if TURRET
    Turret *pTurret,
#else
    Drivetrain *pDrivetrain,
#endif
    Camera *pCamera) : m_pCamera(pCamera),
#if TURRET
                       m_pTurret(pTurret)
#else
                       m_pDrivetrain(pDrivetrain)
#endif

{
#if TURRET
  AddRequirements(m_pTurret);
#else
  AddRequirements(m_pDrivetrain);
#endif
  AddRequirements(m_pCamera);
  // Use addRequirements() here to declare subsystem dependencies.
}

// Called when the command is initially scheduled.
void AutoTurnTurret::Initialize()
{
  m_pCamera->EnableLED();
  m_pCamera->refletiveTapeMode();
}

// Called repeatedly when this Command is scheduled to run
void AutoTurnTurret::Execute()
{
  if (m_pCamera->HasTarget())
  {
#if TURRET
    // version avec tourelle
    m_pTurret->SetSetpoint(m_pTurret->GetSetPoint() + m_pCamera->GetPitch());
#else
    m_pDrivetrain->DriveAuto(0, m_pCamera->GetPitch());
#endif
  }
}

// Called once the command ends or is interrupted.
void AutoTurnTurret::End(bool interrupted)
{
  m_pCamera->DisableLED();
}

// Returns true when the command should end.
bool AutoTurnTurret::IsFinished()
{
  return false;
}
