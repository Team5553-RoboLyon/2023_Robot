// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once
#include <frc2/command/Command.h>

#include "lib/RblUtils.h"

class ChekMachine
{
public:
  ChekMachine();
  void SubsystemsRunning(int id);
  void SubsystemsStop(int id);
  bool AuthorisationToRun(int id);

private:
  int m_val;
};
