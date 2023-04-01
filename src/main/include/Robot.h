// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include "RobotContainer.h"
#include <AHRS.h>
#include "frc/SerialPort.h"
#include "frc/I2C.h"

#include "lib/NL/MotionControl/DriveTrain/Characterization/NLMotorCharacterization.h"
#include "lib/NL/MotionControl/Trajectory/NLFollowerTank.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPack.h"
#include <AHRS.h>
#include "frc/I2C.h"
#include "frc/SerialPort.h"
#include "lib/NLCsv.h"
#include "frc/ADXRS450_Gyro.h"
#include "frc/SPI.h"

class Robot : public frc::TimedRobot
{
public:
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

  NLMOTOR_CHARACTERIZATION m_CrtzL;
  NLMOTOR_CHARACTERIZATION m_CrtzR;

  NLTRAJECTORY_PACK m_TrajectoryPack;
  NLFOLLOWER_TANK m_follower;

private:
  // frc::ADXRS450_Gyro m_ahrs{frc::SPI::Port::kOnboardCS0};
  RobotContainer m_robotContainer;
  frc::ADXRS450_Gyro m_gyro{frc::I2C::Port::kOnboard};

  frc2::Command *m_autonomousCommand = nullptr;

  int m_count;
  bool InAutonomous;

  NLCSV m_csv{5};

  double m_encoderLeft;
  double m_encoderRight;
  double m_gyroAngle;
  double m_VoltageLeft;
  double m_VoltageRight;
};
