// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include "lib/NL/MotionControl/DriveTrain/Characterization/NLMotorCharacterization.h"
#include "lib/NL/MotionControl/Trajectory/NLFollowerTank.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPack.h"

#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include "RobotContainer.h"
#include "ctre/phoenix/motorcontrol/can/TalonFX.h"

class Robot : public frc::TimedRobot
{
public:
  void SetMotor(double speed);

  void RobotInit() override;
  void RobotPeriodic() override;

  void AutonomousInit() override;
  void AutonomousPeriodic() override;

  void TeleopInit() override;
  void TeleopPeriodic() override;

  void DisabledInit() override;
  void DisabledPeriodic() override;

  void TestInit() override;
  void TestPeriodic() override;

  void SimulationInit() override;
  void SimulationPeriodic() override;

  enum STATE
  {
    PATH_ERROR = 0, ///< L'initialisation du path following a rencontr� un probl�me ( erreur au chargement tr�s probablement ). Le Robot ne peut-�tre en �tat PATH_FOLLOWING.
    PATH_FOLLOWING, ///< Le robot est en �tat de suivit de chemin.
    PATH_END        ///< La Vitesse  est en d�passement.
  };

  STATE m_state;

  NLMOTOR_CHARACTERIZATION m_CrtzL1;
  NLMOTOR_CHARACTERIZATION m_CrtzL2;
  NLMOTOR_CHARACTERIZATION m_CrtzR1;
  NLMOTOR_CHARACTERIZATION m_CrtzR2;

  NLTRAJECTORY_PACK m_TrajectoryPack;
  NLFOLLOWER_TANK m_follower;

  ctre::phoenix::motorcontrol::can::TalonFX m_L1{1};
  ctre::phoenix::motorcontrol::can::TalonFX m_L2{2};
  ctre::phoenix::motorcontrol::can::TalonFX m_L3{3};
  ctre::phoenix::motorcontrol::can::TalonFX m_R1{4};
  ctre::phoenix::motorcontrol::can::TalonFX m_R2{5};
  ctre::phoenix::motorcontrol::can::TalonFX m_R3{6};

private:
  RobotContainer m_robotContainer;
};
