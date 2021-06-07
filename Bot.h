#pragma once
#include "Player.h"
#include "Scoring.h"

namespace Farkle
{
    class Bot : public Player
    {
    public:
        Bot(int point_goal, Player::Type player_type = Player::Type::Bot);
        virtual ~Bot();

        Player::Move get_move();

        const int point_goal;

    protected:
        bool select_highest_straight(std::vector<Die>& selections) const;
        void select_all_scorable_dice(std::vector<Die>& selections, bool is_straight) const;
        int get_unselected_count(const std::vector<Die>& selections) const;
        void ready_selections(const std::vector<Die>& selections);

    private:
        bool delay() const;

        virtual void choose_dice();
        virtual Player::Move pass_or_roll() const;

        void select_dice_in_straight(std::vector<Die>& selections, Scoring::Straight straight) const;
        Player::Move select_die();

        std::vector<int> m_to_select;
    };
}
