#include "Gui.h"
#include "splashkit.h"

#define HUD_RATIO (1.0/4.0)
#define DICE_GAP (DIE_FULL_WIDTH / 3.0 )
#define GUI_BORDER_WIDTH 8

#define TEXT_SMALL 28
#define TEXT_MED 48
#define TEXT_LARGE 100
#define TEXT_GAP 80

#define PASS_TEXT "PASS"
#define BUST_TEXT "BUST"
#define WIN_TEXT "YOU WIN!"
#define DELAY 30
#define ROLL_ANIMATION_LENGTH 90
#define ROLL_INTERVAL 10
#define STATUS_SCREENTIME 90
#define VICTORY_SCREENTIME 120

namespace Farkle
{
    double Gui::get_hud_width()
    {
        return screen_width() * HUD_RATIO;
    }

    double Gui::x_to_draw_at_centre(const double shape_width, const rectangle& container)
    {
        return container.x + (container.width - shape_width) / 2.0;
    }

    double Gui::y_to_draw_at_centre(const double shape_height, const rectangle& container)
    {
        return container.y + (container.height - shape_height) / 2.0;
    }

    point_2d Gui::point_to_draw_at_centre(const vector_2d shape_size, const rectangle& container)
    {
        const double x{ x_to_draw_at_centre(shape_size.x, container) };
        const double y{ y_to_draw_at_centre(shape_size.y, container) };
        return { x, y };
    }

    vector_2d Gui::text_size(const std::string& text, const int font_size)
    {
        const double width{ static_cast<double>(text_width(text, FONT, font_size)) };
        const double height{ static_cast<double>(text_height(text, FONT, font_size)) };
        return { width, height };
    }

    rectangle Gui::centred_text_box(const std::string& text, const int font_size, const rectangle& container)
    {
        const vector_2d size{ text_size(text, font_size) };
        const point_2d pos{ point_to_draw_at_centre(size, container) };
        return { pos.x, pos.y, size.x, size.y };
    }

    Gui::Gui(const Game& game) :
        game_ref{ game },
        m_player1_view{ game, Game::PlayerNumber::Player1 },
        m_player2_view{ game, Game::PlayerNumber::Player2 }
    {
        turn_player_view().trigger_event(Player::Move::Roll);
    }

    void Gui::update(const Player::Move move)
    {
        turn_player_view().trigger_event(move);
        clear_screen(COLOR_GREEN);
        draw();
        refresh_screen(60);
    }

    void Gui::draw()
    {
        fill_rectangle(COLOR_DARK_BLUE, 0, 0, get_hud_width(), screen_height());
        m_player1_view.draw();
        m_player2_view.draw();
        draw_dividers();
        draw_border();
    }

    void Gui::draw_border() const
    {
        fill_rectangle(COLOR_BLACK, 0, 0, GUI_BORDER_WIDTH, screen_height());
        fill_rectangle(COLOR_BLACK, 0, 0, screen_width(), GUI_BORDER_WIDTH);

        fill_rectangle(COLOR_BLACK, screen_width(), 0, -GUI_BORDER_WIDTH, screen_height());
        fill_rectangle(COLOR_BLACK, 0, screen_height(), screen_width(), -GUI_BORDER_WIDTH);
    }

    void Gui::draw_dividers() const
    {
        point_2d pos{ 0, (screen_height() - GUI_BORDER_WIDTH) / 2.0 };
        fill_rectangle(COLOR_BLACK, pos.x, pos.y, screen_width(), GUI_BORDER_WIDTH);

        pos = { get_hud_width() - GUI_BORDER_WIDTH, 0 };
        fill_rectangle(COLOR_BLACK, pos.x, pos.y, GUI_BORDER_WIDTH, screen_height());
    }

    const Gui::PlayerView& Gui::turn_player_view() const
    {
        const bool player_1_turn{ game_ref.get_turn_player() == Game::PlayerNumber::Player1 };
        return player_1_turn ? m_player1_view : m_player2_view;
    }

    Gui::PlayerView& Gui::turn_player_view()
    {
        const bool player1_turn{ game_ref.get_turn_player() == Game::PlayerNumber::Player1 };
        return player1_turn ? m_player1_view : m_player2_view;
    }

