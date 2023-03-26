// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Conveyor.h"

Conveyor::Conveyor()
{
    m_conveyorMotor.ConfigFactoryDefault();
    m_conveyorMotor.SetInverted(CONVEYOR_MOTOR_INVERTED);
    m_conveyorMotor.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    m_conveyorMotor.ConfigOpenloopRamp(CONVEYOR_RAMP);
    m_conveyorMotor.EnableVoltageCompensation(true);
    m_conveyorMotor.ConfigVoltageCompSaturation(CONVEYOR_VOLTAGE_COMPENSATION);
};

// This method will be called once per scheduler run

void Conveyor::SetSpeed(double speed)
{
    m_conveyorMotor.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
}