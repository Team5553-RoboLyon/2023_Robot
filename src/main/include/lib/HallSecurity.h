//
// Created by eymeric on 13/03/2022.
//
#pragma once

#include <frc/DigitalInput.h>


class HallSecurity
{

public:
    HallSecurity(int hallDIO);

    HallSecurity(int hallDIORight, int hallDIOLeft);

    HallSecurity(int hallDIORight, int hallDIOLeft, double tolerance);

    HallSecurity(int hallDIO, double tolerance);

    ~HallSecurity();

    void setInverted(bool inverted);

    bool ShouldIStop(double currentPosition, int outputSigne);

    bool MagnetDetected(bool two = false);

private:
    enum state
    {
        max_Direction,
        min_Direction,
        maxMin_Direction,
    };

    std::string stateToString(state state);

    frc::DigitalInput *m_SensorHall1;
    frc::DigitalInput *m_SensorHall2;
    bool doubleHall = false;

    double m_DeltaPosition = 0.0;
    double m_PositionBefore = 0.0;
    double m_tolerance;
    bool m_inverted = false;
    HallSecurity::state m_state = HallSecurity::state::maxMin_Direction;
};
