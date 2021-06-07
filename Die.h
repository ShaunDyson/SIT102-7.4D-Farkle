#pragma once

namespace Farkle
{
    struct Die
    {
        Die();
        void roll();

        int value;
        bool selected;
    };
}
