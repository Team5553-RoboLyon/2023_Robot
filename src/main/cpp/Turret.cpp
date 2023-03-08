// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Turret.h"
#define NCLAMP(mn, a, mx) (((a) < (mn)) ? (mn) : ((a) > (mx)) ? (mx) \
                                                              : (a))

Turret::Turret()
{
    m_TurretMotor.RestoreFactoryDefaults();

    m_TurretMotor.SetInverted(true);
    m_TurretMotor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    m_TurretMotor.SetSmartCurrentLimit(40);
    m_TurretMotor.SetOpenLoopRampRate(0.5);
    m_TurretMotor.EnableVoltageCompensation(12);
};

void Turret::Set(double speed)
{
    m_TurretMotor.Set(NCLAMP(-0.2, speed, 0.2));
}

void Turret::Periodic() {}
