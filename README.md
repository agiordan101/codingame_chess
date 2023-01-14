# codingame_chessbot

CodinGame bot programing chess : https://www.codingame.com/ide/puzzle/chess

## Description

Project created in local, thanks to CodinGame SDK environment and CodinGame chess-engine to simulate exact same executions.

External libraries are used to test & debug my own chess engine (times, valids moves from a position)

## Get started

### Compile and copy the resulting executable into codingame chess engine folder, for its GameRunner/GameManager classes
`g++ src/main.cpp -o mychessbot.out && cp mychessbot.out ../codingame-chess/.`

### Run python test script for main.cpp parsing
`python3 test/parsing_unit_tests.py`

## Externals C++ libraries

### THC the library is a C++ implementation of the rules of standard chess. (Import posiions using FEN / Export possible moves from a position / GUI?)
https://github.com/billforsternz/thc-chess-library

### Times check
https://stackoverflow.com/questions/21856025/getting-an-accurate-execution-time-in-c-micro-seconds

## Notes (12.01.23)

![codingame_chess_notes](https://user-images.githubusercontent.com/39808296/212033429-26b9978a-d90f-4c99-985f-95419892d8c8.jpg)
