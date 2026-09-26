// Self-contained tests: no framework needed, run with `ctest` or directly.
#include <iostream>

#include "tictactoe.hpp"

using namespace ttt;

namespace {

int failures = 0;

void check(bool condition, const char* description) {
    std::cout << (condition ? "  PASS  " : "  FAIL  ") << description << '\n';
    if (!condition) ++failures;
}

Board boardFrom(const char* layout) {
    // layout is 9 chars, e.g. "XX.O....."; '.' means empty.
    Board board;
    for (int i = 0; i < Board::kSize; ++i) {
        if (layout[i] == 'X') board.place(i, Cell::X);
        if (layout[i] == 'O') board.place(i, Cell::O);
    }
    return board;
}

// Tries every possible line of play by the opponent. Returns false if the AI can ever lose.
bool neverLoses(Board board, Cell turn, AI& ai) {
    if (const auto won = board.winner()) return *won == ai.mark();
    if (board.isFull()) return true;
    if (turn == ai.mark()) {
        board.place(ai.chooseMove(board), turn);
        return neverLoses(board, opponent(turn), ai);
    }
    for (int move : board.availableMoves()) {
        Board next = board;
        next.place(move, turn);
        if (!neverLoses(next, opponent(turn), ai)) return false;
    }
    return true;
}

}  // namespace

int main() {
    std::cout << "Board rules\n";
    check(boardFrom("XXX......").winner() == Cell::X, "detects a row win");
    check(boardFrom("O..O..O..").winner() == Cell::O, "detects a column win");
    check(boardFrom("X...X...X").winner() == Cell::X, "detects a diagonal win");
    check(!boardFrom("XOXXOOOXX").winner() && boardFrom("XOXXOOOXX").isFull(), "detects a draw");
    Board board;
    check(board.place(4, Cell::X) && !board.place(4, Cell::O), "rejects moves on taken squares");
    check(!board.place(9, Cell::O) && !board.place(-1, Cell::O), "rejects out-of-range squares");

    std::cout << "\nHard AI tactics\n";
    AI hardO(Cell::O, Difficulty::Hard, 7);
    check(hardO.chooseMove(boardFrom("XX.O.....")) == 2, "blocks an immediate threat");
    check(hardO.chooseMove(boardFrom("OO.XX....")) == 2, "takes an immediate win over blocking");
    check(hardO.chooseMove(boardFrom("X...O...X")) % 2 == 1, "answers the opposite-corner trap with an edge");

    std::cout << "\nHard AI is unbeatable\n";
    AI asO(Cell::O, Difficulty::Hard, 1);
    AI asX(Cell::X, Difficulty::Hard, 2);
    check(neverLoses(Board{}, Cell::X, asO), "never loses as O against every possible opponent");
    check(neverLoses(Board{}, Cell::X, asX), "never loses as X against every possible opponent");

    int draws = 0;
    for (unsigned seed = 0; seed < 20; ++seed) {
        AI x(Cell::X, Difficulty::Hard, seed);
        AI o(Cell::O, Difficulty::Hard, seed + 100);
        Board game;
        Cell turn = Cell::X;
        while (!game.isOver()) {
            game.place((turn == Cell::X ? x : o).chooseMove(game), turn);
            turn = opponent(turn);
        }
        if (!game.winner()) ++draws;
    }
    check(draws == 20, "perfect play against itself always ends in a draw");

    std::cout << "\nEasy AI\n";
    AI easy(Cell::O, Difficulty::Easy, 3);
    bool legal = true;
    for (int i = 0; i < 200; ++i) {
        Board b = boardFrom("XO.X.O...");
        const int move = easy.chooseMove(b);
        legal = legal && b.isEmpty(move);
    }
    check(legal, "only ever picks empty squares");

    std::cout << '\n' << (failures == 0 ? "All tests passed." : "Some tests FAILED.") << '\n';
    return failures == 0 ? 0 : 1;
}
