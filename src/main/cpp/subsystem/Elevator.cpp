// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Elevator.h"

Elevator::Elevator()
{
    m_elevatorEncoder.Reset();
    m_elevatorEncoder.SetDistancePerPulse(ELEVATOR_DISTANCE_PER_PULSE); //(1.0 / 2048.0) * 1.0 / 3.44 * 0.96 en m
    m_elevatorPid.SetSetpoint(0.0);

    m_elevatorMotor.SetInverted(ELEVATOR_MOTOR_INVERTED);
    m_elevatorMotor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    m_elevatorMotor.SetSmartCurrentLimit(ELEVATOR_CURRENT_LIMIT);
    m_elevatorMotor.EnableVoltageCompensation(ELEVATOR_VOLTAGE_COMPENSATION);
    m_elevatorMotor.SetOpenLoopRampRate(0.8);
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
    std::cout << m_elevatorEncoder.GetDistance() << std::endl;
    return m_elevatorEncoder.GetDistance();
}

void Elevator::Set(double speed)
{
    m_elevatorMotor.SetVoltage(units::volt_t(speed));
}

void Elevator::Periodic()
{
    m_elevatorMotor.Set((NCLAMP(-0.5, m_elevatorPid.Calculate(GetEncoder()) + 0.08, 0.5)));
}