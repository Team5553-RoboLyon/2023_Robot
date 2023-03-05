#pragma once

#include <cmath>

namespace utils
{
    inline double Deadband(double value, double deadband = 0.1)
    {
        if (std::abs(value) < deadband)
        {
            return 0;
        }

        if (value >= 0)
        {
            return (value - deadband) / (1.0 - deadband);
        }
        else
        {
            return (value + deadband) / (1.0 - deadband);
        }
    }
} // namespace utils