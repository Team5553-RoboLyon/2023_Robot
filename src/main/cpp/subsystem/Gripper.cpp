// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Gripper.h"
#include <iostream>

Gripper::Gripper()
{
    m_gripperMotor.RestoreFactoryDefaults();
    m_gripperMotor.SetSmartCurrentLimit(GRIPPER_CURRENT_LIMIT);
    m_gripperMotor.SetOpenLoopRampRate(GRIPPER_RAMP);
    m_gripperMotor.SetInverted(GRIPPER_MOTOR_INVERTED);
    m_gripperMotor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    m_gripperMotor.EnableVoltageCompensation(GRIPPER_VOLTAGE_COMPENSATION);

    m_gripperTake = true;
}

void Gripper::Take(double speed)
{
    m_gripperMotor.Set(speed);
    std::cout << "take" << std::endl;
}

void Gripper::Spit()
{
    if (DropHighCube)
    {
        m_gripperMotor.Set(-0.3);
    }
    else
    {
        m_gripperMotor.Set(-0.1);
    }
}

void Gripper::ChangePosition()
{
    if (m_gripperTake)
    {
        Take(0.3);
    }
    else
    {
        Spit();
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