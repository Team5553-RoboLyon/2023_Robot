// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"
//cc

bool Robot::IsFollower() {
  return frc::SmartDashboard::GetBoolean("Is Follower", frc::SmartDashboard::GetBoolean("Is Follower", true));
}
void Robot::RobotInit() {}
void Robot::RobotPeriodic() {
  frc2::CommandScheduler::GetInstance().Run();
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {  
  frc::SmartDashboard::PutNumber(" speed 1", 0);
  frc::SmartDashboard::PutNumber("speed 2", 0);
  frc::SmartDashboard::PutNumber("speed Follow", 0);
  frc::SmartDashboard::PutBoolean("Is Follower", true);
  //inverser les moteurs
  frc::SmartDashboard::PutBoolean("Inverser 1", false);
  frc::SmartDashboard::PutBoolean("Inverser 2", false);
}
void Robot::TeleopPeriodic() {
  if (IsFollower()) {
    m_motorLeft.Set(frc::SmartDashboard::GetNumber("speed Follow", frc::SmartDashboard::GetNumber("speed Follow", 0)));
    m_motorRight.Set(frc::SmartDashboard::GetNumber("speed Follow", frc::SmartDashboard::GetNumber("speed Follow", 0)));
  } else {
    m_motorLeft.Set(frc::SmartDashboard::GetNumber("speed 1", frc::SmartDashboard::GetNumber("speed 1", 0)));
    m_motorRight.Set(frc::SmartDashboard::GetNumber("speed 2", frc::SmartDashboard::GetNumber("speed 2", 0)));
  }

  double invert1 = frc::SmartDashboard::GetBoolean("Inverser 1", frc::SmartDashboard::GetBoolean("Inverser 1", false));
  double invert2 = frc::SmartDashboard::GetBoolean("Inverser 2", frc::SmartDashboard::GetBoolean("Inverser 2", false));

  if (invert1 != last_invert1) {
    m_motorLeft.SetInverted(invert1);
  }
  if (invert2 != last_invert2) {
    m_motorRight.SetInverted(invert2);
  }
  last_invert1 = invert1;
  last_invert2 = invert2;


}

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