    Gui::PlayerView::PlayerView(const Game& game, const Game::PlayerNumber player_number) :
        player_number{ player_number },
        game_ref{ game },
        player_ref{ game.get_player(player_number) },
        point_goal{ game.point_goal },
        m_is_rolling{},
        m_is_passing{},
        m_is_bust{},
        m_victory{},
        m_left_view{},
        m_right_view{},
        m_dice_images{},
        m_delay_timer{ DELAY },
        m_display_timer{ STATUS_SCREENTIME }
    {
        m_left_view.x = GUI_BORDER_WIDTH;
        m_left_view.y = GUI_BORDER_WIDTH;
        m_left_view.width = Gui::get_hud_width() - GUI_BORDER_WIDTH;
        m_left_view.height = screen_height() / 2.0 - GUI_BORDER_WIDTH;

        m_right_view.x = Gui::get_hud_width();
        m_right_view.y = GUI_BORDER_WIDTH;
        m_right_view.width = screen_width() - m_right_view.x - GUI_BORDER_WIDTH;
        m_right_view.height = (screen_height() - DIE_BORDER_WIDTH) / 2.0 - DIE_BORDER_WIDTH;

        if ( player_number == Game::PlayerNumber::Player1 )
        {
            m_left_view.y += (screen_height() + GUI_BORDER_WIDTH) / 2.0;
            m_right_view.y += (screen_height() - GUI_BORDER_WIDTH) / 2.0;
        }
        else
        {
            for (int i = 0; i < m_dice_images.size(); i++)
            {
                m_dice_images[i] = { true };
            }
        }
    }

    void Gui::PlayerView::trigger_event(const Player::Move move)
    {
        typedef Player::Move Move;

        switch (move)
        {
        case Move::Roll:
            m_is_rolling = true;
            reset_timers(DELAY, ROLL_ANIMATION_LENGTH);
            break;
        case Move::Pass:
            m_is_passing = true;
            reset_timers(DELAY, STATUS_SCREENTIME);
            break;
        case Move::Bust:
            m_is_bust = true;
            reset_timers(DELAY, STATUS_SCREENTIME);
            break;
        case Move::Win:
            m_victory = true;
            reset_timers(DELAY, VICTORY_SCREENTIME);
            break;
        default:
            break;
        }
    }

    void Gui::PlayerView::draw()
    {
        const bool is_turn_player{ player_number == game_ref.get_turn_player() };
        const color colour{ is_turn_player ? COLOR_LIGHT_GREEN : COLOR_GREEN };
        fill_rectangle(colour, m_right_view.x, m_right_view.y, m_right_view.width, m_right_view.height);

        draw_points();

        if (m_is_rolling)
        {
            play_roll_animation();
            return;
        }
        else if(m_is_bust && show_status(m_is_bust))
        {
            draw_status(BUST_TEXT, COLOR_RED);
        }
        else if (m_is_passing && show_status(m_is_passing))
        {
            draw_status(PASS_TEXT, COLOR_TEAL);
        }
        else if (m_victory && show_status(m_victory))
        {
            draw_victory();
            return;
        }

        draw_dice(player_ref.get_dice());
    }

    void Gui::PlayerView::draw_dice(const std::vector<Die>& dice)
    {
        const double set_width{ dice.size() * (DIE_FULL_WIDTH + DICE_GAP) - DICE_GAP };
        const vector_2d set_size{ set_width, DIE_FULL_WIDTH };
        const point_2d dice_start_pos{ point_to_draw_at_centre(set_size, m_right_view) };

        for (int i = 0; i < dice.size(); i++)
        {
            const double x_adjust{ (DIE_FULL_WIDTH + DICE_GAP) * i };
            m_dice_images[i].set_face(dice[i].value);
            m_dice_images[i].draw(dice_start_pos.x + x_adjust, dice_start_pos.y, dice[i].selected);
        }
    }

    void Gui::PlayerView::draw_points() const
    {
        draw_selected_points();
        draw_total_and_banked_points();
    }

