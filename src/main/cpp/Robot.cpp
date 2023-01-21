// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

//cc
void Robot::DisableLED()
{
    m_camera.SetLEDMode(photonlib::LEDMode::kOff);
}

void Robot::EnableLED() { m_camera.SetLEDMode(photonlib::LEDMode::kOn); }

int Robot::IdAprilTag()
{
    double Id=m_camera.GetLatestResult().GetBestTarget().GetFiducialId();
    return Id;
}

void Robot::RobotInit() {}
void Robot::RobotPeriodic() {}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {}

void Robot::DisabledInit() {}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}

void Robot::SimulationInit() {}
void Robot::SimulationPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
