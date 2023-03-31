// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

RobotContainer::RobotContainer()
{
    ConfigureButtonBindings();
    m_drivetrain.SetDefaultCommand(Drive([=]
                                         { return -m_joystickLeft.GetY(); },
                                         [=]
                                         { return -m_joystickRight.GetZ(); },
                                         &m_drivetrain));

    m_turret.SetDefaultCommand(TurnTurret([=]
                                          { return m_joystickCopilot.GetZ(); },
                                          &m_turret));

    m_elevator.SetDefaultCommand(MoveElevator([=]
                                              { return m_joystickCopilot.GetY(); },
                                              &m_elevator));

    // m_arm.SetDefaultCommand(MoveArm([=]
    //                                 { return m_joystickCopilot.GetX(); },
    //                                 &m_arm));

    m_CameraPilote = frc::CameraServer::StartAutomaticCapture();
    m_CameraPilote.SetResolution(320, 240);
    m_CameraPilote.SetFPS(12);

    m_compressor.EnableDigital();
};

// ################### COMMANDS ###################

void RobotContainer::ConfigureButtonBindings()
{

    // Conveyor

    frc2::JoystickButton m_ButtonActiveMotorConveyorIntake = frc2::JoystickButton(&m_joystickRight, 1);
    m_ButtonActiveMotorConveyorIntake.WhileActiveContinous(frc2::ParallelCommandGroup(ActiveConveyorMotor(&m_conveyor), ActiveIntakeMotor(&m_intake)));

    frc2::JoystickButton m_ButtonConveyorIntakeReverseMotor = frc2::JoystickButton(&m_joystickRight, 2);
    m_ButtonConveyorIntakeReverseMotor.WhileActiveContinous(frc2::ParallelCommandGroup(ReverseConveyorMotor(&m_conveyor), ReverseIntakeMotor(&m_intake)));

    // Gripper

    frc2::JoystickButton m_ButtonGripperChangePosition = frc2::JoystickButton(&m_joystickCopilot, 1);
    m_ButtonGripperChangePosition.WhileActiveOnce(Catch(&m_gripper));

    frc2::JoystickButton m_ButtonTakeCones = frc2::JoystickButton(&m_joystickCopilot, 5);
    m_ButtonTakeCones.WhileActiveContinous(TakeCones(&m_elevator, &m_arm, &m_gripper));

    frc2::JoystickButton m_ButtonDropHigh = frc2::JoystickButton(&m_joystickCopilot, 7);
    m_ButtonDropHigh.WhileActiveContinous(DropHigh(&m_elevator, &m_arm));

    frc2::JoystickButton m_ButtonDropMiddle = frc2::JoystickButton(&m_joystickCopilot, 6);
    m_ButtonDropMiddle.WhileActiveContinous(DropMiddle(&m_elevator, &m_arm));

    frc2::JoystickButton m_ButtonCubeDropHigh = frc2::JoystickButton(&m_joystickCopilot, 8);
    m_ButtonCubeDropHigh.WhileActiveContinous(DropHighCube(&m_elevator, &m_arm));

    frc2::JoystickButton m_ButtonCubeDropMiddle = frc2::JoystickButton(&m_joystickCopilot, 9);
    m_ButtonCubeDropMiddle.WhileActiveContinous(DropMiddleCube(&m_elevator, &m_arm));

    frc2::JoystickButton m_ButtonCubeRobot = frc2::JoystickButton(&m_joystickCopilot, 10);
    m_ButtonCubeRobot.WhenPressed(TakeCubeRobot(&m_elevator, &m_arm, &m_gripper));

    frc2::JoystickButton m_ButtonActiveCompressor = frc2::JoystickButton(&m_joystickCopilot, 11);
    m_ButtonActiveCompressor.WhenActive(frc2::InstantCommand([this]
                                                             {
        if (m_compressor.Enabled())
        {
            m_compressor.Disable();
        }
        else
        {
            m_compressor.EnableDigital();
        } }));
}

// frc2::Command *RobotContainer::GetAutonomousCommand()
// {

//     return &m_autonomousGroupCommand;
// }
