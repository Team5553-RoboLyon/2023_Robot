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
#include "lib/Pid.h"

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

  void AutoCube1();
  void AutoCube2();

  void AutoBalance1();
  void AutoBalance2();

private:
  enum class StateAutoCube1
  {
    open,
    speed,
    close,
    forward,
    finish
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

  StateAutoCube1 m_StateAutoCube1;
  StateAutoCube2 m_StateAutoCube2;
  StateAutobalance1 m_StateAutobalance1;
  StateAutobalance2 m_StateAutobalance2;

  Pid m_pidGyro{
      0.0, 0.0011, 0.0, 0.0};

  RobotContainer m_robotContainer;
  AHRS m_ahrs{frc::I2C::Port::kOnboard};

  frc2::Command *m_autonomousCommand = nullptr;

  int m_count;
  bool InAutonomous;
  double m_output;
};
