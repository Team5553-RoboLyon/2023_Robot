// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Copiloter.h"

Copiloter::Copiloter() = default;

// This method will be called once per scheduler run
void Copiloter::Periodic() {}

void Copiloter::Calcul()
{
    double a = ((m_x + Length_CenterElevator_To_FloorX) * sin(Angle_Elevator) / Length_Gripper);
    m_theta = asin(NMIN(a, 1.0));
    m_h = (sin(Angle_Elevator + m_theta) / sin(Angle_Elevator) * Length_Gripper - Length_CenterElevator_To_Floor) - 0.242;
    std::cout << "theta: " << m_theta << " h: " << m_h << std::endl;
    if (m_h < 0)
    {
        m_h = 0;
        std::cout << "ERRROOROROR" << std::endl;
    }
}

void Copiloter::UpdateX(double joystickX)
{
    joystickX = (((joystickX + 1.0) / 2.0) * (Max_X - Min_X)) + Min_X;
    m_x = joystickX;
    m_x = NCLAMP(Min_X, m_x, Max_X);
    Calcul();
    m_arm.SetSetpoint(NCLAMP(NF64_PI_4, m_theta, NF64_PI_4 * 3.0));
    m_elevator.SetSetpoint(NCLAMP(0.0, m_h, 0.8));
}
