// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/TimedRobot.h>

#include "lib/N/NType.h"
#include "lib/N/NFlags.h"
#include "lib/N/GL/extgl.h"

#include "lib/NL/MotionControl/Trajectory/NLTrajectory.h"
#include "lib/NL/MotionControl/Path/NLPathPersistentTrackingData.h"
#include "lib/NL/Simulation/VControllers/NLVMotorController.h"
#include "lib/NL/Simulation/VControllers/NLVEncoder.h"
#include "lib/NL/Simulation/VControllers/NLVGyro.h"

#include "lib/NL/MotionControl/NLPid.h"
#include "lib/NL/MotionControl/NLRobotPose.h"
#include "lib/NL/MotionControl/DriveTrain/Characterization/NLCharacterizationTable.h"
#include "lib/NL/MotionControl/DriveTrain/Characterization/NLMotorCharacterization.h"
#include "lib/NL/MotionControl/NLPathWorkbench.h"

#include "lib/NL/MotionControl/Trajectory/NLFollowerTank.h"

#include "rev/CANSparkMax.h"
#include <frc/Encoder.h>
#include <frc/ADXRS450_Gyro.h>

class NLVIRTUAL_ROBOT;
class Robot : public frc::TimedRobot
{
public:
  enum STATE
  {
    PATH_ERROR = 0, ///< L'initialisation du path following a rencontr� un probl�me ( erreur au chargement tr�s probablement ). Le Robot ne peut-�tre en �tat PATH_FOLLOWING.
    PATH_FOLLOWING, ///< Le robot est en �tat de suivit de chemin.
    PATH_END        ///< La Vitesse  est en d�passement.
  };

  Robot() : m_flags(0) {}
  Robot(NLVIRTUAL_ROBOT *pvr) : m_flags(0), m_pVirtualRobot(pvr) {}
  ~Robot() {}

  void inline setVirtualRobot(NLVIRTUAL_ROBOT *pvr) { m_pVirtualRobot = pvr; }
  inline Nbool IsInitialized() { return (ISFLAG_ON(m_flags, FLAG_NLROBOT_IS_INITIALIZED) ? NTRUE : false); }

  STATE m_state;
  Nu32 m_flags;

  NLMOTOR_CHARACTERIZATION m_CrtzL1;
  NLMOTOR_CHARACTERIZATION m_CrtzL2;
  NLMOTOR_CHARACTERIZATION m_CrtzR1;
  NLMOTOR_CHARACTERIZATION m_CrtzR2;

  NLTRAJECTORY_PACK m_TrajectoryPack;
  NLFOLLOWER_TANK m_follower;

  void RobotInit(NLPATH_WORKBENCH *pwb);
  void RobotPeriodic(const Nf32 dt);

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

private:
  NLVIRTUAL_ROBOT *m_pVirtualRobot;

  rev::CANSparkMax m_moteurL1{1, rev::CANSparkMax::MotorType::kBrushless};
  rev::CANSparkMax m_moteurL2{2, rev::CANSparkMax::MotorType::kBrushless};
  rev::CANSparkMax m_moteurR1{3, rev::CANSparkMax::MotorType::kBrushless};
  rev::CANSparkMax m_moteurR2{4, rev::CANSparkMax::MotorType::kBrushless};

  frc::Encoder m_leftGearboxEncoder{1, 2};
  frc::Encoder m_rightGearboxEncoder{3, 4};

  frc::ADXRS450_Gyro m_gyro{};
};
