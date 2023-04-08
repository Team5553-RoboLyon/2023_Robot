// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Copiloter.h"

Copiloter::Copiloter() = default;

void Copiloter::TakeCubeRobot()
{

    switch (m_State)
    {
        m_count++;
    case State::Init:
        m_count = 0;
        m_Elevator.SetSetpoint(0.80);
        m_Gripper.Open();
        m_Arm.SetSetpoint(NDEGtoRAD(-35.0)); // valeur théorique à vérifier
        m_State = State::High;
        break;

    case State::High:
        if (m_Arm.GetEncoder() < -30.0) // temps à réduire
        {
            m_Elevator.SetSetpoint(0.35); // valeur théorique à vérifier
            m_count = 0;
            m_State = State::Lowered;
        }
        break;
    case State::Lowered:
        if (m_Elevator.GetEncoder() < 0.40) // temps à réduire
        {
            m_Gripper.Close();
            m_count = 0;
            m_State = State::Taken;
        }
        break;
    case State::Taken:
        if (m_count > 50) // temps à réduire
        {
            m_Elevator.SetSetpoint(0.95);
            m_count = 0;
            m_State = State::GoDown;
        }
        break;
    case State::GoDown:
        if (m_Elevator.GetEncoder() > 0.90) // temps à réduire
        {
            m_Arm.SetSetpoint(NDEGtoRAD(90.0));
            m_count = 0;
            m_State = State::Finish;
        }
        break;
    case State::Finish:
        if (m_Arm.GetEncoder() > NDEGtoRAD(60.0)) // temps à réduire
        {
            m_Elevator.SetSetpoint(0.0);
        }
        break;
    default:
        break;
    };
}