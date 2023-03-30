// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Turret/TurnTurret.h"
#include "lib/utils.h"

TurnTurret::TurnTurret(std::function<double()> turn, Turret *pTurret, Intake *pIntake) : m_turn(turn), m_pTurret(pTurret), m_pIntake(pIntake)
{
  AddRequirements({m_pTurret, m_pIntake});
}

// Called when the command is initially scheduled.
void TurnTurret::Initialize()
{
}

// Called repeatedly when this Command is scheduled to run
void TurnTurret::Execute()
{
  if (m_pIntake->m_IntakeOpen)
  {
    m_pTurret->SetSetpoint(0.0);
  }
  else
  {
    double turn = m_turn() * 90;
    m_pTurret->SetSetpoint(turn);
  }

  // if (!m_ChekMachine.AuthorisationToRun(ID_SUBSYSTEM_TURRET))
  // {
  //   m_pTurret->SetSetpoint(0.0);
  //   m_ChekMachine.SubsystemsStop(ID_SUBSYSTEM_TURRET);
  // }
  // else
  // {
  //   m_ChekMachine.SubsystemsRunning(ID_SUBSYSTEM_TURRET);
  // }
}

// Called once the command ends or is interrupted.
void TurnTurret::End(bool interrupted)
{
  // checkMachine.subsystemStop(Turret);
}

// Returns true when the command should end.
bool TurnTurret::IsFinished()
{
  return false;
}
