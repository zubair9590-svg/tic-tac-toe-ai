<div align="center">

# 🎮 Tic-Tac-Toe AI

**Terminal Tic-Tac-Toe in modern C++ with an unbeatable minimax AI.**

![C++17](https://img.shields.io/badge/C%2B%2B17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![License: MIT](https://img.shields.io/badge/License-MIT-22c55e?style=for-the-badge)
<br />
[![CI](https://github.com/zubair9590-svg/tic-tac-toe-ai/actions/workflows/ci.yml/badge.svg)](https://github.com/zubair9590-svg/tic-tac-toe-ai/actions/workflows/ci.yml)

</div>

## ✨ Features

- **Unbeatable Hard mode** using the **minimax** algorithm with **alpha-beta pruning**
- **Three difficulty levels**: Easy (random), Medium (plays perfectly 60% of the time) and Hard
- **Two-player mode** for playing with a friend on the same keyboard
- **Choose who goes first**, with a running scoreboard across games
- **Robust input handling**: invalid or taken squares are rejected with a friendly message
- **Tested and cross-platform**: CI builds and runs the tests on Linux, macOS and Windows

## 📺 Demo

```text
=================================
   TIC-TAC-TOE  vs  minimax AI
=================================

1) Player vs computer
2) Player vs player
Choose a mode: 1

1) Easy   2) Medium   3) Hard (unbeatable)
Choose a difficulty: 3
Do you want to go first? (y/n): y

You are X. X moves first.
...
Computer (O) plays 8.

    X | X | 3
   ---+---+---
    4 | X | 6
   ---+---+---
    O | O | O

The computer wins this one.
Score: X 0 | O 1 | draws 0
```

## 🧠 How the AI works

Tic-tac-toe is small enough to search the entire game tree. **Minimax** explores every possible sequence of moves, assuming both players play perfectly:

- a win scores `10 - depth` (so faster wins are preferred)
- a loss scores `depth - 10` (so losses are delayed as long as possible)
- a draw scores `0`

**Alpha-beta pruning** skips branches that cannot change the final decision, which makes the search much faster without changing the result. When several moves are equally good, the AI picks one at random, so games don't all look the same.

The test suite proves the AI is unbeatable: it plays the Hard AI against *every possible sequence of opponent moves*, both as X and as O, and checks that it never loses.

## 🚀 Build and run

Requires a C++17 compiler and CMake 3.16+.

```bash
git clone https://github.com/zubair9590-svg/tic-tac-toe-ai.git
cd tic-tac-toe-ai
cmake -S . -B build
cmake --build build
./build/tictactoe          # on Windows: build\Debug\tictactoe.exe
ctest --test-dir build     # run the tests
```

Or compile it directly with g++:

```bash
g++ -std=c++17 -O2 -Iinclude src/main.cpp -o tictactoe
```

## 📁 Project structure

```
tic-tac-toe-ai/
├── include/tictactoe.hpp   # Board and AI (header-only game logic)
├── src/main.cpp            # interactive terminal game
├── tests/test_ai.cpp       # rules, tactics and "never loses" tests
└── CMakeLists.txt
```

## 📄 License

[MIT](LICENSE) © Zubair
