// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/DoubleSolenoid.h>
#include <frc/PneumaticsModuleType.h>
#include "Constants.h"

class Intake : public frc2::SubsystemBase
{
public:
  Intake();

  void Open();
  void Close();
  void ChangePosition();

private:
  frc::DoubleSolenoid m_intakeSolenoid{frc::PneumaticsModuleType::REVPH, ID_SOLENOID_INTAKE_A, ID_SOLENOID_INTAKE_B};
};
