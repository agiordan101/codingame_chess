# codingame_chessbot

CodinGame bot programing chess : https://www.codingame.com/ide/puzzle/chess

## Get started

### Compile and copy the resulting executable into codingame chess engine folder, for its GameRunner/GameManager classes
`g++ main_codingame.cpp chessengine/* -o mychessbot.out && cp mychessbot.out ../codingame-chess/.`

### Compile & Run unittests
`g++ main.test.cpp chessengine/* -o unittests.out && ./unittests.out`

### Compile & Run project main
`g++ main.cpp chessengine/* -o chessproject.out && ./chessproject.out`

### Run python test script for main.cpp parsing
`python3 test/parsing_unit_tests.py`

## Description

Project created in local, thanks to CodinGame SDK environment and CodinGame chess-engine to simulate exact same executions.

External libraries are used to test & debug my own chess engine (times, valids moves from a position)

## Roadmap

* Global function unit tests: algebraic_to_coord()
* Global function unit tests: coord_to_algebraic()
* Move::Move(uci) unit tests
* Move::get_uci() unit tests
* Parse en passant (UCI) from FEN
* Implement Move::display_UCI() method
* Implement Move::operator==() method
* Implement Board::find_moves() methods
* Implement Board::is_end_game() method
* Board::next_turn() unit tests

* Rules to implement :
    - Stalemate, while a player isn't under check and no legal move exist. (Pretty much the same as checkmate)
    - Threefold Repetition.
    - Fifty-Move rule.
    - Draw by agreement
    - Insufficient material, This includes:
        - King vs king
        - King+knight vs king
        - King+bishop vs king
        - King+bishop vs king+bishop if both bishops are on the same square color.

## Project explanations

### Branches

- main  ->  Stable best version used in CodinGame
- dev   ->  Stable version in devlopment (base for new branches, base for comparison, etc...)

### Unit testing

All unit tests file must have the class name their are testing, with ".test.cpp" as suffix.

Inside the file :

- A main function (Named as "mainTest{className}"): that call all unit test launchers.
- One region per class method, with multiple unit test functions and their associated launcher.
- Unit test functions (Named as "{methodName}_{InputExplanations}_{ExpectedBehavior}"): Take testing data and expected results in parameter. Each function should test a specific behavior of the class method, compare the result and display explanations if a difference is found.
- Unit test launchers (Named as "{methodName}_testLauncher"): Call their corresponding unit test method, at least one time, with data directly in parameter.

## Externals C++ libraries

### THC the library is a C++ implementation of the rules of standard chess. (Import posiions using FEN / Export possible moves from a position / GUI?)
https://github.com/billforsternz/thc-chess-library

### Time
https://stackoverflow.com/questions/21856025/getting-an-accurate-execution-time-in-c-micro-seconds

## Notes (12.01.23)

![codingame_chess_notes](https://user-images.githubusercontent.com/39808296/212033429-26b9978a-d90f-4c99-985f-95419892d8c8.jpg)
