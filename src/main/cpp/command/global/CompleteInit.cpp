// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/global/CompleteInit.h"

CompleteInit::CompleteInit(Drivetrain *drivetrain, Intake *intake, Gripper *gripper, Arm *arm, Conveyor *conveyor, Elevator *elevator, Turret *turret) : m_drivetrain(drivetrain), m_intake(intake), m_gripper(gripper), m_arm(arm), m_conveyor(conveyor), m_elevator(elevator), m_turret(turret)
{
  AddRequirements({m_drivetrain, m_intake, m_gripper, m_arm, m_conveyor, m_elevator, m_turret});
}

// Called when the command is initially scheduled.
void CompleteInit::Initialize()
{
  m_drivetrain->Reset();
  m_intake->Reset();
  m_gripper->Reset();
  m_arm->Reset();
  m_conveyor->Reset();
  m_elevator->Reset();
  m_turret->Reset();
}

// Called repeatedly when this Command is scheduled to run
void CompleteInit::Execute() {}

// Called once the command ends or is interrupted.
void CompleteInit::End(bool interrupted) {}

// Returns true when the command should end.
bool CompleteInit::IsFinished()
{
  return false;
}
