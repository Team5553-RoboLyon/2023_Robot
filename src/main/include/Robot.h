// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/Command.h>
#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Joystick.h>
#include <rev/CANSparkMax.h>

class Robot : public frc::TimedRobot {
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


  private:
    frc::Joystick m_stick{0};
    rev::CANSparkMax m_motorRight{1, rev::CANSparkMax::MotorType::kBrushless};
    rev::CANSparkMax m_motorLeft{2, rev::CANSparkMax::MotorType::kBrushless};

    double last_invert1 = false;
    double last_invert2 = false;
};

