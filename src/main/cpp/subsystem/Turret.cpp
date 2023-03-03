// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Turret.h"

Turret::Turret()
{
    m_turretEncoder.Reset();
    m_turretEncoder.SetDistancePerPulse((1 / 2048) * 1 / 3.44 * 0.96);
    m_turretPid.SetSetpoint(0);

    m_turretMotor.SetInverted(true);
    m_turretMotor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    m_turretMotor.SetSmartCurrentLimit(40);
    m_turretMotor.EnableVoltageCompensation(10);
};

void Turret::SetSetpoint(double setpoint)
{
    m_turretPid.SetSetpoint(setpoint);
}

void Turret::SetGains(double p, double i, double d)
{
    m_turretPid.SetGains(p, i, d);
}

double Turret::GetEncoder()
{
    return m_turretEncoder.GetDistance();
}

void Turret::Periodic()
{
    m_turretMotor.Set(m_turretPid.Calculate(GetEncoder()));
}
