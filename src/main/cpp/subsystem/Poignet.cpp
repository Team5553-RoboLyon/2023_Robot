// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Poignet.h"

Poignet::Poignet()
{
    m_poignetPid.SetSetpoint(0.0);

    m_PoignetPidRate.Reset(0.0, 0.0, 0.25);

    m_poignetEncoder.Reset();
    m_poignetEncoder.SetDistancePerPulse(POIGNET_DISTANCE_PER_PULSE);

    m_poignetMotor.SetInverted(POIGNET_MOTOR_INVERTED);
    m_poignetMotor.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    m_poignetMotor.ConfigOpenloopRamp(POIGNET_RAMP);
    m_poignetMotor.EnableVoltageCompensation(true);
    m_poignetMotor.ConfigVoltageCompSaturation(POIGNET_VOLTAGE_COMPENSATION);
};

void Poignet::SetSetpoint(double setpoint)
{
    m_poignetPid.SetSetpoint(setpoint);
}

void Poignet::SetGains(double p, double i, double d)
{
    m_poignetPid.SetGains(p, i, d);
}

double Poignet::GetEncoder()
{
    return m_poignetEncoder.GetDistance();
}

void Poignet::Periodic()
{

    double output = m_poignetPid.Calculate(GetEncoder());
    m_PoignetPidRate.Update(output);

    m_poignetMotor.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, NCLAMP(-0.1, m_PoignetPidRate.m_current, 0.1));
}
