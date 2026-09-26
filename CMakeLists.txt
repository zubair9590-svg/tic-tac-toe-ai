cmake_minimum_required(VERSION 3.16)
project(tic_tac_toe_ai VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
  add_compile_options(/W4 /utf-8)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Header-only game logic shared by the game and the tests.
add_library(ttt_core INTERFACE)
target_include_directories(ttt_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(tictactoe src/main.cpp)
target_link_libraries(tictactoe PRIVATE ttt_core)

enable_testing()
add_executable(ttt_tests tests/test_ai.cpp)
target_link_libraries(ttt_tests PRIVATE ttt_core)
add_test(NAME ai_tests COMMAND ttt_tests)
