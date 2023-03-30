// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Gripper.h"

Gripper::Gripper()
{
    Open();
}

void Gripper::Open()
{
    m_gripperSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
}

void Gripper::Close()
{
    m_gripperSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
}

void Gripper::ChangePosition()
{
    (m_gripperSolenoid.Get() == frc::DoubleSolenoid::Value::kForward) ? Close() : Open();
}

bool Gripper::GetState()
{
    return m_gripperSolenoid.Get() == frc::DoubleSolenoid::Value::kReverse;
}

void Gripper::Reset()
{
    Close();
}