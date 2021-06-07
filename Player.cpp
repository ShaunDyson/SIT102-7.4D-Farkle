#include "Player.h"
#include "Scoring.h"
#include "splashkit.h"

namespace Farkle
{
    Player::Player(const Type type) :
        type{ type },
        m_dice{},
        m_total_points{},
        m_banked_points{},
        m_selected_points{},
        m_bust_pending{},
        m_roll_pending{},
        m_pass_pending{}
    {
        fill_dice_set();
    }

    Player::~Player()
    {
    }

    Player::Move Player::prompt()
    {
        if (m_roll_pending)
        {
            roll();
        }
        else if (m_bust_pending)
        {
            bust();
            return Move::Bust;
        }
        else if (m_pass_pending)
        {
            pass();
            return Move::Pass;
        }

        return Move::None;
    }

    Player::Move Player::try_select(const Move move)
    {
        const int n{ static_cast<int>(move) };

        if (0 <= n && n < m_dice.size() && can_select_die(n))
        {
            bool& selected{ m_dice[n].selected };
            selected = !selected;
            std::vector<Die> selected_dice{ get_selected_dice() };
            m_selected_points = Scoring::score_dice(selected_dice);
            return move;
        }

        return Move::None;
    }

    Player::Move Player::try_roll()
    {
        if (try_bank_dice())
        {
            prepare_roll();
            return Move::Roll;
        }

        return Move::None;
    }

    Player::Move Player::try_pass()
    {
        m_pass_pending = try_bank_dice();
        return Move::None;
    }

    void Player::start_turn()
    {
        prepare_roll();
    }

    void Player::end_turn(const int point_goal)
    {
        m_total_points += m_banked_points;
        m_banked_points = 0;

        if (m_total_points < point_goal)
        {
            fill_dice_set();
        }
    }

    bool Player::can_select_die(const int index) const
    {
        const int die_value{ m_dice[index].value };

        if (die_value == 1 || die_value == 5)
        {
            return true;
        }
        if (Scoring::get_value_count(m_dice, die_value) >= 3)
        {
            return true;
        }
        if (Scoring::die_is_part_of_straight(m_dice, index))
        {
            return true;
        }

        return false;
    };

    bool Player::no_dice_selected() const
    {
        for (const Die& die : m_dice)
        {
            if (die.selected)
            {
                return false;
            }
        }

        return true;
    }

    std::vector<Die> Player::get_selected_dice() const
    {
        std::vector<Die> selected{};

        for (const Die& die : m_dice)
        {
            if (die.selected)
            {
                selected.push_back(die);
            }
        }

        return selected;
    }

    void Player::prepare_roll()
    {
        m_roll_pending = true;

        if (m_dice.empty())
        {
            fill_dice_set();
        }
    }

    void Player::roll()
    {
        m_roll_pending = false;

        for (Die& die : m_dice)
        {
            die.selected = false;
            die.roll();
        }

        std::vector<Die> bust_test{ m_dice };
        m_bust_pending = !Scoring::score_dice(bust_test);
    }

    void Player::bust()
    {
        m_bust_pending = false;
        m_selected_points = 0;
        m_banked_points = 0;
    }

    void Player::pass()
    {
        m_pass_pending = false;
        m_banked_points += m_selected_points;
    }

    bool Player::try_bank_dice()
    {
        std::vector<Die> selected{ get_selected_dice() };

        if (!selected.empty()) // have to bank at least one die to pass or roll.
        {
            int points{ Scoring::score_dice(selected) };

            if (selected.empty()) // selections are only valid if no dice remain after scoring.
            {
                m_banked_points += points;
                m_selected_points = 0;
                remove_selected_dice();
                return true;
            }
        }

        return false;
    }

    void Player::remove_selected_dice()
    {
        for (int i = 0; i < m_dice.size(); i++)
        {
            if (m_dice[i].selected)
            {
                Scoring::remove_die(m_dice, i);
                i--;
            }
        }
    }

    void Player::fill_dice_set()
    {
        for (int i = m_dice.size(); i < 6; i++)
        {
            m_dice.push_back({});
        }
    }
}