    void Gui::PlayerView::draw_selected_points() const
    {
        const vector_2d size{ text_size("00000", TEXT_SMALL) };
        point_2d pos{ Gui::x_to_draw_at_centre(size.x, m_right_view), m_right_view.y - GUI_BORDER_WIDTH };
        const rectangle shape{ pos.x, pos.y, size.x, size.y };

        fill_rectangle(COLOR_BLACK, shape.x - 2, shape.y, shape.width + 4, shape.height + 2);
        fill_rectangle(COLOR_ORANGE, shape.x, shape.y, shape.width, shape.height);

        const std::string text{ std::to_string(player_ref.get_selected_points()) };
        pos = { point_to_draw_at_centre(text_size(text, TEXT_SMALL), shape) };
        draw_text( text, COLOR_WHITE, FONT, TEXT_SMALL, pos.x, pos.y);
    }

    void Gui::PlayerView::draw_total_and_banked_points() const
    {
        using namespace std;

        string total_text{ "Total: " + to_string(player_ref.get_total_points()) + " / " + to_string(point_goal)};
        const string banked_text{ "Banked: " + to_string(player_ref.get_banked_points()) };

        vector_2d size{ text_size(total_text, TEXT_SMALL) };
        size.y += TEXT_GAP;

        const point_2d pos{ Gui::point_to_draw_at_centre(size, m_left_view) };
        const rectangle shape{ pos.x, pos.y, size.x, size.y };

        draw_text(total_text, COLOR_WHITE, FONT, TEXT_SMALL, shape.x, shape.y);
        draw_text(banked_text, COLOR_WHITE, FONT, TEXT_SMALL, shape.x, shape.y + TEXT_GAP);
    }

    void Gui::PlayerView::play_roll_animation()
    {
        static Timer roll_interval{ 0 };
        static std::vector<Die> random_dice{};

        while (random_dice.size() < player_ref.get_dice().size())
        {
            random_dice.push_back({});
        }

        if (roll_interval.tick())
        {
            randomise_dice_values(random_dice);
            roll_interval.reset(ROLL_INTERVAL);
        }
        else if (m_display_timer.get() && roll_interval.get())
        {
            m_is_rolling = false;
            draw_dice(random_dice);
            random_dice.clear();
            return;
        }

        if (m_delay_timer.tick())
        {
            draw_dice(random_dice);
            m_display_timer.tick();
            return;
        }

        draw_dice(player_ref.get_dice());
    }

    bool Gui::PlayerView::show_status(bool& status)
    {
        if (!m_delay_timer.tick())
        {
            return false;
        }
        else if (m_display_timer.tick())
        {
            status = false;
            return false;
        }

        return true;
    }

    void Gui::PlayerView::draw_status(const std::string& text, const color& colour) const
    {
        rectangle shape{ centred_text_box(text, TEXT_MED, m_right_view) };
        shape.y = get_bottom() - shape.height - 32;
        fill_rectangle(COLOR_BLACK, shape.x - 16, shape.y - 12, shape.width + 32, shape.height + 24);
        fill_rectangle(colour, shape.x - 12, shape.y - 8, shape.width + 24, shape.height + 16);
        draw_text(text, COLOR_WHITE, FONT, TEXT_MED, shape.x, shape.y);
    }

    void Gui::PlayerView::draw_victory() const
    {
        const rectangle shape{ centred_text_box(WIN_TEXT, TEXT_LARGE, m_right_view) };
        fill_rectangle(COLOR_BLACK, shape.x - 24, shape.y - 16, shape.width + 48, shape.height + 32);
        fill_rectangle(COLOR_MAGENTA, shape.x - 16, shape.y - 8, shape.width + 32, shape.height + 16);
        draw_text(WIN_TEXT, COLOR_WHITE, FONT, TEXT_LARGE, shape.x, shape.y);
    }

    void Gui::PlayerView::randomise_dice_values(std::vector<Die>& dice) const
    {
        for (Die& die : dice)
        {
            die.roll();
        }
    }

    void Gui::PlayerView::reset_timers(const unsigned delay, const unsigned display)
    {
        m_delay_timer.reset(delay);
        m_display_timer.reset(display);
    }
}
