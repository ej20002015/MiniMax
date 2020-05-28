project("MiniMax")

set(CMAKE_BUILD_TYPE Release)

cmake_minimum_required(VERSION 2.8)

add_executable(MiniMax
    Main.cpp
    MiniMaxWithPruning.cpp
    Player.cpp
    PlayTicTacToe.cpp
    TicTacToe.cpp
)