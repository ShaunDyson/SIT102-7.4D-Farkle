#pragma once
#include "types.h"
#include <array>

#define DIE_FILL_WIDTH          100
#define DIE_BORDER_WIDTH        4
#define DIE_FULL_WIDTH          (DIE_FILL_WIDTH + DIE_BORDER_WIDTH * 2)
#define DIE_HIGHLIGHT_WIDTH     6

namespace Farkle
{
    class DieImage
    {
    public:
        DieImage(bool black = false);
        void set_face(int value);
        void draw(double x, double y, bool highlight = false) const;

    private:
        void set_dot_draw_points();
        void draw_dots(double x, double y) const;

        int m_value;
        bool m_dots_to_draw[3][3];
        point_2d m_dot_draw_points[3][3];
        color m_colour;
        color m_dot_colour;
    };
}
