// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Copiloter.h"

Copiloter::Copiloter() = default;

// This method will be called once per scheduler run
void Copiloter::Periodic() {}

void Copiloter::Calcul()
{
    m_theta = asin((m_x + Length_CenterElevator_To_FloorX) * sin(Angle_Elevator) / Length_Gripper);
    m_h = sin(Angle_Elevator + m_theta) / sin(Angle_Elevator) * Length_Gripper - Length_CenterElevator_To_Floor;
}

void Copiloter::UpdateX(double joystickX)
{
    m_x += joystickX * Coef_IncrementationX;
    m_x = NCLAMP(Min_X, m_x, Max_X);
    Calcul();
    m_arm.SetSetpoint(m_theta);
    m_elevator.SetSetpoint(m_h);
}
