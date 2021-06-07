#pragma once
#include "DieImage.h"
#include "Game.h"
#include "Timer.h"
#include "types.h"
#include <array>

#define FONT "ariblk.ttf"

namespace Farkle
{
    class Gui
    {
    public:
        static double get_hud_width();

        // Returns the x coordinate of where to start drawing a shape to center it horizontally in the container.
        static double x_to_draw_at_centre(double shape_width, const rectangle& container);

        // Returns the y coordinate of where to start drawing a shape to center it vertically in the container.
        static double y_to_draw_at_centre(double shape_height, const rectangle& container);

        // Returns the point where to start drawing a shape if you want to center it in the container.
        static point_2d point_to_draw_at_centre(vector_2d shape_size, const rectangle& container);

        static vector_2d text_size(const std::string& text, int font_size);

        // Returns a shape sized to fit the given text and positioned in the centre of the container.
        static rectangle centred_text_box(const std::string& text, int font_size, const rectangle& container);

        Gui(const Game& game);
        void update(Player::Move move);
        inline bool is_busy() const { return turn_player_view().is_busy(); }

        const Game& game_ref;

    private:
        class PlayerView
        {
        public:
            PlayerView(const Game& game, Game::PlayerNumber player);

            inline bool is_busy() const { return m_is_rolling || m_is_passing || m_is_bust || m_victory; }
            void trigger_event(Player::Move move);

            void draw();

            const Game::PlayerNumber player_number;
            const Game& game_ref;
            const Player& player_ref;
            const int point_goal;

        private:
            inline double get_bottom() const { return m_right_view.y + m_right_view.height; }
            void draw_dice(const std::vector<Die>& dice);
            void draw_points() const;
            void draw_selected_points() const;
            void draw_total_and_banked_points() const;
            void play_roll_animation();
            bool show_status(bool& status);
            void draw_status(const std::string& text, const color& colour) const;
            void draw_victory() const;
            void randomise_dice_values(std::vector<Die>& dice) const;
            void reset_timers(unsigned delay, unsigned display);

            bool m_is_rolling;
            bool m_is_passing;
            bool m_is_bust;
            bool m_victory;
            rectangle m_left_view;
            rectangle m_right_view;
            std::array<DieImage, 6> m_dice_images;
            Farkle::Timer m_delay_timer;
            Farkle::Timer m_display_timer;
        };

        void draw();
        void draw_border() const;
        void draw_dividers() const;

        const PlayerView& turn_player_view() const;
        PlayerView& turn_player_view();

        PlayerView m_player1_view;
        PlayerView m_player2_view;
    };
}
