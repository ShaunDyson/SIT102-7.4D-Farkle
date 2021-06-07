#include "Bot.h"
#include "Timer.h"
#include <map>

#define DELAY_LENGTH 25

namespace Farkle
{
    Bot::Bot(const int point_goal, const Player::Type player_type) :
        Player{ player_type },
        point_goal{ point_goal },
        m_to_select{}
    {
    }

    Bot::~Bot()
    {

    }

    Player::Move Bot::get_move()
    {
        if (!delay())
        {
            const Move select_move{ select_die() };

            if (select_move !=  Move::None)
            {
                return select_move;
            }
            else if (no_dice_selected())
            {
                choose_dice();
            }
            else
            {
                return pass_or_roll();
            }
        }

        return Move::None;
    }

    bool Bot::delay() const
    {
        static Timer timer{ DELAY_LENGTH };

        if (timer.tick())
        {
            timer.reset(DELAY_LENGTH);
            return false;
        }

        return true;
    }

    void Bot::choose_dice()
    {
        std::vector<Die> choices{ get_dice() };
        const bool straight{ select_highest_straight(choices) };
        select_all_scorable_dice(choices, straight);
        ready_selections(choices);
    }

    Player::Move Bot::pass_or_roll() const
    {
        const int unselected{ get_unselected_count(get_dice()) };
        const bool reroll{ unselected > 2 || unselected == 0 };
        const bool cant_win{ get_all_points() < point_goal };
        return reroll && cant_win ? Player::Move::Roll : Player::Move::Pass;
    }

    void Bot::ready_selections(const std::vector<Die>& selections)
    {
        // dice are selected starting from the end of the vector so we will fill it backwards
        // this means the selections will appear left to right in the gui
        for (int i = selections.size() - 1; i >= 0; i--)
        {
            if (selections[i].selected)
            {
                m_to_select.push_back(i);
            }
        }
    }

    bool Bot::select_highest_straight(std::vector<Die>& selections) const
    {
        typedef Scoring::Straight Straight;

        const Straight straight{ Scoring::get_highest_straight(selections) };

        if (straight == Straight::None)
        {
            return false;
        }

        select_dice_in_straight(selections, straight);
        return true;
    }

    void Bot::select_dice_in_straight(std::vector<Die>& selections, const Scoring::Straight straight) const
    {
        int value_to_skip{ Scoring::get_value_not_in_straight(straight) };
        std::map<int, bool> selected_values{};

        for (int i = 1; i <= 6; i++)
        {
            selected_values.emplace_hint(selected_values.end(), i, false);
        }

        for (int i = 0; i < selections.size(); i++)
        {
            Die& die{ selections[i] };

            if (!die.selected && die.value != value_to_skip && !selected_values[die.value])
            {
                die.selected = true;
                selected_values[die.value] = true;
            }
        }
    }

    void Bot::select_all_scorable_dice(std::vector<Die>& selections, const bool is_straight) const
    {
        for (int i = 0; i < selections.size(); i++)
        {
            Die& die{ selections[i] };
            die.selected = die.selected || (!is_straight && can_select_die(i));
            die.selected = die.selected || die.value == 1 || die.value == 5;
        }
    }

    int Bot::get_unselected_count(const std::vector<Die>& selections) const
    {
        int count{};

        for (const Die& die : selections)
        {
            count += !die.selected;
        }

        return count;
    }

    Player::Move Bot::select_die()
    {
        if (!m_to_select.empty())
        {
            const int end{ static_cast<int>(m_to_select.size() - 1) };
            const Player::Move move{ static_cast<Player::Move>(m_to_select[end]) };
            m_to_select.pop_back();
            return move;
        }

        return Move::None;
    }
}
