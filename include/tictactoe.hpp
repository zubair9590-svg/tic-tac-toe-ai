#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <limits>
#include <optional>
#include <random>
#include <string>
#include <vector>

namespace ttt {

enum class Cell : char { Empty = ' ', X = 'X', O = 'O' };

inline Cell opponent(Cell player) { return player == Cell::X ? Cell::O : Cell::X; }

// A 3x3 board. Squares are numbered 0-8, left to right and top to bottom.
class Board {
public:
    static constexpr int kSize = 9;

    Board() { cells_.fill(Cell::Empty); }

    Cell at(int index) const { return cells_[index]; }
    bool isEmpty(int index) const { return cells_[index] == Cell::Empty; }

    bool place(int index, Cell player) {
        if (index < 0 || index >= kSize || !isEmpty(index)) return false;
        cells_[index] = player;
        return true;
    }

    void clear(int index) { cells_[index] = Cell::Empty; }

    std::vector<int> availableMoves() const {
        std::vector<int> moves;
        for (int i = 0; i < kSize; ++i) {
            if (isEmpty(i)) moves.push_back(i);
        }
        return moves;
    }

    std::optional<Cell> winner() const {
        static constexpr int kLines[8][3] = {
            {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // rows
            {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // columns
            {0, 4, 8}, {2, 4, 6},             // diagonals
        };
        for (const auto& line : kLines) {
            const Cell first = cells_[line[0]];
            if (first != Cell::Empty && first == cells_[line[1]] && first == cells_[line[2]]) return first;
        }
        return std::nullopt;
    }

    bool isFull() const {
        return std::none_of(cells_.begin(), cells_.end(), [](Cell c) { return c == Cell::Empty; });
    }

    bool isOver() const { return winner().has_value() || isFull(); }

    // Renders the board; empty squares show their number (1-9) so players know what to type.
    std::string render() const {
        std::string out;
        for (int row = 0; row < 3; ++row) {
            out += "   ";
            for (int col = 0; col < 3; ++col) {
                const int i = row * 3 + col;
                out += ' ';
                out += isEmpty(i) ? static_cast<char>('1' + i) : static_cast<char>(cells_[i]);
                out += ' ';
                if (col < 2) out += '|';
            }
            out += '\n';
            if (row < 2) out += "   ---+---+---\n";
        }
        return out;
    }

private:
    std::array<Cell, kSize> cells_{};
};

enum class Difficulty { Easy, Medium, Hard };

// Computer player. Hard mode uses minimax with alpha-beta pruning and never loses.
class AI {
public:
    explicit AI(Cell me, Difficulty difficulty = Difficulty::Hard, unsigned seed = std::random_device{}())
        : me_(me), difficulty_(difficulty), rng_(seed) {}

    Cell mark() const { return me_; }

    int chooseMove(Board board) {
        if (difficulty_ == Difficulty::Easy) return randomMove(board);
        if (difficulty_ == Difficulty::Medium) {
            // Plays perfectly 60% of the time, so it's beatable but not careless.
            std::bernoulli_distribution playWell(0.6);
            return playWell(rng_) ? bestMove(board) : randomMove(board);
        }
        return bestMove(board);
    }

private:
    int randomMove(const Board& board) {
        const std::vector<int> moves = board.availableMoves();
        std::uniform_int_distribution<std::size_t> pick(0, moves.size() - 1);
        return moves[pick(rng_)];
    }

    int bestMove(Board& board) {
        int bestScore = std::numeric_limits<int>::min();
        std::vector<int> bestMoves;
        for (int move : board.availableMoves()) {
            board.place(move, me_);
            const int score = minimax(board, opponent(me_), 1, std::numeric_limits<int>::min(),
                                      std::numeric_limits<int>::max());
            board.clear(move);
            if (score > bestScore) {
                bestScore = score;
                bestMoves = {move};
            } else if (score == bestScore) {
                bestMoves.push_back(move);
            }
        }
        // Several moves are often equally good; pick one at random so games feel less repetitive.
        std::uniform_int_distribution<std::size_t> pick(0, bestMoves.size() - 1);
        return bestMoves[pick(rng_)];
    }

    // Scores a position from the AI's point of view. Faster wins and slower losses score higher.
    int minimax(Board& board, Cell turn, int depth, int alpha, int beta) {
        if (const auto won = board.winner()) return *won == me_ ? 10 - depth : depth - 10;
        if (board.isFull()) return 0;

        const bool maximizing = turn == me_;
        int best = maximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
        for (int move : board.availableMoves()) {
            board.place(move, turn);
            const int score = minimax(board, opponent(turn), depth + 1, alpha, beta);
            board.clear(move);
            if (maximizing) {
                best = std::max(best, score);
                alpha = std::max(alpha, best);
            } else {
                best = std::min(best, score);
                beta = std::min(beta, best);
            }
            if (beta <= alpha) break;  // prune: the opponent will never allow this line
        }
        return best;
    }

    Cell me_;
    Difficulty difficulty_;
    std::mt19937 rng_;
};

}  // namespace ttt
