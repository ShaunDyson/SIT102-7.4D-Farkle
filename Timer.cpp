#include "Timer.h"

namespace Farkle
{
    Timer::Timer(const unsigned length) :
        time{ length }
    {
    }

    bool Timer::tick()
    {
        if (time == 0)
        {
            return true;
        }

        time--;
        return false;
    }

    void Timer::reset(const unsigned length)
    {
        time = length;
    }
}
