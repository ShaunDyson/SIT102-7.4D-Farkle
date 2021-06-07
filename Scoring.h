#pragma once
#include "Die.h"
#include <vector>
#include <map>

namespace Farkle::Scoring
{
        // Enum values are the amount of points you get for each type of straight.
    enum class Straight
    {
        None = 0,
        OneToFive = 500,
        TwoToSix = 750,
        OneToSix = 1500
    };

    // Returns the number of dice in the vector that have the given value.
    int get_value_count(const std::vector<Die>& dice, int value);

    // Returns a map of each possible die value and the number of dice in the vector that have that value.
    std::map<int, int> get_all_value_counts(const std::vector<Die>& dice);

    // Returns the greatest number of points the dice are worth and removes the dice that helpd score those points.
    int score_dice(std::vector<Die>& dice);

    // Finds the highest straight, removes the dice in that straight and returns the points it's worth.
    int score_straight(std::vector<Die>& dice);

    // Finds highest kinds for all values, removes the dice in them and returns the total ponits they're worth.
    int score_kinds(std::vector<Die>& dice);

    // Finds the highest kind for the given value, removes the dice in it and returns the points it's worth.
    int score_highest_kind(std::vector<Die>& dice, int value);

    // Finds all ones and fives, removes them and returns the total points they're worth.
    int score_ones_and_fives(std::vector<Die>& dice);

    // Returns true if the given element could be part of a straight in the vector.
    bool die_is_part_of_straight(const std::vector<Die>& dice, int index);

    Straight get_highest_straight(const std::vector<Die>& dice);

    // Returns the die value that the straight doesn't include.
    // Returns 0 if all values are included or if argument is None.
    int get_value_not_in_straight(Straight straight);

    Straight downgrade_straight(const Straight straight);
    void remove_dice_in_straight(std::vector<Die>& dice, Straight straight);
    void remove_all_dice_of_value(std::vector<Die>& dice, int value);
    void remove_die(std::vector<Die>& dice, int index);
}
