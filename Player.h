#pragma once
#include "Die.h"
#include <vector>

namespace Farkle
{
    class Player
    {
    public:
        enum class Type
        {
            Human,
            Bot,
            HardBot
        };

        enum class Move
        {
            SelectDie0 = 0,
            SelectDie1 = 1,
            SelectDie2 = 2,
            SelectDie3 = 3,
            SelectDie4 = 4,
            SelectDie5 = 5,
            Pass,
            Roll,
            Bust,
            Win,
            None
        };

        Player(Type type);
        virtual ~Player();

        inline const std::vector<Die>& get_dice() const { return m_dice; }
        inline int get_total_points() const { return m_total_points; }
        inline int get_banked_points() const { return m_banked_points; }
        inline int get_selected_points() const { return m_selected_points; }
        inline int get_all_points() const { return m_total_points + m_banked_points + m_selected_points; }

        Move prompt();
        Move try_select(Move move);
        Move try_roll();
        Move try_pass();
        void start_turn();
        void end_turn(int point_goal);

        const Type type;

    protected:
        bool can_select_die(int index) const;
        bool no_dice_selected() const;
        std::vector<Die> get_selected_dice() const;

    private:
        void prepare_roll();
        void roll();
        void bust();
        void pass();

        bool try_bank_dice();
        void remove_selected_dice();
        void fill_dice_set();

        std::vector<Die> m_dice;
        int m_total_points;
        int m_banked_points;
        int m_selected_points;
        bool m_bust_pending;
        bool m_roll_pending;
        bool m_pass_pending;
    };
}
