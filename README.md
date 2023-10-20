# codingame_chessbot

CodinGame bot programing chess : https://www.codingame.com/ide/puzzle/chess

## Description

Project created in local, thanks to CodinGame SDK environment and CodinGame chess-engine to simulate exact same executions.

External libraries are used to test & debug my own chess engine (times, valids moves from a position)

### Branches

- main  ->  Stable best version used in CodinGame
- dev   ->  Stable version in devlopment (base for new branches, base for comparison, etc...)

## Get started

### Compile and copy the resulting executable into codingame chess engine folder, for its GameRunner/GameManager classes
`g++ main_codingame.cpp chessengine/* -o mychessbot.out && cp mychessbot.out ../codingame-chess/.`

### Compile & Run C++ Board unittests
`g++ main.test.cpp chessengine/* -o unittest_Board.out && ./unittest_Board.out`

### Run python test script for main.cpp parsing
`python3 test/parsing_unit_tests.py`

## Externals C++ libraries

### THC the library is a C++ implementation of the rules of standard chess. (Import posiions using FEN / Export possible moves from a position / GUI?)
https://github.com/billforsternz/thc-chess-library

### Time
https://stackoverflow.com/questions/21856025/getting-an-accurate-execution-time-in-c-micro-seconds

## Notes (12.01.23)

![codingame_chess_notes](https://user-images.githubusercontent.com/39808296/212033429-26b9978a-d90f-4c99-985f-95419892d8c8.jpg)
