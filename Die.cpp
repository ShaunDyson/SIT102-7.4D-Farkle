#include "Die.h"
#include "splashkit.h"

namespace Farkle
{
    Die::Die() :
        value{ 0 },
        selected{ false }
    {
        roll();
    }

    void Die::roll()
    {
        value = rnd(1, 7);
    }
}
