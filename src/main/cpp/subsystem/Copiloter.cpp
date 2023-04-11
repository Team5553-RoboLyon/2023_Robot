// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Copiloter.h"

Copiloter::Copiloter() = default;

void Copiloter::TakeCubeRobot()
{

    switch (m_StateTakeCubeRobot)
    {
        m_count++;
    case StateTakeCubeRobot::Init:
        m_count = 0;
        m_Elevator.SetSetpoint(0.80);
        m_Arm.SetSetpoint(NDEGtoRAD(-35.0)); // valeur théorique à vérifier
        m_StateTakeCubeRobot = StateTakeCubeRobot::High;
        break;

    case StateTakeCubeRobot::High:
        if (m_Arm.GetEncoder() < -30.0) // temps à réduire
        {
            m_Elevator.SetSetpoint(0.35); // valeur théorique à vérifier
            m_count = 0;
            m_StateTakeCubeRobot = StateTakeCubeRobot::Lowered;
        }
        break;
    case StateTakeCubeRobot::Lowered:
        if (m_Elevator.GetEncoder() < 0.40) // temps à réduire
        {
            m_Gripper.Take(0.5);
            m_count = 0;
            m_StateTakeCubeRobot = StateTakeCubeRobot::Taken;
        }
        break;
    case StateTakeCubeRobot::Taken:
        if (m_count > 50) // temps à réduire
        {
            m_Elevator.SetSetpoint(0.95);
            m_count = 0;
            m_StateTakeCubeRobot = StateTakeCubeRobot::GoDown;
        }
        break;
    case StateTakeCubeRobot::GoDown:
        if (m_Elevator.GetEncoder() > 0.90) // temps à réduire
        {
            m_Arm.SetSetpoint(NDEGtoRAD(90.0));
            m_count = 0;
            m_StateTakeCubeRobot = StateTakeCubeRobot::Finish;
        }
        break;
    case StateTakeCubeRobot::Finish:
        if (m_Arm.GetEncoder() > NDEGtoRAD(60.0)) // temps à réduire
        {
            m_Elevator.SetSetpoint(0.0);
        }
        break;
    default:
        break;
    };
}

void Copiloter::TakeCone()
{
    switch (m_StateTakeCone)
    {
    case StateTakeCone::Init:
        m_Elevator.SetSetpoint(0.98);
        m_Arm.SetSetpoint(NDEGtoRAD(98.0)); // valeur théorique à vérifier
        if (!m_Gripper.m_gripperTake)
        {
            m_StateTakeCone = StateTakeCone::Taked;
        }
        break;
    case StateTakeCone::Taked:
        m_countCone++;
        if (m_countCone > 20)
        {
            m_countCone = 0;
            m_StateTakeCone = StateTakeCone::High;
        }
        break;
    case StateTakeCone::High:
        m_Arm.SetSetpoint(NDEGtoRAD(129.0));
        if (m_Gripper.m_gripperTake)
        {
            m_StateTakeCone = StateTakeCone::Init;
        }
        break;
    }
}
