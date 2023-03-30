// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Arm.h"
#include "lib/RblUtils.h"

// This method will be called once per scheduler run
Arm::Arm()
{
    m_armEncoder.Reset();
    m_armEncoder.SetDistancePerPulse(ARM_DISTANCE_PER_PULSE); // 1.0 / 2048.0 * 2.0 * 3.14159265358979323846 en radian
    m_armPid.SetSetpoint(NDEGtoRAD(90.0));

    m_armMotor.RestoreFactoryDefaults();
    m_armMotor.SetInverted(ARM_MOTOR_INVERTED);
    m_armMotor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    m_armMotor.SetSmartCurrentLimit(ARM_CURRENT_LIMIT);
    m_armMotor.EnableVoltageCompensation(ARM_VOLTAGE_COMPENSATION);
}

double Arm::GetEncoder()
{
    return (m_armEncoder.GetDistance() + NDEGtoRAD(123.80));
}

void Arm::SetSetpoint(double setpoint)
{
    m_armPid.SetSetpoint(NCLAMP(-33.0, setpoint, 129.0));
}

void Arm::SetGains(double p, double i, double d)
{
    m_armPid.SetGains(p, i, d);
}

void Arm::Periodic()
{
    m_armMotor.Set(NCLAMP(-0.6, m_armPid.Calculate(GetEncoder()), 0.6));
}

void Arm::Reset()
{
    m_armEncoder.Reset();
    m_armPid.SetSetpoint(NDEGtoRAD(90.0));
}
