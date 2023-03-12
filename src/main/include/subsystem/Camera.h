// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/filter/LinearFilter.h>
#include <photonlib/PhotonCamera.h>
#include <photonlib/PhotonUtils.h>
#include <units/length.h>
#include "Constants.h"

class Camera : public frc2::SubsystemBase
{
public:
  Camera();
  void DisableLED();
  void EnableLED();
  void GetDistance();
  void setDistance(double distance);
  void Periodic() override;
  double GetPitch();
  double GetHorizontalError();
  bool HasTarget();
  void aprilTagMode();
  void refletiveTapeMode();
  int getAprilId();
  bool isAprilTagMode();
  bool isReflectiveMode();

private:
  // Components (e.g. motor controllers and sensors) should generally be
  // declared private and exposed only through public methods.

  photonlib::PhotonCamera m_camera{"photonvision"};
  frc::LinearFilter<double> m_horizontalErrorMovingAverage = frc::LinearFilter<double>::MovingAverage(3);
};
