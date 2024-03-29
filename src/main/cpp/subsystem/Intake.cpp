// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Intake.h"
#include <iostream>

Intake::Intake()
{
    Close();
    m_intakeMotorRight.ConfigFactoryDefault();
    m_intakeMotorLeft.ConfigFactoryDefault();

    m_intakeMotorRight.SetInverted(INTAKE_MOTOR_LEFT_INVERTED);
    m_intakeMotorLeft.SetInverted(INTAKE_MOTOR_RIGHT_INVERTED);

    m_intakeMotorRight.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    m_intakeMotorLeft.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);

    m_intakeMotorRight.ConfigOpenloopRamp(INTAKE_RAMP);
    m_intakeMotorLeft.ConfigOpenloopRamp(INTAKE_RAMP);

    m_intakeMotorRight.EnableVoltageCompensation(true);
    m_intakeMotorLeft.EnableVoltageCompensation(true);

    m_intakeMotorRight.ConfigVoltageCompSaturation(INTAKE_VOLTAGE_COMPENSATION);
    m_intakeMotorLeft.ConfigVoltageCompSaturation(INTAKE_VOLTAGE_COMPENSATION);
}

void Intake::Open()
{
    std::cout << "open" << std::endl;
    m_IntakeOpen = true;
    m_intakeSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
}

void Intake::Close()
{
    m_IntakeOpen = false;
    m_intakeSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
}

void Intake::ChangePosition()
{
    (m_IntakeOpen) ? Close() : Open();
}

void Intake::SetSpeed(double speed)
{
    m_intakeMotorRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
    m_intakeMotorLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -speed);
}

void Intake::Reset()
{
    Close();
}