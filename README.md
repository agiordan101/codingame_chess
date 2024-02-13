# codingame_chessbot

CodinGame bot programing chess : https://www.codingame.com/ide/puzzle/chess

## Get started

### Compile & Run project main
`make && ./bins/chessproject`

### Compile & Run unittests
`make test && ./bins/unittests`

### Compile and copy the resulting executable into codingame chess engine folder, for its GameRunner/GameManager classes
`make cg`
`make cgtest`

### Run python tools
`make cg && python3 pythonTools/benchmarks.py`


## Description

Project created in local, thanks to CodinGame SDK environment and CodinGame chess-engine to simulate exact same executions.

External libraries are used to test & debug my own chess engine (times, valids moves from a position)

## Roadmap

- Next steps :

    * Algo comp :
        - Try more ELO
        - Try stockfish as p1
        - REmove bad data ?
        - Mean isn't relevant while 2 solutions are found !

    * Simplify mains
    * Rework Makefile to integrate python things
        - + a make rule for each algorithms
    * Duplicate CGGameRunner to create one that test everything ?
    * Try to evaluate the current bot with Stockfish
        - https://en.wikipedia.org/wiki/Elo_rating_system
        - Best bot history, elo etc..
        - Implement the immaginated algorithm

    * BotPlayer : from vector to *vector

    * Board optimization :
        - Do not optimize Board methods ! Don't break anything
        - Create AbstractBoard, and change almost all Board references to AbstractBoard (Unit test too)
        - Probably need to create more inline getter/Setter
        - Create TimedBoard, inrehit from AbstractBoard, and pass an AbstractBoard in constructor parameters
        - Create BitBoard, inherit from AbtractBoard
        - Create a function/main to metric boards (Will mainly be usefull to optimize BitBoard performances)
    * Create TimedAgent or TimedMinMax ? -> Probably not usefull as only the board performances make a real difference

    * Modify Board::Board(): Randomize the board generation

    * Use valgrind to remove invalid reads
    * Try using smart pointers
    * CG game engine returns illegal castling move ? Report the bug ?


- Refactor & Improvments :

    * Refactor CGGameEngine
    * Create 10 private vars : Opponent pieces | turn_pieces
        - Assing them at each new turn
        - Simplify find_move()
            * Factorize opp_case_func and EMPTY_cell tests
            * Factorize edge tests
    * Board::Board() parsing protection from invalid FEN
        - REgex to valid chess FEN ?

- Optimizations :

    * Use copilot to optimize 
    * Opti castle loops
    * not legal moves - on apply 
    * Board :
        - _filter_non_legal_moves():
            - Do not find the king through iterating over the board for each moves.
                Should save the king position.
        - find_move():
            - Create move in order of piece values
         

## Project explanations

### Branches

- main  ->  Stable best version used in CodinGame
- dev   ->  Stable version in devlopment

### Entity heritage

- GameEngine
- CGGameEngine

- AbtractPlayer
    * BotPlayer
    * HumanPlayer

- AbtractBoard
    * Board
    * BitBoard
    * TimedBoard

- AbstractAgent
    * RandomAgent
    * HeuristicAgent
    * MinMaxAgent
    * MCTSAgent
    * TimedAgent ?

- AbstractHeuristic
    * PiecesHeuristic
    * PositionalPiecesHeuristic
    * BasicHeuristic
    ...
    * NNHeuristic


### Entity explanations

#### Board

##### Castling

In my implementation, castling moves are always represented by a king moving to its own rook, as Chess960 rules.
As the Board support both Standard chess rules and Chess960 rules, the UCI representation varies :
- Chess960 rules -> King moves to the rook
- Standard rules -> King moves by 2

Despite the rules, the final position after castling is always the same:
                a b c d e f g h
- Left castle:      K R
- Right castle:           R K

##### En passant

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

#### RandomAgent
#### HeuristicsAgent

### Python tools

* What do I want from python ? :
    - Integration test
        * Create a random position, provide the FEN and a list of moves.
        * Create a new type of standard input/output, where the C++ bot send all its available move. Python could asserts their validity
    - Benchmark exe/bots - plots
    - Benchmark heuristic (See next sections)
        * Create a protocol and use stockfish for a MSE for example. (hard to normalize stockfish output ?)
        * Compare speed and results of all heuristic (First heuristic choice won't be much the same ?)
    - Evaluate a bot ELO (with stockfish)
    - Create a dataset PO
    - Python script that merge all specified files into 1 (For codingame bot programming)

#### ELO rating algorithm using Stockfish

Just a graph X: Stockfish ELO - Y: Winrate
    - Try n games different ELO
    - Linear regression degres 2

Info about Stockfish :
    - Time constraints alter the ELO.
    - Under 10ms, ELO 1300 could be rater lower than 600

#### Heuristics benchmarks

Heuristic values should be between -1 and 1. Will be easier for deep learning, tree searchs and comparaison between heuristics :
    -1 : Black wins
    0 : Balanced game
    1 : White wins

Python script that creates a dataset : FEN / policy / best UCI move
Do a MSE between heuristic policies and Stockfish results ?
    -> Each heuristics could have their performance score
    -> Heuristics graph - x:Mean time y:MSE


### Unit testing

Not up to date !
All unit tests file must have the class name their are testing, with ".test.cpp" as suffix.

Inside the file :

- A main function (Named as "mainTest{className}"): that call all unit test launchers.
- One region per class method, with multiple unit test functions and their associated launcher.
- Unit test functions (Named as "{methodName}_{InputExplanations}_{ExpectedBehavior}"): Take testing data and expected results in parameter. Each function should test a specific behavior of the class method, compare the result and display explanations if a difference is found.
- Unit test launchers (Named as "{methodName}_testLauncher"): Call their corresponding unit test method, at least one time, with data directly in parameter.

## Externals C++ libraries

### Time
https://stackoverflow.com/questions/21856025/getting-an-accurate-execution-time-in-c-micro-seconds

## Notes (12.01.23)

![codingame_chess_notes](https://user-images.githubusercontent.com/39808296/212033429-26b9978a-d90f-4c99-985f-95419892d8c8.jpg)
