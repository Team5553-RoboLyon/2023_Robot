// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Turret.h"

Turret::Turret()
{
    m_turretEncoder.Reset();
    m_turretEncoder.SetDistancePerPulse(TURRET_DISTANCE_PER_PULSE); //(1 / 2048) * 1 / 3.44 * 0.96 en degré
    m_turretPid.SetSetpoint(0);

    m_turretMotor.RestoreFactoryDefaults();
    m_turretMotor.SetInverted(TURRET_MOTOR_INVERTED);
    m_turretMotor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    m_turretMotor.SetSmartCurrentLimit(TURRET_CURRENT_LIMIT);
    m_turretMotor.EnableVoltageCompensation(TURRET_VOLTAGE_COMPENSATION);
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
