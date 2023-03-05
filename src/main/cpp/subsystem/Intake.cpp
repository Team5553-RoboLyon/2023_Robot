// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Intake.h"

Intake::Intake()
{
    m_intakeSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
}

void Intake::Open()
{
    m_intakeSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
}

void Intake::Close()
{
    m_intakeSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
}

void Intake::ChangePosition()
{
    (m_intakeSolenoid.Get() == frc::DoubleSolenoid::Value::kForward) ? Close() : Open();
}
