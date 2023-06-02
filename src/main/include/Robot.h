// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include "RobotContainer.h"
#include <AHRS.h>
#include "lib/NL/MotionControl/DriveTrain/Characterization/NLMotorCharacterization.h"
#include "lib/NL/MotionControl/Trajectory/NLFollowerTank.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPack.h"
#include "frc/I2C.h"
#include "frc/SerialPort.h"
#include "lib/NLCsv.h"
#include "frc/SPI.h"
#include "lib/Pid.h"
#include "lib/RblUtils.h"

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

  bool TakeCubeRobot();

  void AutoCube1();
  void AutoCube2();
  void AutoBalance1();
  void AutoBalance2();
  void AutoCubeHaut();
  void AutoBalanceTout();

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
  enum class StateAutoCube1
  {
    open,
    speed,
    close,
    forward,
    finish
  };

  enum class StateAutoBalanceTout
  {
    forward,
    stop
  };

  enum class StateAutoCube2
  {
    open,
    speed,
    close,
    forward,
    open2,
    close2,
    backward,
    finish
  };

  enum class StateAutobalance1
  {
    forward,
    stop,
    recule,
    forwardV2,
    finish
  };

  enum class StateAutobalance2
  {
    open,
    speed,
    close,
    tour,
    forward,
    finish
  };

  enum class StateAutoCubeHaut
  {
    Init,
    High,
    Lowered,
    Taken,
    GoDown,
    Forward,
    Open,
    Recule,
    Finish
  };

  StateAutoCube1 m_StateAutoCube1;
  StateAutoCube2 m_StateAutoCube2;
  StateAutobalance1 m_StateAutobalance1;
  StateAutobalance2 m_StateAutobalance2;
  StateAutoCubeHaut m_StateAutoCubeHaut;
  StateAutoBalanceTout m_StateAutobalanceTout;

  enum class StateTakeCubeRobot
  {
    Init,
    High,
    Lowered,
    Taken,
    GoDown,
    Finish
  };
  StateTakeCubeRobot m_StateTakeCubeRobot;

  int m_countTakeCube;

  Pid m_pidGyro{
      0.0, 0.0011, 0.0, 0.0};

  RobotContainer m_robotContainer;

  double m_output;
  AHRS m_gyro{frc::SerialPort::Port::kUSB};

  frc2::Command *m_autonomousCommand = nullptr;

  int m_count;
  bool InAutonomous;

  NLCSV m_csv{5};

  double m_encoderLeft;
  double m_encoderRight;
  double m_gyroAngle;
  double m_VoltageLeft;
  double m_VoltageRight;

  bool m_takeCubeInRobot;
  bool m_armToLevelCube2;
  bool m_autoCube;
};
