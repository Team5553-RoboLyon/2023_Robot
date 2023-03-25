// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "lib/NL/MotionControl/DriveTrain/Characterization/NLCharacterizationTable.h"
#include "Robot.h"

// cc
// cc
void Robot::RobotInit() {}
void Robot::RobotPeriodic()
{
  frc2::CommandScheduler::GetInstance().Run();
}
// on fait

void Robot::AutonomousInit()
{
  NLCHARACTERIZATION_TABLE characterization_table(4);
  characterization_table.importTxt("D:/_PROJETS/FIRST/C++/Simulateur/Simulateur/data/characterization_MultiVarLinearRegression.txt");
  characterization_table.get(&m_CrtzL1, "L1", NFALSE);
  characterization_table.get(&m_CrtzL2, "L2", NFALSE);
  characterization_table.get(&m_CrtzR1, "R1", NFALSE);
  characterization_table.get(&m_CrtzR2, "R2", NFALSE);

  m_TrajectoryPack.load("trajectory1.pak");
  m_follower.load("scrumtrooper.ftk");
  m_follower.initialize(&m_TrajectoryPack);
  m_state = Robot::STATE::PATH_FOLLOWING;
}
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
int main()
{
  return frc::StartRobot<Robot>();
}
#endif
