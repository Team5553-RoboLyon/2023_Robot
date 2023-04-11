// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Gripper.h"

Gripper::Gripper()
{
    m_gripperMotor.RestoreFactoryDefaults();
    m_gripperMotor.SetSmartCurrentLimit(GRIPPER_CURRENT_LIMIT);
    m_gripperMotor.SetOpenLoopRampRate(GRIPPER_RAMP);
    m_gripperMotor.SetInverted(GRIPPER_MOTOR_INVERTED);
    m_gripperMotor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    m_gripperMotor.EnableVoltageCompensation(GRIPPER_VOLTAGE_COMPENSATION);

    m_gripperTake = false;
}

void Gripper::Take(double speed)
{
    m_gripperMotor.Set(speed);
}

void Gripper::Spit(double speed)
{
    m_gripperMotor.Set(-speed);
}

void Gripper::ChangePosition()
{
    if (m_gripperTake)
    {
        Take(0.5);
    }
    else
    {
        Spit(0.5);
    }
}

void Gripper::Stop()
{
    m_gripperMotor.Set(0.0);
}

void Gripper::Old(double speed)
{
    m_gripperMotor.Set(speed);
}

void Gripper::Reset()
{
    m_gripperTake = false;
}

void Gripper::Periodic()
{
}