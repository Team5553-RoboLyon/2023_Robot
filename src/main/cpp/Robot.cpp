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
	m_robotContainer.GetAutonomousCommand()->Schedule();

	// ######## NLMOTOR_CHARACTERIZATION ########
	NLCHARACTERIZATION_TABLE characterization_table(4);
	// characterization_table.importTxt("D:/_PROJETS/FIRST/C++/Simulateur/Simulateur/data/characterization_MultiVarLinearRegression.txt");
	characterization_table.get(&m_CrtzL, "L1", NFALSE);
	characterization_table.get(&m_CrtzR, "R1", NFALSE);

	m_TrajectoryPack.load("trajectory1.pak");
	m_follower.initialize(&m_TrajectoryPack);
	m_state = Robot::STATE::PATH_FOLLOWING;
}
void Robot::AutonomousPeriodic()
{
}

void Robot::TeleopInit()
{
	// m_robotContainer.m_drivetrain.m_logCSV.open("/home/lvuser/", true); // ouverture du fichier de log
}
void Robot::TeleopPeriodic()
{
	// m_robotContainer.m_arm.m_speed = m_robotContainer.m_joystickRight.GetY();

	// if (m_robotContainer.m_joystickLeft.GetRawButtonPressed(1))
	// {
	//   m_robotContainer.m_gripper.ChangePosition();
	// }
	// frc::SmartDashboard::PutNumber("x", m_robotContainer.m_copiloter.m_x);
	// frc::SmartDashboard::PutNumber("h", m_robotContainer.m_copiloter.m_h);
	// frc::SmartDashboard::PutNumber("tetha", m_robotContainer.m_copiloter.m_theta);
	// frc::SmartDashboard::PutNumber("encoderElevator", m_robotContainer.m_copiloter.m_elevator.GetEncoder());
	// frc::SmartDashboard::PutNumber("encoderArm", m_robotContainer.m_copiloter.m_arm.GetEncoder());
	frc::SmartDashboard::PutNumber("encoderArm", m_robotContainer.m_arm.GetEncoder());
	frc::SmartDashboard::PutNumber("outputArm", m_robotContainer.m_arm.m_armPid.m_output);

	// frc::SmartDashboard::PutNumber("outputelevateur", m_robotContainer.m_copiloter.m_elevator.m_elevatorPid.m_output);
	// frc::SmartDashboard::PutNumber("setpoint", m_robotContainer.m_copiloter.m_elevator.m_elevatorPid.m_setpoint);
	// m_robotContainer.m_copiloter.m_elevator.SetGains(frc::SmartDashboard::GetNumber("Pelevator", 0),
	//                                                  frc::SmartDashboard::GetNumber("Ielevator", 0),
	//                                                  frc::SmartDashboard::GetNumber("Delevator", 0));

	// m_robotContainer.m_copiloter.m_arm.SetGains(frc::SmartDashboard::GetNumber("Parm", 0),
	//                                             frc::SmartDashboard::GetNumber("Iarm", 0),
	//                                             frc::SmartDashboard::GetNumber("Darm", 0));

	// m_robotContainer.m_turret.SetGains(frc::SmartDashboard::GetNumber("Pturret", 0),
	//                                    frc::SmartDashboard::GetNumber("Iturret", 0),
	//                                    frc::SmartDashboard::GetNumber("Dturret", 0));

	// m_robotContainer.m_copiloter.m_elevator.SetSetpoint(frc::SmartDashboard::GetNumber("Setpointelevator", 0));
	// m_robotContainer.m_copiloter.m_arm.SetSetpoint(frc::SmartDashboard::GetNumber("Setpointarm", 0));
	// m_robotContainer.m_turret.SetSetpoint(frc::SmartDashboard::GetNumber("Setpointturret", 0));

	// m_robotContainer.m_copiloter.m_elevator.Set(frc::SmartDashboard::GetNumber("voltage", 0.0));
}

void Robot::DisabledInit()
{
	// m_robotContainer.m_drivetrain.m_logCSV.close(); // fermeture du fichier de log
}
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
