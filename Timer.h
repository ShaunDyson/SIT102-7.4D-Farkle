#pragma once

namespace Farkle
{
    class Timer
    {
    public:
        Timer(unsigned length);

        // Ticks the timer and returns whether it has reached 0
        bool tick();
        inline bool get() const { return time == 0; }

        void reset(unsigned length);

    private:
        unsigned time;
    };
}
