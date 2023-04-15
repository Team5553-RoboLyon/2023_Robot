// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystem/Camera.h"

Camera::Camera() = default;

// This method will be called once per scheduler run
void Camera::Periodic() {}

void Camera::DisableLED()
{
    m_camera.SetDriverMode(true);
}

void Camera::EnableLED()
{
    m_camera.SetDriverMode(false);
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
    if (m_camera.GetLatestResult().HasTargets())
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

double Camera::GetHorizontalError(bool top)
{
    std::optional<photonlib::PhotonTrackedTarget> nearestTarget = GetNearestTarget(top);
    if (nearestTarget)
    {
        return (*nearestTarget).GetYaw();
    }
    else
    {
        return 0.0;
    }
}

double Camera::GetVerticalError(bool top)
{
    std::optional<photonlib::PhotonTrackedTarget> nearestTarget = GetNearestTarget(top);
    if (nearestTarget)
    {
        return (*nearestTarget).GetPitch();
    }
    else
    {
        return 0.0;
    }
}

std::optional<photonlib::PhotonTrackedTarget> Camera::GetNearestTarget(bool top)
{
    if (!m_camera.HasTargets())
    {
        return std::nullopt;
    }
    std::optional<photonlib::PhotonTrackedTarget> nearestTarget; // TODO if optional is working
    std::span<const photonlib::PhotonTrackedTarget> targets = m_camera.GetLatestResult().GetTargets();
    // int startIndex = 0;
    // for (int i = 0; i < targets.size(); i++)
    // {
    //     if (top)
    //     {
    //         if (targets[i].GetPitch() > CAMERA_HIGH_LOW_DELIMITER)
    //         {
    //             nearestTarget = targets[i];
    //             startIndex = i;
    //             break;
    //         }
    //     }
    //     else
    //     {
    //         nearestTarget = targets[i];
    //         startIndex = i;
    //         break;
    //     }
    // }

    for (int i = 0 /*startIndex*/; i < targets.size(); i++)
    {
        if (top)
        {
            if (targets[i].GetPitch() > CAMERA_HIGH_LOW_DELIMITER)
            {
                if (nearestTarget && (NABS(targets[i].GetYaw()) < NABS((*nearestTarget).GetYaw())))
                {
                    nearestTarget = targets[i];
                }
                else
                {
                    nearestTarget = targets[i];
                }
            }
        }
        else
        {
            if (nearestTarget && (NABS(targets[i].GetYaw()) < NABS((*nearestTarget).GetYaw())))
            {
                nearestTarget = targets[i];
            }
            else
            {
                nearestTarget = targets[i];
            }
        }
    }
    return nearestTarget;
}

bool Camera::HasTarget(bool top)
{
    if (!m_camera.HasTargets())
    {
        return false;
    }
    if (!top)
    {
        return true;
    }
    std::span<const photonlib::PhotonTrackedTarget> targets = m_camera.GetLatestResult().GetTargets();
    for (int i = 0 /*startIndex*/; i < targets.size(); i++)
    {
        if (targets[i].GetPitch() > CAMERA_HIGH_LOW_DELIMITER)
        {
            return true;
        }
    }
    return false;
}