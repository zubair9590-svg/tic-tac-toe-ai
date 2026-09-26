#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>

#include "tictactoe.hpp"

using namespace ttt;

namespace {

struct Score {
    int x = 0;
    int o = 0;
    int draws = 0;
};

std::string trim(const std::string& text) {
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    const auto last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt << std::flush;
    std::string line;
    if (!std::getline(std::cin, line)) {
        std::cout << "\nThanks for playing!\n";
        std::exit(0);
    }
    return trim(line);
}

int readNumber(const std::string& prompt, int min, int max) {
    while (true) {
        const std::string line = readLine(prompt);
        if (line.size() == 1 && std::isdigit(static_cast<unsigned char>(line[0]))) {
            const int value = line[0] - '0';
            if (value >= min && value <= max) return value;
        }
        std::cout << "  Please enter a number from " << min << " to " << max << ".\n";
    }
}

bool askYesNo(const std::string& prompt) {
    while (true) {
        const std::string answer = readLine(prompt);
        if (answer == "y" || answer == "Y" || answer == "yes") return true;
        if (answer == "n" || answer == "N" || answer == "no") return false;
        std::cout << "  Please answer y or n.\n";
    }
}

int askHumanMove(const Board& board, const std::string& who) {
    while (true) {
        const int square = readNumber(who + ", pick a square (1-9): ", 1, 9) - 1;
        if (board.isEmpty(square)) return square;
        std::cout << "  That square is taken, try another one.\n";
    }
}

// Plays one game. `ai` is null in two-player mode.
void playRound(AI* ai, Score& score) {
    Board board;
    Cell turn = Cell::X;  // X always moves first
    std::cout << '\n' << board.render() << '\n';

    while (!board.isOver()) {
        if (ai && turn == ai->mark()) {
            const int move = ai->chooseMove(board);
            board.place(move, turn);
            std::cout << "Computer (" << static_cast<char>(turn) << ") plays " << move + 1 << ".\n";
        } else {
            const std::string who = ai ? "You (" + std::string(1, static_cast<char>(turn)) + ")"
                                       : "Player " + std::string(1, static_cast<char>(turn));
            board.place(askHumanMove(board, who), turn);
        }
        std::cout << '\n' << board.render() << '\n';
        turn = opponent(turn);
    }

    if (const auto won = board.winner()) {
        (*won == Cell::X ? score.x : score.o) += 1;
        if (!ai) {
            std::cout << "Player " << static_cast<char>(*won) << " wins!\n";
        } else if (*won == ai->mark()) {
            std::cout << "The computer wins this one.\n";
        } else {
            std::cout << "You win! Nicely played.\n";
        }
    } else {
        score.draws += 1;
        std::cout << "It's a draw.\n";
    }
    std::cout << "Score: X " << score.x << " | O " << score.o << " | draws " << score.draws << "\n\n";
}

}  // namespace

int main() {
    std::cout << "=================================\n"
              << "   TIC-TAC-TOE  vs  minimax AI\n"
              << "=================================\n\n"
              << "1) Player vs computer\n"
              << "2) Player vs player\n";
    const bool vsComputer = readNumber("Choose a mode: ", 1, 2) == 1;

    AI* ai = nullptr;
    AI computer(Cell::O);
    if (vsComputer) {
        std::cout << "\n1) Easy   2) Medium   3) Hard (unbeatable)\n";
        const int level = readNumber("Choose a difficulty: ", 1, 3);
        const bool humanFirst = askYesNo("Do you want to go first? (y/n): ");
        const Difficulty difficulty = level == 1 ? Difficulty::Easy : level == 2 ? Difficulty::Medium : Difficulty::Hard;
        computer = AI(humanFirst ? Cell::O : Cell::X, difficulty);
        ai = &computer;
        std::cout << "\nYou are " << static_cast<char>(opponent(computer.mark())) << ". X moves first.\n";
    }

    Score score;
    do {
        playRound(ai, score);
    } while (askYesNo("Play again? (y/n): "));

    std::cout << "Thanks for playing!\n";
    return 0;
}
