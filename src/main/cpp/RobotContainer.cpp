// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

RobotContainer::RobotContainer()
{
    ConfigureButtonBindings();
    // m_copiloter.SetDefaultCommand(AutoCopiloter([=]
    //                                             { return m_joystickLeft.GetY(); },
    //                                             &m_copiloter));

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

    m_CameraPilote = frc::CameraServer::StartAutomaticCapture();
    m_CameraPilote.SetResolution(320, 240);
    m_CameraPilote.SetFPS(12);

    m_compressor.EnableDigital();
};

// ################### COMMANDS ###################

void RobotContainer::ConfigureButtonBindings()
{
    // Intake

    frc2::JoystickButton m_ButtonIntakeChangePosition = frc2::JoystickButton(&m_joystickRight, 7);
    m_ButtonIntakeChangePosition.WhileActiveContinous(ChangeIntakePosition(&m_intake));

    // Conveyor

    // frc2::JoystickButton m_ButtonActiveMotorConveyorIntake = frc2::JoystickButton(&m_joystickRight, 1);
    // m_ButtonActiveMotorConveyorIntake.WhileActiveContinous(frc2::ParallelCommandGroup(ActiveConveyorMotor(&m_conveyor), ActiveIntakeMotor(&m_intake)));

    // frc2::JoystickButton m_ButtonConveyorIntakeReverseMotor = frc2::JoystickButton(&m_joystickRight, 2);
    // m_ButtonConveyorIntakeReverseMotor.WhileActiveContinous(frc2::ParallelCommandGroup(ReverseConveyorMotor(&m_conveyor), ReverseIntakeMotor(&m_intake)));

    // Gripper

    frc2::JoystickButton m_ButtonGripperChangePosition = frc2::JoystickButton(&m_joystickCopilot, 1);
    m_ButtonGripperChangePosition.WhileActiveContinous(Catch(&m_gripper));

    frc2::JoystickButton m_ButtonDropHigh = frc2::JoystickButton(&m_joystickCopilot, 2);
    m_ButtonDropHigh.WhileActiveContinous(DropHigh(&m_gripper, &m_elevator, &m_arm));
}

frc2::Command *RobotContainer::GetAutonomousCommand()
{
    return &m_autonomousGroupCommand;
}
