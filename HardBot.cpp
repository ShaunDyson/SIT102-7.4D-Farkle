#include "HardBot.h"

namespace Farkle
{
    HardBot::HardBot(const int point_goal) :
        Bot{ point_goal, Player::Type::HardBot }
    {
    }

    HardBot::~HardBot()
    {
    }

    void HardBot::choose_dice()
    {
        std::vector<Die> choices{ get_dice() };
        const bool straight{ select_highest_straight(choices) };
        select_kinds(choices);
        select_other_dice(choices, straight);
        ready_selections(choices);
    }

    Player::Move HardBot::pass_or_roll() const
    {
        return will_pass_or_roll(get_dice());
    }

    Player::Move HardBot::will_pass_or_roll(const std::vector<Die>& selections) const
    {
        const int unselected{ get_unselected_count(selections) };
        const int points_at_risk{ get_banked_points() + get_selected_points() };

        int min_dice_to_reroll{ points_at_risk >= 400 ? 3 : 2 };
        min_dice_to_reroll += points_at_risk >= 600;
        min_dice_to_reroll += points_at_risk >= 1000;

        const bool reroll{ unselected >= min_dice_to_reroll || unselected == 0 };
        const bool cant_win{ get_all_points() < point_goal };
        return reroll && cant_win ? Player::Move::Roll : Player::Move::Pass;
    }

    void HardBot::select_kinds(std::vector<Die>& selections) const
    {
        std::map<int, int> value_counts{ Scoring::get_all_value_counts(selections) };

        if (value_counts[2] > 2)
        {
            // three twos aren't worth much so only select twos if there are at least four
            // or if we have already scored a couple of dice
            if (value_counts[2] > 3 || selections.size() < 5)
            {
                select_all_dice_of_value(selections, 2);
            }
        }

        for (int i = 1; i <= 6; i++)
        {
            if (value_counts[i] > 2 && i != 2)
            {
                select_all_dice_of_value(selections, i);
            }
        }
    }

    void HardBot::select_all_dice_of_value(std::vector<Die>& selections, const int value) const
    {
        for (Die& die : selections)
        {
            die.selected = die.selected || die.value == value;
        }
    }

    void HardBot::select_other_dice(std::vector<Die>& selections, bool is_straight) const
    {
        if (no_dice_chosen(selections))
        {
            select_single_one_or_five(selections);
        }

        if (get_unselected_count(selections) <= 3)
        {
            select_all_ones(selections);
        }

        const bool can_win{ get_all_points() >= point_goal - 100 };
        const bool will_pass{ will_pass_or_roll(selections) == Player::Move::Pass };

        if (no_dice_chosen(selections) || will_pass || can_win)
        {
            select_all_scorable_dice(selections, is_straight);
        }
    }

    void HardBot::select_single_one_or_five(std::vector<Die>& selections) const
    {
        if (!select_single_die_of_value(selections, 1))
        {
            select_single_die_of_value(selections, 5);
        }
    }

    bool HardBot::select_single_die_of_value(std::vector<Die>& selections, const int value) const
    {
        for (Die& die : selections)
        {
            if (!die.selected && die.value == value)
            {
                die.selected = true;
                return true;
            }
        }

        return false;
    }

    void HardBot::select_all_ones(std::vector<Die>& selections) const
    {
        for (Die& die : selections)
        {
            die.selected = die.selected || die.value == 1;
        }
    }

    bool HardBot::no_dice_chosen(const std::vector<Die>& selections) const
    {
        for (const Die& die : selections)
        {
            if (die.selected)
            {
                return false;
            }
        }

        return true;
    }
}
