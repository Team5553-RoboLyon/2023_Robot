// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Arm.h"

// This method will be called once per scheduler run
Arm::Arm()
{
    m_armEncoder.Reset();
    m_armEncoder.SetDistancePerPulse(1 / 2048 * 2 * 3.14159265358979323846);
    m_armPid.SetSetpoint(0);

    m_armMotor.SetInverted(true);
    m_armMotor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    m_armMotor.SetSmartCurrentLimit(40);
    m_armMotor.EnableVoltageCompensation(8);
}

double Arm::GetEncoder()
{
    return m_armEncoder.GetDistance();
}

void Arm::SetSetpoint(double setpoint)
{
    m_armPid.SetSetpoint(setpoint);
}

void Arm::SetGains(double p, double i, double d)
{
    m_armPid.SetGains(p, i, d);
}

void Arm::Periodic()
{
    m_armMotor.Set(m_armPid.Calculate(GetEncoder()));
}
