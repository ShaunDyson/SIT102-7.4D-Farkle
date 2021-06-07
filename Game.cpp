#include "Game.h"
#include "HardBot.h"
#include "splashkit.h"

namespace Farkle
{
    Game::Game(const Player::Type player1_type, const Player::Type player2_type, const int player2_start) :
        point_goal{ 3000 },
        m_player1{ nullptr },
        m_player2{ nullptr },
        m_turn_player{ player2_start ? PlayerNumber::Player2 : PlayerNumber::Player1 },
        m_end_turn_pending{},
        m_start_turn_pending{},
        m_victory_achieved{}
    {
        create_player(m_player1, player1_type);
        create_player(m_player2, player2_type);
        turn_player()->start_turn();
    }

    Game::~Game()
    {
        delete m_player1;
        delete m_player2;
    }

    const Player& Game::get_player(const PlayerNumber number) const
    {
        if (number == PlayerNumber::Player1)
        {
            return *m_player1;
        }
        else
        {
            return *m_player2;
        }
    }

    Player* Game::turn_player() const
    {
        if (m_turn_player == PlayerNumber::Player1)
        {
            return m_player1;
        }
        else
        {
            return m_player2;
        }
    }

    void Game::create_player(Player*& player, const Player::Type type)
    {
        typedef Player::Type Type;

        switch (type)
        {
        case Type::Human:
            player = new Player{ Type::Human };
            break;

        case Type::Bot:
            player = new Bot{ point_goal };
            break;

        case Type::HardBot:
            player = new HardBot{ point_goal };

        default:
            break;
        }
    }

    Player::Move Game::prompt(Player::Move move)
    {
        typedef Player::Move Move;

        const Move prompt_move{ prompt_player() };

        if (prompt_move != Move::None)
        {
            return prompt_move;
        }

        if (try_end_turn())
        {
            return Move::None;
        }
        if (check_for_victory())
        {
            return Move::Win;
        }
        if (try_start_turn())
        {
            return Move::Roll;
        }

        if (turn_player()->type != Player::Type::Human)
        {
            move = get_bot_move();
        }

        return process_move(move);
    }

    Player::Move Game::prompt_player()
    {
        typedef Player::Move Move;

        const Move move{ turn_player()->prompt() };

        if (move == Move::Bust || move == Move::Pass)
        {
            m_end_turn_pending = true;
            return move;
        }

        return Move::None;
    }

    Player::Move Game::get_bot_move()
    {
        Bot* const bot = dynamic_cast<Bot*>(turn_player());
        return bot->get_move();
    }

    Player::Move Game::process_move(Player::Move move)
    {
        typedef Player::Move Move;

        const Move select_move{ process_select_die(move) };
        move = (select_move == Move::None ? move : select_move);

        switch (move)
        {
        case Move::Roll:
            return process_roll();

        case Move::Pass:
            return process_pass();

        default:
            return move;
        }
    }

    Player::Move Game::process_select_die(const Player::Move move)
    {
        return turn_player()->try_select(move);
    }

    Player::Move Game::process_roll()
    {
        return turn_player()->try_roll();
    }

    Player::Move Game::process_pass()
    {
        return turn_player()->try_pass();
    }

    bool Game::try_end_turn()
    {
        if (m_end_turn_pending)
        {
            m_end_turn_pending = false;
            turn_player()->end_turn(point_goal);
            m_start_turn_pending = true;
            return true;
        }

        return false;
    }

    bool Game::check_for_victory()
    {
        m_victory_achieved = turn_player()->get_total_points() >= point_goal;
        return m_victory_achieved;
    }

    bool Game::try_start_turn()
    {
        if (m_start_turn_pending)
        {
            m_start_turn_pending = false;
            switch_turn_player();
            turn_player()->start_turn();
            return true;
        }

        return false;
    }

    void Game::switch_turn_player()
    {
        if (m_turn_player == PlayerNumber::Player1)
        {
            m_turn_player = PlayerNumber::Player2;
        }
        else
        {
            m_turn_player = PlayerNumber::Player1;
        }
    }
}
