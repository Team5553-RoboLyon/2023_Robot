// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Elevator.h"

Elevator::Elevator()
{
    m_elevatorEncoder.Reset();
    m_elevatorEncoder.SetDistancePerPulse((1 / 2048) * 1 / 3.44 * 0.96);
    m_elevatorPid.SetSetpoint(0);

    m_elevatorMotor.SetInverted(true);
    m_elevatorMotor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    m_elevatorMotor.SetSmartCurrentLimit(40);
    m_elevatorMotor.EnableVoltageCompensation(10);
}

void Elevator::SetSetpoint(double setpoint)
{
    m_elevatorPid.SetSetpoint(setpoint);
}

void Elevator::SetGains(double p, double i, double d)
{
    m_elevatorPid.SetGains(p, i, d);
}

double Elevator::GetEncoder()
{
    return m_elevatorEncoder.GetDistance();
}

void Elevator::Periodic()
{
    m_elevatorMotor.Set(m_elevatorPid.Calculate(GetEncoder()));
}
