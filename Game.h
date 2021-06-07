#pragma once
#include "Player.h"

namespace Farkle
{
    class Game
    {
    public:
        enum class PlayerNumber
        {
            Player1,
            Player2
        };

        Game(Player::Type player1_type, Player::Type player2_type, int player2_start);
        ~Game();

        inline bool victory_achieved() const { return m_victory_achieved; }
        inline PlayerNumber get_turn_player() const { return m_turn_player; }
        const Player& get_player(PlayerNumber number) const;

        // Process the next action to be performed. Will decide if the move indicated is valid and handles it.
        // Returns the move actually made, which can be different, such as if a Bot needs to make its move.
        Player::Move prompt(Player::Move move);

        const int point_goal;

    private:
        Player* turn_player() const;

        void create_player(Player*& player, Player::Type type);

        Player::Move prompt_player();
        Player::Move get_bot_move();
        Player::Move process_move(Player::Move move);
        Player::Move process_select_die(Player::Move move);
        Player::Move process_roll();
        Player::Move process_pass();
        bool try_end_turn();
        bool check_for_victory();
        bool try_start_turn();
        void switch_turn_player();

        Player* m_player1;
        Player* m_player2;
        PlayerNumber m_turn_player;
        bool m_end_turn_pending;
        bool m_start_turn_pending;
        bool m_victory_achieved;
    };
}
