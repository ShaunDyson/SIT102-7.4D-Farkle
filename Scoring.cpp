#include "Scoring.h"
#include <map>
#include "splashkit.h"

namespace Farkle::Scoring
{
    void remove_all_dice_of_value(std::vector<Die>& dice, int value);

    int get_value_count(const std::vector<Die>& dice, const int value)
    {
        int count{ 0 };

        for (int i = 0; i < dice.size(); i++)
        {
            count += dice[i].value == value;
        }

        return count;
    }

    std::map<int, int> get_all_value_counts(const std::vector<Die>& dice)
    {
        std::map<int, int> value_counts{};

        for (int i = 1; i <= 6; i++)
        {
            value_counts.emplace(i, get_value_count(dice, i));
        }

        return value_counts;
    }

    int score_dice(std::vector<Die>& dice)
    {
        int points{};
        points += score_straight(dice);
        points += score_kinds(dice);
        points += score_ones_and_fives(dice);
        return points;
    }

    int score_straight(std::vector<Die>& dice)
    {
        Straight straight{ get_highest_straight(dice) };
        remove_dice_in_straight(dice, straight);
        return static_cast<int>(straight);
    }

    int score_kinds(std::vector<Die>& dice)
    {
        int points{};

        for (int i = 1; i <= 6; i++)
        {
            points += score_highest_kind(dice, i);
        }

        return points;
    }

    int score_highest_kind(std::vector<Die>& dice, const int value)
    {
        const int count{ get_value_count(dice, value) };
        int points{};

        if (count >= 3) // then there is at least a three of a kind
        {
            // three of a kind is worth 100 multiplied by die value, ones are special
            points = (value == 1 ? 1000 : value * 100);

            // a four of a kind is worth double a three of a kind, five of a kind is worth double again, etc
            for (int i = 3; i < count; i++)
            {
                points *= 2;
            }

            remove_all_dice_of_value(dice, value);
        }

        return points;
    }

    int score_ones_and_fives(std::vector<Die>& dice)
    {
        int points{};

        for (int i = 0; i < dice.size(); i++)
        {
            int die_value{ dice[i].value };

            switch (die_value)
            {
            case 1: // ones are worth 100 points
                points += 50;
            case 5: // fives are worth 50 points
                points += 50;
                remove_die(dice, i);
                i--;
            }
        }

        return points;
    }

    bool die_is_part_of_straight(const std::vector<Die>& dice, const int index)
    {
        const Straight straight{ get_highest_straight(dice) };
        const int die_value{ dice[index].value };

        switch(straight)
        {
            case Straight::OneToSix:
                return true;

            case Straight::OneToFive:
                return (1 <= die_value && die_value <= 5);

            case Straight::TwoToSix:

                return (2 <= die_value && die_value <= 6);

            default:
                return false;
        }
    }

    Straight get_highest_straight(const std::vector<Die>& dice)
    {
        std::map<int, int> value_counts{ get_all_value_counts(dice) };

        // all straights contain values 2 - 5
        for (int i = 2; i <= 5; i++)
        {
            if (!value_counts[i])
            {
                return Straight::None;
            }
        }

        Straight straight{ Straight::OneToSix };

        if (!value_counts[6])
        {
            straight = downgrade_straight(straight);
            straight = downgrade_straight(straight);
        }
        if (!value_counts[1])
        {
            straight = downgrade_straight(straight);
        }

        return straight;
    }

    int get_value_not_in_straight(const Straight straight)
    {
        switch (straight)
        {
        case Straight::OneToFive:
            return 6;

        case Straight::TwoToSix:
            return 1;

        default:
            return 0;
        }
    }

    Straight downgrade_straight(const Straight straight)
    {
        switch (straight)
        {
        case Straight::OneToSix:
            return Straight::TwoToSix;

        case Straight::TwoToSix:
            return Straight::OneToFive;

        default:
            return Straight::None;
        }
    }

    void remove_dice_in_lesser_straight(std::vector<Die>& dice, const Straight straight)
    {
        // lesser straights are One to Five and Two - Six
        const int ignored_value{ straight == Straight::OneToFive ? 6 : 1 };
        std::map<int, bool> removed_values{};

        for (int i = 1; i <= 6; i++)
        {
            removed_values.emplace_hint(removed_values.end(), i, false);
        }

        for (int i = 0; i < dice.size(); i++)
        {
            const int die_value{ dice[i].value };

            if (die_value != ignored_value && !removed_values[die_value])
            {
                remove_die(dice, i);
                removed_values[die_value] = true;
                i--;
            }
        }
    }

    void remove_dice_in_straight(std::vector<Die>& dice, const Straight straight)
    {
        switch (straight)
        {
        case Straight::OneToFive:
        case Straight::TwoToSix:
            remove_dice_in_lesser_straight(dice, straight);
            break;

        case Straight::OneToSix:
            dice.clear(); // all dice must be part of straight
            break;

        default:
            break;
        }
    }

    void remove_all_dice_of_value(std::vector<Die>& dice, const int value)
    {
        for (int i = 0; i < dice.size(); i++)
        {
            if (dice[i].value == value)
            {
                remove_die(dice, i);
                i--;
            }
        }
    }

    void remove_die(std::vector<Die>& dice, const int index)
    {
        dice[index] = dice[dice.size() - 1];
        dice.pop_back();
    }
}
