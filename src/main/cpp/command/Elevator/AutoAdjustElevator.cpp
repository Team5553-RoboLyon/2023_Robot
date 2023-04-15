// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "command/Elevator/AutoAdjustElevator.h"

AutoAdjustElevator::AutoAdjustElevator(
    Camera *camera,
    Elevator *elevator,
    std::function<double()> height)
    : m_pCamera(camera),
      m_pElevator(elevator),
      m_height(height) {}

void AutoAdjustElevator::Initialize()
{
  AddRequirements(m_pCamera);
  AddRequirements(m_pElevator);
  if (m_height() < 0)
  {
    lastHorizontalError = m_pCamera->GetHorizontalError(true);
  }
  else
  {
    lastHorizontalError = m_pCamera->GetHorizontalError(false);
  }
}

void AutoAdjustElevator::Execute()
{
  bool top;
  if (m_height() < 0)
  {
    top = true;
  }
  else
  {
    top = false;
  }
  if (m_pCamera->HasTarget(top))
  {
    double error = m_pCamera->GetHorizontalError(top);
    if (NABS(error) - lastHorizontalError < CAMERA_MAX_ERROR_DIFFERENCE)
    {
      double vertError = m_pCamera->GetVerticalError(top);
      if (vertError >= CAMERA_TARGET_LOW_PITCH && vertError <= CAMERA_TARGET_HIGH_PITCH)
      {
        double height = NLERP(ELEVATOR_HIGH_CONE,
                              ELEVATOR_MIDDLE_CONE,
                              (vertError - CAMERA_TARGET_LOW_PITCH) / (CAMERA_TARGET_HIGH_PITCH - CAMERA_TARGET_LOW_PITCH));
        m_pElevator->SetSetpoint(height);
      }
      lastHorizontalError = NABS(error);
    }
  }
}

void AutoAdjustElevator::End(bool interrupted)
{
  m_pCamera->DisableLED();
  m_pElevator->SetSetpoint(0);
}

bool AutoAdjustElevator::IsFinished()
{
  return false;
}
