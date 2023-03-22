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
                                         { return -m_joystickRight.GetY(); },
                                         [=]
                                         { return -m_joystickLeft.GetZ(); },
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

    m_compressor.EnableAnalog(MIN_PRESSURE_COMPRESSOR, MAX_PRESSURE_COMPRESSOR);
};

// ################### COMMANDS ###################

void RobotContainer::ConfigureButtonBindings()
{
    // Intake

    frc2::JoystickButton m_ButtonIntakeActiveMotor = frc2::JoystickButton(&m_joystickRight, 4);
    m_ButtonIntakeActiveMotor.WhileActiveContinous(ActiveIntakeMotor(&m_intake));

    frc2::JoystickButton m_ButtonIntakeReverseMotor = frc2::JoystickButton(&m_joystickRight, 5);
    m_ButtonIntakeReverseMotor.WhileActiveContinous(ReverseIntakeMotor(&m_intake));

    frc2::JoystickButton m_ButtonIntakeChangePosition = frc2::JoystickButton(&m_joystickRight, 2);
    m_ButtonIntakeChangePosition.WhileActiveContinous(ChangeIntakePosition(&m_intake));

    // Conveyor

    frc2::JoystickButton m_ButtonConveyorActiveMotor = frc2::JoystickButton(&m_joystickRight, 7);
    m_ButtonConveyorActiveMotor.WhileActiveContinous(ActiveConveyorMotor(&m_conveyor));

    frc2::JoystickButton m_ButtonConveyorReverseMotor = frc2::JoystickButton(&m_joystickRight, 8);
    m_ButtonConveyorReverseMotor.WhileActiveContinous(ReverseConveyorMotor(&m_conveyor));

    // Gripper

    frc2::JoystickButton m_ButtonGripperChangePosition = frc2::JoystickButton(&m_joystickRight, 1);
    m_ButtonGripperChangePosition.WhileActiveContinous(Catch(&m_gripper));
}

frc2::Command *RobotContainer::GetAutonomousCommand()
{
    return &m_autonomousGroupCommand;
}
