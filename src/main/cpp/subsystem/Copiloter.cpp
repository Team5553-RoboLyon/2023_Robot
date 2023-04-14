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
        m_pElevator->SetSetpoint(0.80);
        m_pArm->SetSetpoint(NDEGtoRAD(-15.0)); // valeur théorique à vérifier
        m_StateTakeCubeRobot = StateTakeCubeRobot::High;
        break;

    case StateTakeCubeRobot::High:
        if (m_pArm->GetEncoder() < NDEGtoRAD(-10.0)) // temps à réduire
        {
            m_pElevator->SetSetpoint(0.45); // valeur théorique à vérifier
            m_count = 0;
            m_StateTakeCubeRobot = StateTakeCubeRobot::Lowered;
        }

        break;
    case StateTakeCubeRobot::Lowered:
        if (m_pElevator->GetEncoder() < 0.60) // temps à réduire
        {
            m_pGripper->Take(0.5);
            m_count = 0;
            m_StateTakeCubeRobot = StateTakeCubeRobot::Taken;
        }
        break;
    case StateTakeCubeRobot::Taken:
        if (m_count > 15) // temps à réduire
        {
            m_pElevator->SetSetpoint(0.95);
            m_count = 0;
            m_StateTakeCubeRobot = StateTakeCubeRobot::GoDown;
        }
        break;
    case StateTakeCubeRobot::GoDown:
        if (m_pElevator->GetEncoder() > 0.50) // temps à réduire
        {
            m_pArm->SetSetpoint(NDEGtoRAD(90.0));
            m_count = 0;
            m_StateTakeCubeRobot = StateTakeCubeRobot::Finish;
        }
        break;
    case StateTakeCubeRobot::Finish:
        if (m_pArm->GetEncoder() > NDEGtoRAD(40.0)) // temps à réduire
        {
            m_pElevator->SetSetpoint(0.0);
        }
        m_pGripper->Old(0.1);
        break;
    default:
        break;
    };
}

void Copiloter::TakeCubeRobotInit()
{
    m_StateTakeCubeRobot = StateTakeCubeRobot::Init;
}

void Copiloter::TakeCone()
{
    m_pGripper->Take(0.6);
    m_pElevator->SetSetpoint(0.80);
    m_pArm->SetSetpoint(NDEGtoRAD(90.0));
}

void Copiloter::DropConeHighExecute()
{
    m_pElevator->SetSetpoint(0.97);
    m_pArm->SetSetpoint(NDEGtoRAD(110.0));
}

void Copiloter::DropConeMiddleExecute()
{
    m_pElevator->SetSetpoint(0.80);
    m_pArm->SetSetpoint(NDEGtoRAD(90.0));
}

void Copiloter::DropCubeHighExecute()
{
    m_pElevator->SetSetpoint(0.98);
    m_pArm->SetSetpoint(NDEGtoRAD(85.0));
    m_pGripper->DropHighCube = true;
}

void Copiloter::DropCubeMiddleExecute()
{
    m_pElevator->SetSetpoint(0.43);
    m_pArm->SetSetpoint(NDEGtoRAD(97.0));
    m_pGripper->DropHighCube = true;
}

void Copiloter::DropConeHighEnd()
{
    m_pElevator->SetSetpoint(0.0);
    m_pArm->SetSetpoint(NDEGtoRAD(90.0));
}

void Copiloter::DropConeMiddleEnd()
{
    m_pElevator->SetSetpoint(0.0);
    m_pArm->SetSetpoint(NDEGtoRAD(90.0));
}

void Copiloter::DropCubeHighEnd()
{
    m_pElevator->SetSetpoint(0.0);
    m_pArm->SetSetpoint(NDEGtoRAD(90.0));
    m_pGripper->DropHighCube = false;
}

void Copiloter::DropCubeMiddleEnd()
{
    m_pElevator->SetSetpoint(0.0);
    m_pArm->SetSetpoint(NDEGtoRAD(90.0));
    m_pGripper->DropHighCube = false;
}
