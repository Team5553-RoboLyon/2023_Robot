// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Intake.h"

Intake::Intake()
{
    m_intakeSolenoid.Set(frc::DoubleSolenoid::Value::kForward);

    m_intakeMotor.ConfigFactoryDefault();
    m_intakeMotorFollower.ConfigFactoryDefault();

    m_intakeMotor.SetInverted(false);
    m_intakeMotorFollower.SetInverted(true);

    m_intakeMotor.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    m_intakeMotorFollower.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);

    m_intakeMotor.ConfigOpenloopRamp(INTAKE_RAMP);
    m_intakeMotorFollower.ConfigOpenloopRamp(INTAKE_RAMP);

    m_intakeMotor.EnableVoltageCompensation(true);
    m_intakeMotorFollower.EnableVoltageCompensation(true);

    m_intakeMotor.ConfigVoltageCompSaturation(INTAKE_VOLTAGE_COMPENSATION);
    m_intakeMotorFollower.ConfigVoltageCompSaturation(INTAKE_VOLTAGE_COMPENSATION);
}

void Intake::Open()
{
    m_intakeSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
}

void Intake::Close()
{
    m_intakeSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
}

void Intake::ChangePosition()
{
    (m_intakeSolenoid.Get() == frc::DoubleSolenoid::Value::kForward) ? Close() : Open();
}

void Intake::SetSpeed(double speed)
{
    m_intakeMotor.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
    m_intakeMotorFollower.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -speed);
}
