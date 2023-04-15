// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#if TURRET
#include "subsystem/Turret.h"
#else
#include "subsystem/Drivetrain.h"

#endif
#include "subsystem/Camera.h"
#include "command/Turret/TurnTurret.h"
#include "Constants.h"

/**
 * An example command.
 *
 * <p>Note that this extends CommandHelper, rather extending CommandBase
 * directly; this is crucially important, or else the decorator functions in
 * Command will *not* work!
 */
class AutoTurnTurret
    : public frc2::CommandHelper<frc2::CommandBase, AutoTurnTurret>
{
public:
  AutoTurnTurret(
#if TURRET
      Turret *pTurret,
#else
      Drivetrain *pDrivetrain,
#endif
      Camera *pCamera,
      std::function<bool()> top);

  void Initialize() override;

  void Execute() override;

  void End(bool interrupted) override;

  bool IsFinished() override;

private:
#if TURRET
  Turret *m_pTurret;
#else
  Drivetrain *m_pDrivetrain;
#endif
  Camera *m_pCamera;
  std::function<double()> m_top;
  double lastHorizontalError = 0.0;
};
