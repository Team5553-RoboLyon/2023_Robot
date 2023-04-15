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
    Camera *pCamera,
    std::function<bool()> top) : m_top(top), m_pCamera(pCamera),
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

  lastHorizontalError = m_pCamera->GetHorizontalError(m_top());
}

// Called repeatedly when this Command is scheduled to run
void AutoTurnTurret::Execute()
{
  if (m_pCamera->HasTarget(m_top()))
  {
    double error = m_pCamera->GetHorizontalError(m_top());
    if (NABS(error) - lastHorizontalError < CAMERA_MAX_ERROR_DIFFERENCE)
    {
#if TURRET
      m_pTurret->SetSetpoint(m_pTurret->GetEncoder() + 1.3 * m_pCamera->GetYaw());
#else
      m_pDrivetrain->DriveAuto(0, 0.02 * m_pCamera->GetYaw());
#endif
    }
  }
}

// Called once the command ends or is interrupted.
void AutoTurnTurret::End(bool interrupted)
{
  m_pCamera->DisableLED();
#if TURRET
  m_pTurret->SetSetpoint(0.0);
#endif
}

// Returns true when the command should end.
bool AutoTurnTurret::IsFinished()
{
  return false;
}
