#include "Gui.h"
#include "splashkit.h"

#define INSTRUCTION_TEXT_SIZE 60
#define OPTION_TEXT_SIZE 40
#define OPTION_GAP 80

namespace Farkle
{
    void write_option(const int number, const std::string& text)
    {
        const rectangle container{ 0, 0, static_cast<double>(screen_width()), static_cast<double>(screen_height()) };
        const double x_pos{ Gui::x_to_draw_at_centre(text_width(text, FONT, OPTION_TEXT_SIZE), container) };
        const double y_pos{ screen_height() / 2.0 + OPTION_GAP * number };
        draw_text(text, COLOR_WHITE, FONT, OPTION_TEXT_SIZE, x_pos, y_pos);
    }

    void write_instruction(const std::string& text)
    {
        const rectangle container{ 0, 0, static_cast<double>(screen_width()), static_cast<double>(screen_height()) };
        const double x_pos{ Gui::x_to_draw_at_centre(text_width(text, FONT, INSTRUCTION_TEXT_SIZE), container) };
        const double y_pos{ screen_height() / 6.0 };
        draw_text(text, COLOR_WHITE, FONT, INSTRUCTION_TEXT_SIZE, x_pos, y_pos);
    }

    enum class RpsOption
    {
        Rock,
        Paper,
        Scissors,
        None
    };

    RpsOption process_rps_input()
    {
        if (key_released(NUM_1_KEY))
        {
            return RpsOption::Rock;
        }
        if (key_released(NUM_2_KEY))
        {
            return RpsOption::Paper;
        }
        if (key_released(NUM_3_KEY))
        {
            return RpsOption::Scissors;
        }

        return RpsOption::None;
    }

    // Returns the player number of the winner of Rock, Paper, Scissors.
    // Returns 0 if it was a draw.
    int get_winner(const RpsOption player1_type, const RpsOption player2_type)
    {
        if (player1_type == player2_type)
        {
            return 0;
        }

        switch (player1_type)
        {
        case RpsOption::Rock:
            return player2_type == RpsOption::Scissors ? 1 : 2;

        case RpsOption::Paper:
            return player2_type == RpsOption::Rock ? 1 : 2;

        case RpsOption::Scissors:
            return player2_type == RpsOption::Paper ? 1 : 2;

        default:
            return 0;
        }
    }

    std::string rps_option_to_string(const RpsOption option)
    {
        switch (option)
        {
        case RpsOption::Rock:
            return "Rock";

        case RpsOption::Paper:
            return "Paper";

        case RpsOption::Scissors:
            return "Scissors";

        default:
            return "None";
        }
    }

    void display_rps_selections(const RpsOption player1_selection, const RpsOption player2_selection, const int winner)
    {
        clear_screen(COLOR_DARK_BLUE);
        delay(1000);
        write_option(-2, "Player 1 chose " + rps_option_to_string(player1_selection) + ".");
        write_option(-1, "Player 2 chose " + rps_option_to_string(player2_selection) + ".");

        if (winner)
        {
            write_option(0, "Player " + std::to_string(winner) + " will go first.");
        }
        else
        {
            write_option(0, "This was a draw. Try again.");
        }

        refresh_screen(60);
        delay(2500);
    }

    void write_rps_options(const int player_number)
    {
        clear_screen(COLOR_DARK_BLUE);
        write_instruction("Player " + std::to_string(player_number) + ", select one of the following.");
        write_option(0, "1: Rock");
        write_option(1, "2: Paper");
        write_option(2, "3: Scissors");
        refresh_screen(60);
    }

    enum class PlayerTypeOption
    {
        Human,
        EasyBot,
        HardBot,
        None
    };

    PlayerTypeOption process_menu_input()
    {
        if (key_released(NUM_1_KEY))
        {
            return PlayerTypeOption::Human;
        }
        if (key_released(NUM_2_KEY))
        {
            return PlayerTypeOption::EasyBot;
        }
        if (key_released(NUM_3_KEY))
        {
            return PlayerTypeOption::HardBot;
        }

        return PlayerTypeOption::None;
    }


    RpsOption get_rock_paper_scissors_option(const Player::Type player_type, const int player_number)
    {
        while (player_type == Player::Type::Human && !quit_requested())
        {
            process_events();
            write_rps_options(player_number);
            const RpsOption option{ process_rps_input() };

            if (option != RpsOption::None)
            {
                return option;
            }
        }

        return static_cast<RpsOption>(rnd(0, 3));
    }

    bool start_with_player2(const Player::Type player1_type, const Player::Type player2_type)
    {
        int winner{};

        // play rock, paper scissors to determine first turn player
        while (!quit_requested() && !winner)
        {
            const RpsOption player1_selection{ get_rock_paper_scissors_option(player1_type, 1) };
            const RpsOption player2_selection{ get_rock_paper_scissors_option(player2_type, 2) };
            winner = get_winner(player1_selection, player2_selection);
            display_rps_selections(player1_selection, player2_selection, winner);
        }

        return winner == 2 ? true : false;
    }

    void write_player_type_options(const std::string& player)
    {
        clear_screen(COLOR_DARK_BLUE);
        write_instruction("Select Player Type for " + player + ".");
        write_option(0, "1: Human");
        write_option(1, "2: Easy Bot");
        write_option(2, "3: Hard Bot");
        refresh_screen(60);
    }

    Player::Type get_player_type(const std::string& player)
    {
        write_player_type_options(player);

        while (!quit_requested())
        {
            process_events();
            const PlayerTypeOption option{ process_menu_input() };

            switch (option)
            {
            case PlayerTypeOption::Human:
                return Player::Type::Human;

            case PlayerTypeOption::EasyBot:
                return Player::Type::Bot;

            case PlayerTypeOption::HardBot:
                return Player::Type::HardBot;

            default:
                break;
            }
        }

        return Player::Type::Human;
    }

    Player::Move process_input()
    {
        typedef Player::Move Move;

        if (key_released(NUM_1_KEY))
        {
            return Move::SelectDie0;
        }
        if (key_released(NUM_2_KEY))
        {
            return Move::SelectDie1;
        }
        if (key_released(NUM_3_KEY))
        {
            return Move::SelectDie2;
        }
        if (key_released(NUM_4_KEY))
        {
            return Move::SelectDie3;
        }
        if (key_released(NUM_5_KEY))
        {
            return Move::SelectDie4;
        }
        if (key_released(NUM_6_KEY))
        {
            return Move::SelectDie5;
        }
        if (key_released(R_KEY))
        {
            return Move::Roll;
        }
        if (key_released(E_KEY))
        {
            return Move::Pass;
        }

        return Move::None;
    }

    void run()
    {
        const Player::Type player1_type{ get_player_type("Player 1") };
        const Player::Type player2_type{ get_player_type("Player 2") };
        const int player2_start{ start_with_player2(player1_type, player2_type) };

        Game game{ player1_type, player2_type, player2_start };
        Gui gui{ game };

        while (!quit_requested() && (gui.is_busy() || !game.victory_achieved()))
        {
            process_events();
            Player::Move move{ process_input() };

            if (!gui.is_busy() && !game.victory_achieved())
            {
                move = game.prompt(move);
            }

            gui.update(move);
        }
    }
}

int main()
{
    open_window("Farkle", 1366, 768);

    while (!quit_requested())
    {
        Farkle::run();
    }

    return 0;
}
