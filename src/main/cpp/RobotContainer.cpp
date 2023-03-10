// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

RobotContainer::RobotContainer()
{
    // m_copiloter.SetDefaultCommand(AutoCopiloter([=]
    //                                             { return m_joystickLeft.GetY(); },
    //                                             &m_copiloter));

    m_drivetrain.SetDefaultCommand(Drive([=]
                                         { return m_joystickRight.GetY(); },
                                         [=]
                                         { return -m_joystickLeft.GetZ(); },
                                         &m_drivetrain));

    m_turret.SetDefaultCommand(TurnTurret([=]
                                          { return m_joystickCopilot.GetZ(); },
                                          &m_turret));

    m_elevator.SetDefaultCommand(MoveElevator([=]
                                              { return m_joystickCopilot.GetY(); },
                                              &m_elevator));
};

// ################### COMMANDS ###################

void RobotContainer::ConfigureButtonBindings()
{
    // frc2::JoystickButton m_ButtonGripperCatch = frc2::JoystickButton(&m_joystickRight, 2);
    // m_ButtonGripperCatch.WhenActive(Catch(&m_gripper));

    // frc2::JoystickButton m_ButtonDropHigh = frc2::JoystickButton(&m_joystickRight, 3);
    // m_ButtonDropHigh.WhenActive(DropHigh(&m_gripper, &m_elevator, &m_arm));
}
