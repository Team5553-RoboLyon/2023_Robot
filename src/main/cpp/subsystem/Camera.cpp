// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Camera.h"

Camera::Camera() = default;

// This method will be called once per scheduler run
void Camera::Periodic() {}

void Camera::DisableLED()
{
    m_camera.SetDriverMode(false);
}

void Camera::EnableLED()
{
    m_camera.SetDriverMode(true);
}

void Camera::aprilTagMode()
{
    m_camera.SetPipelineIndex(0);
}
void Camera::refletiveTapeMode()
{
    m_camera.SetPipelineIndex(1);
}
int Camera::getAprilId()
{
    if (HasTarget())
    {
        return m_camera.GetLatestResult().GetBestTarget().GetFiducialId();
    }
    else
    {
        return 0;
    }
}
bool Camera::isAprilTagMode()
{
    return !m_camera.GetDriverMode() && (m_camera.GetPipelineIndex() == 0);
}
bool Camera::isReflectiveMode()
{
    return !m_camera.GetDriverMode() && (m_camera.GetPipelineIndex() == 1);
}

double Camera::GetPitch()
{
    if (HasTarget())
    {
        return m_camera.GetLatestResult().GetBestTarget().GetPitch();
    }
    else
    {
        return 0.0;
    }
}

double Camera::GetHorizontalError()
{
    if (HasTarget())
    {
        return m_horizontalErrorMovingAverage.Calculate(m_camera.GetLatestResult().GetBestTarget().GetYaw());
    }
    else
    {
        return 0.0;
    }
}

bool Camera::HasTarget()
{
    return m_camera.GetLatestResult().HasTargets();
}