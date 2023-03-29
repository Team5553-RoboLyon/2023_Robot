// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "lib/NL/MotionControl/DriveTrain/Characterization/NLCharacterizationTable.h"

#include "lib/NL/MotionControl/Trajectory/NLTrajectoryActionMessagesEnum.h"
#include "Robot.h"

// cc
// cc
void Robot::SetMotor(double speed)
{
  m_L1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
  m_L2.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
  m_L3.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
  m_R1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
  m_R2.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
  m_R3.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
}
void Robot::RobotInit() {}
void Robot::RobotPeriodic()
{
  frc2::CommandScheduler::GetInstance().Run();
}
// on fait

void Robot::AutonomousInit()
{
  // ######## Motor ########

  m_L1.ConfigFactoryDefault();
  m_L2.ConfigFactoryDefault();
  m_L3.ConfigFactoryDefault();
  m_R1.ConfigFactoryDefault();
  m_R2.ConfigFactoryDefault();
  m_R3.ConfigFactoryDefault();

  m_L1.SetInverted(true);
  m_L2.SetInverted(true);
  m_L3.SetInverted(true);

  m_R1.SetInverted(false);
  m_R2.SetInverted(false);
  m_R3.SetInverted(false);

  m_L1.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
  m_L2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
  m_L3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);

  m_R1.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
  m_R2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
  m_R3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);

  // ######## NLMOTOR_CHARACTERIZATION ########
  NLCHARACTERIZATION_TABLE characterization_table(4);
  //characterization_table.importTxt("D:/_PROJETS/FIRST/C++/Simulateur/Simulateur/data/characterization_MultiVarLinearRegression.txt");
  characterization_table.get(&m_CrtzL1, "L1", NFALSE);
  characterization_table.get(&m_CrtzL2, "L2", NFALSE);
  characterization_table.get(&m_CrtzR1, "R1", NFALSE);
  characterization_table.get(&m_CrtzR2, "R2", NFALSE);
  
  m_TrajectoryPack.load("trajectory1.pak");
  m_follower.load("scrumtrooper.ftk");
  m_follower.initialize(&m_TrajectoryPack);
  m_state = Robot::STATE::PATH_FOLLOWING;
}
void Robot::AutonomousPeriodic() 
{
	NLRAMSETEOUTPUT output;
	const NLFOLLOWER_TANK_OUTPUT* pout;
	
	NLTRJ_POSTED_MESSAGE message;	// Posted Message

	switch (m_state)
	{
		case Robot::STATE::PATH_ERROR:
			break;

		case Robot::STATE::PATH_FOLLOWING:
			// *****************************************************    'THE' METHOD(e)
			// A) Feed back:
			// avec les encodeurs on estime la position du robot:
			//			l = distance parcourue par la roue gauche depuis le dernier reset encodeur.
			//			r = distance parcourue par la roue droite depuis le dernier reset encodeur.
			//
			//			dl et dr = distances parcourues par les roues gauche et droite depuis le dernier call.
			//			(note dl/dt = vitesse roue gauche et dr/dt = vitesse roue droite  )
			//
			/*
			l = (m_leftGearboxEncoder.getRaw() / 8192.0f) * NF32_2PI * m_DriveTrainSpecs.m_wheelRadius;
			r = (m_rightGearboxEncoder.getRaw() / 8192.0f) * NF32_2PI * m_DriveTrainSpecs.m_wheelRadius;
			dl = l - m_dsLeftWheel;
			dr = r - m_dsRightWheel;
			m_dsLeftWheel = l;
			m_dsRightWheel = r;
			// forward:
			m_estimatedPose.odometryUpdate(&m_DriveTrainSpecs, dl, dr, m_gyro.get());
			*/
			// backward:
			//m_estimatedPose.odometryUpdate(&m_DriveTrainSpecs, -dr, -dl, m_gyro.get());
			m_follower.estimate(m_leftGearboxEncoder.getRaw() / 8192.0f, m_rightGearboxEncoder.getRaw() / 8192.0f, m_gyro.get());

			/*
			// B) Feed forward : State ( full )
			m_currrentPoint.m_kin.m_t += dt;
			m_TrajectoryPack.getPoint(&m_currrentPoint, m_currrentPoint.m_kin.m_t);
			*/
			m_follower.updateTarget(&m_TrajectoryPack,dt);
			// C) Ramsete:
			//m_ramsete.update2(&output, &m_DriveTrainSpecs, &m_currrentPoint, &m_estimatedPose);
			//m_ramsete.update3(&output, &m_follower);
			
			pout =  m_follower.compute();
			// forward:
			/*
			m_moteurL1.SetVoltage(m_CrtzL1.getVoltage(output.m_leftVelocity, output.m_leftAcceleration));
			m_moteurL2.SetVoltage(m_CrtzL2.getVoltage(output.m_leftVelocity, output.m_leftAcceleration));
			m_moteurR1.SetVoltage(m_CrtzR1.getVoltage(output.m_rightVelocity, output.m_rightAcceleration));
			m_moteurR2.SetVoltage(m_CrtzR2.getVoltage(output.m_rightVelocity, output.m_rightAcceleration));
			*/
			/*
			// backward:
			m_moteurR1.SetVoltage(m_CrtzL1.getVoltage(-output.m_leftVelocity, -output.m_leftAcceleration));
			m_moteurR2.SetVoltage(m_CrtzL2.getVoltage(-output.m_leftVelocity, -output.m_leftAcceleration));

			m_moteurL1.SetVoltage(m_CrtzR1.getVoltage(-output.m_rightVelocity,-output.m_rightAcceleration));
			m_moteurL2.SetVoltage(m_CrtzR2.getVoltage(-output.m_rightVelocity,-output.m_rightAcceleration));
			*/
			m_moteurL1.SetVoltage(m_CrtzL1.getVoltage(pout->m_leftVelocity,		pout->m_leftAcceleration));
			m_moteurL2.SetVoltage(m_CrtzL2.getVoltage(pout->m_leftVelocity,		pout->m_leftAcceleration));
			m_moteurR1.SetVoltage(m_CrtzR1.getVoltage(pout->m_rightVelocity,	pout->m_rightAcceleration));
			m_moteurR2.SetVoltage(m_CrtzR2.getVoltage(pout->m_rightVelocity,	pout->m_rightAcceleration));

			//m_logCsv.write();

			while (m_follower.getMessage(&message))
			{
				switch (message.m_id)
				{
					case OPEN_INTAKE:
						m_allMechanisms.Command("open intake");
						break;
					case CLOSE_INTAKE:
						m_allMechanisms.Command("close intake");
						break;
					case ACTIVATE_INTAKE_WHEELS:
						m_allMechanisms.Command("activate intake wheels");
						break;
					case DEACTIVATE_INTAKE_WHEELS:
						m_allMechanisms.Command("deactivate intake wheels");
						break;
					case ACTIVATE_CONVEYOR:
						m_allMechanisms.Command("activate conveyor");
						break;
					case DEACTIVATE_CONVEYOR:
						m_allMechanisms.Command("deactivate intake wheels");
						break;

					case ACTIVATE_AIM:
						break;
					case DEACTIVATE_AIM:
						break;
					case ACTIVATE_SHOOTER:
						break;
					case DEACTIVATE_SHOOTER:
						break;

					default:
						break;
				}
			}

			break;

		case NLROBOT::STATE::PATH_END:
			break;
		default:
			NErrorIf(1, NERROR_UNAUTHORIZED_CASE);
			break;
	}
}

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
