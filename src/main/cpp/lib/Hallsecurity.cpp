//
// Created by eymeric on 13/03/2022.
//

#include "lib/HallSecurity.h"
// ma petit modif dans le gros repo

HallSecurity::HallSecurity(int hallDIO, double tolerance)
{
    doubleHall = false;
    m_SensorHall1 = new frc::DigitalInput{hallDIO};
    m_tolerance = tolerance;
}

HallSecurity::HallSecurity(int hallDIO)
{
    doubleHall = false;
    m_SensorHall1 = new frc::DigitalInput{hallDIO};
    m_tolerance = 0.3;
}

HallSecurity::HallSecurity(int hallDIORight, int hallDIOLeft)
{
    doubleHall = true;

    m_SensorHall1 = new frc::DigitalInput{hallDIORight};
    m_SensorHall2 = new frc::DigitalInput{hallDIOLeft};

    m_tolerance = 0.3;
}

HallSecurity::HallSecurity(int hallDIORight, int hallDIOLeft, double tolerance)
{
    doubleHall = true;
    m_SensorHall1 = new frc::DigitalInput{hallDIORight};
    m_SensorHall2 = new frc::DigitalInput{hallDIOLeft};
    m_tolerance = tolerance;
}

HallSecurity::~HallSecurity()
{
    delete m_SensorHall1;
    delete m_SensorHall2;
}

void HallSecurity::setInverted(bool inverted)
{
    m_inverted = inverted;
}

bool HallSecurity::ShouldIStop(double currentPosition, int outputSigne)
{
    if (m_inverted)
    {
        outputSigne = -outputSigne;
    }
    m_DeltaPosition = currentPosition - m_PositionBefore;
    if (m_DeltaPosition > -m_tolerance && m_DeltaPosition < m_tolerance)
    {
        m_DeltaPosition = 0;
    }
    m_PositionBefore = currentPosition;
    bool retour = false;
    switch (m_state)
    {
    case HallSecurity::state::maxMin_Direction:
        if (MagnetDetected())
        {
            if (doubleHall)
            {
                if (outputSigne > 0)
                {
                    retour = true;
                }
                m_state = HallSecurity::state::max_Direction;
            }
            else if (m_DeltaPosition > 0)
            {
                if (outputSigne > 0)
                {
                    retour = true;
                }
                m_state = HallSecurity::state::max_Direction;
            }
            else if (m_DeltaPosition < 0)
            {
                if (outputSigne < 0)
                {
                    retour = true;
                }
                m_state = HallSecurity::state::min_Direction;
            }
        }
        if (doubleHall && MagnetDetected(true))
        {
            if (outputSigne < 0)
            {
                retour = true;
            }
            m_state = HallSecurity::state::min_Direction;
        }
        break;
    case HallSecurity::state::min_Direction:
        if (outputSigne < 0)
        {
            retour = true;
        }
        if (!MagnetDetected())
        {
            m_state = HallSecurity::state::maxMin_Direction;
        }
        break;

    case HallSecurity::state::max_Direction:
        if (outputSigne > 0)
        {
            retour = true;
        }
        if ((!doubleHall && !MagnetDetected()) || (doubleHall && !MagnetDetected(true)))
        {
            m_state = HallSecurity::state::maxMin_Direction;
        }
        break;
    default:
        break;
    }
    return retour;
}

bool HallSecurity::MagnetDetected(bool two)
{
    if (doubleHall)
    {
        if (two)
        {
            return !m_SensorHall2->Get();
        }
        else
        {
            return !m_SensorHall1->Get();
        }
    }
    else
    {
        return !m_SensorHall1->Get();
    }
}

std::string HallSecurity::stateToString(HallSecurity::state state)
{
    switch (state)
    {
    case HallSecurity::state::min_Direction:
        return "min_Direction";
    case HallSecurity::state::max_Direction:
        return "max_Direction";
    case HallSecurity::state::maxMin_Direction:
        return "maxMin_Direction";
    default:
        return "";
    }
}