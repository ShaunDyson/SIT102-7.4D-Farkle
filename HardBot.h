#pragma once
#include "Bot.h"

namespace Farkle
{
    class HardBot : public Bot
    {
    public:
        HardBot(int point_goal);
        ~HardBot();

    private:
        void choose_dice() override;
        Player::Move pass_or_roll() const override;
        Player::Move will_pass_or_roll(const std::vector<Die>& selections) const;

        void select_kinds(std::vector<Die>& selections) const;
        void select_all_dice_of_value(std::vector<Die>& selections, int value) const;
        void select_other_dice(std::vector<Die>& selections, bool is_straight) const;
        void select_single_one_or_five(std::vector<Die>& selections) const;
        bool select_single_die_of_value(std::vector<Die>& selections, int value) const;
        void select_all_ones(std::vector<Die>& selections) const;

        bool no_dice_chosen(const std::vector<Die>& selections) const;
    };
}
