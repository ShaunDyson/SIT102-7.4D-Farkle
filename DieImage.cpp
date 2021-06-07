#include "DieImage.h"
#include "splashkit.h"

#define DIE_DOT_SIZE        10

#define ONE_FIFTH_WDITH     (DIE_FILL_WIDTH / 5.0)
#define FOUR_FIFTHS_WIDTH   (DIE_FILL_WIDTH - ONE_FIFTH_WDITH)
#define HALF_WIDTH          (DIE_FILL_WIDTH / 2.0)
#define ONE_TENTH_WIDTH     (DIE_FILL_WIDTH / 10.0)
#define THREE_TENTH_WIDTH   (3 * DIE_FILL_WIDTH / 10.0)
#define SEVEN_TENTH_WIDTH   (DIE_FILL_WIDTH - THREE_TENTH_WIDTH)

namespace Farkle
{
    DieImage::DieImage(const bool black) :
        m_value{},
        m_dots_to_draw{},
        m_dot_draw_points{},
        m_colour{ black ? COLOR_BLACK : COLOR_WHITE },
        m_dot_colour{ black ? COLOR_WHITE : COLOR_BLACK }
    {
        set_dot_draw_points();
    }

    void DieImage::set_face(const int value)
    {
        // array makes it easy to check the value against multiple possibilities
        // element 0 is ignored for clarity
        bool v[7]{};
        v[value] = true;
        const bool v4_5_6{ v[4] || v[5] || v[6] };
        const bool v3_4_5_6{ v[3] || v4_5_6 };

        // each element in the 3x3 array represents a dot on a dice
        // choose which dots to draw based on current value
        // for example, if value is 4, 5 or 6 the top left dot (0,0) should be drawn
        m_dots_to_draw[0][0] = v4_5_6;
        m_dots_to_draw[0][1] = v[6];
        m_dots_to_draw[0][2] = v3_4_5_6;

        m_dots_to_draw[1][0] = v[2];
        m_dots_to_draw[1][1] = v[1] || v[3] || v[5];
        m_dots_to_draw[1][2] = v[2];

        m_dots_to_draw[2][0] = v3_4_5_6;
        m_dots_to_draw[2][1] = v[6];
        m_dots_to_draw[2][2] = v4_5_6;
    }

    void DieImage::set_dot_draw_points()
    {
        m_dot_draw_points[0][0] = { ONE_FIFTH_WDITH,    ONE_FIFTH_WDITH     };
        m_dot_draw_points[0][1] = { HALF_WIDTH,         ONE_FIFTH_WDITH     };
        m_dot_draw_points[0][2] = { FOUR_FIFTHS_WIDTH,  ONE_FIFTH_WDITH     };
        m_dot_draw_points[1][0] = { THREE_TENTH_WIDTH,  SEVEN_TENTH_WIDTH   };
        m_dot_draw_points[1][1] = { HALF_WIDTH,         HALF_WIDTH          };
        m_dot_draw_points[1][2] = { SEVEN_TENTH_WIDTH,  THREE_TENTH_WIDTH   };
        m_dot_draw_points[2][0] = { ONE_FIFTH_WDITH,    FOUR_FIFTHS_WIDTH   };
        m_dot_draw_points[2][1] = { HALF_WIDTH,         FOUR_FIFTHS_WIDTH   };
        m_dot_draw_points[2][2] = { FOUR_FIFTHS_WIDTH,  FOUR_FIFTHS_WIDTH   };
    }

    void DieImage::draw(double x, double y, const bool highlight) const
    {
        const point_2d fill_pos{ x + DIE_BORDER_WIDTH, y + DIE_BORDER_WIDTH };
        const rectangle die_border{ x, y, DIE_FULL_WIDTH, DIE_FULL_WIDTH };
        const rectangle die_fill{ fill_pos.x, fill_pos.y, DIE_FILL_WIDTH, DIE_FILL_WIDTH };

        if (highlight)
        {
            point_2d highlight_pos{ x - DIE_HIGHLIGHT_WIDTH, y - DIE_HIGHLIGHT_WIDTH };
            const double highlight_size{ DIE_FULL_WIDTH + DIE_HIGHLIGHT_WIDTH * 2};
            rectangle highlight_shape{ highlight_pos.x, highlight_pos.y, highlight_size, highlight_size };
            fill_rectangle(COLOR_ORANGE, highlight_shape);
        }

        fill_rectangle(COLOR_BLACK, die_border);
        fill_rectangle(m_colour, die_fill);
        draw_dots(x, y);
    }

    void DieImage::draw_dots(double x, double y) const
    {
        x += DIE_BORDER_WIDTH;
        y += DIE_BORDER_WIDTH;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (m_dots_to_draw[i][j])
                {
                    const point_2d& draw_point{ m_dot_draw_points[i][j] };
                    fill_circle(m_dot_colour, x + draw_point.x, y + draw_point.y, DIE_DOT_SIZE);
                }
            }
        }
    }
}
