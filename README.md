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

- Next steps :

    * Create RandomAgent
    * Create Bot
    * Create HeuristicAgent
    * Implement Board::Board(): Randomize the board generation
    * Run games indefinitly with the codingame engine

- Refactor & Improvments :

    * Create 10 private vars : Opponent pieces | turn_pieces
        - Assing them at each new turn
        - Simplify find_move()
            * Factorize opp_case_func and EMPTY_cell tests
            * Factorize edge tests
    * Board::Board() parsing protection from invalid FEN

- Optimizations :

    * Detect end games (checkmate) at the start of the turn. (Not having to resolve all moves)

## Project explanations

### Branches

- main  ->  Stable best version used in CodinGame
- dev   ->  Stable version in devlopment

### Entity heritage

- AbstractAgent
    * RandomAgent
    * HeuristicAgent
    * MinMaxAgent
    * MCTSAgent
    * TimedAlgorithm

- AbtractBoard
    * Board
    * BitBoard
    * TimedBoard

- AbtractPlayer
    * BotPlayer
    * HumanPlayer

- GameEngine (One big engine doing benchmarks too ?)

### Unit testing

All unit tests file must have the class name their are testing, with ".test.cpp" as suffix.

Inside the file :

- A main function (Named as "mainTest{className}"): that call all unit test launchers.
- One region per class method, with multiple unit test functions and their associated launcher.
- Unit test functions (Named as "{methodName}_{InputExplanations}_{ExpectedBehavior}"): Take testing data and expected results in parameter. Each function should test a specific behavior of the class method, compare the result and display explanations if a difference is found.
- Unit test launchers (Named as "{methodName}_testLauncher"): Call their corresponding unit test method, at least one time, with data directly in parameter.

### Board

#### Castling

In my implementation, castling moves are always represented by a king moving to its own rook, as Chess960 rules.
As the Board support both Standard chess rules and Chess960 rules, the UCI representation varies :
- Chess960 rules -> King moves to the rook
- Standard rules -> King moves by 2

Despite the rules, the final position after castling is always the same:
                a b c d e f g h
- Left castle:      K R
- Right castle:           R K

#### En passant

* Turn n:
    - find_move()
        Generate a Move where a pawn advances by two 
    - apply_move()
        Pawn advances by two
        Pawn advances by two ? ->
            available=false
            x: 5
            y: 3
    - end_turn():
        available=false ? ->
            available=true

* Turn n+1:
    - find_move()
        available=true ? ->
            Generate a Move with en_passant=true
    - apply_move()
        Pawn advances in diagonale
        available=true ? ->
            Pawn takes
    - end_turn():
        available=true ? ->
            available=false
            x: -1
            y: -1

## Externals C++ libraries

### THC the library is a C++ implementation of the rules of standard chess. (Import posiions using FEN / Export possible moves from a position / GUI?)
https://github.com/billforsternz/thc-chess-library

### Time
https://stackoverflow.com/questions/21856025/getting-an-accurate-execution-time-in-c-micro-seconds

## Notes (12.01.23)

![codingame_chess_notes](https://user-images.githubusercontent.com/39808296/212033429-26b9978a-d90f-4c99-985f-95419892d8c8.jpg)
