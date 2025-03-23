# codingame_chessbot

CodinGame bot programing chess : https://www.codingame.com/ide/puzzle/chess

## Get started

### Setup
`make setup`

### Run all tests to assure everything is ok
`make test`

### Compile actual bot
`make run`

### Compile a specific bot and copy the resulting executable into codingame chess engine folder, for its GameRunner/GameManager classes
`make runcg`


### Python tools usage

Bot names, as python script argument, could be :

#### Versus
`python3 python/versus.py {botName} {botName}`

#### ELO rating algorithm

Run a bunch of games against Stockfish instances with different ELO.
From this dataset, we then fit linear regression algorithms to predict the given bot ELO. (i.e. With stockfish ELO would have a 50% win rate against the given bot)

`python3 python/versus.py {botName} {botName}`

Displays a graph X: Stockfish ELO - Y: Winrate
    - Try n games different ELO
    - Linear regression degrees 1, 2, 3


## Description

I created my own chess engine compatible with both Standard and Chess960 rules. It can take a FEN in parameter and respond available and legal UCI moves. My bot executable can be used with CodinGame protocol through stdout.

The idea is to implement several algorithm and heuristic and benchmark them using python scripts and Stockfish.
The actual best stable bot is BbMmPv (bitboard_minmaxiterdeep[50]_piecevalues). With ELO rating of ????
The actual bot in development is BbMmPv-2 (bitboard_minmaxiterdeep[50]_piecevalues). With ELO rating of ????

External libraries are used to test & debug my own chess engine (times, valids moves from a position)


## Versionning

An unique bot is defined by 4 properties:

    * Chess engine
    * Algorithm
    * Heuristic
    * Thinking time in ms

These properties combinations are represented in 3 different ways:

    * A short name for executable name (PascalCase) : {chess_engine}{algorithm}{heuristic}{thinking_time}
    * A long name for logging name     (snake_case) : {chess_engine}_{algorithm}[{thinking_time}]_{heuristic}

Examples:

    * BMm2Pv / board_minimax[2]_piecevalues (No thinking time yet, just depth level)
    * BbMmPv / bitboard_minimax[50]_piecevalues
    * BbMmttPv / bitboard_minimaxtranstable[50]_piecevalues

Git tag should be created when bot is stable and a next version begin to be developed.
All bots should be kept locally as executable to benchmarch them. (History files keeping track of all games between versions ? Saved in git)
Bot versions must be created on their tag commit. For example, an older version shouldn't be impacted by an engine fix (so benchmarks can conclude the fix has really improved the new bot version...)
All versions should have a ELO rating.

Before creating a new version :
- Run all tests
- Run format
- Make sure it wins against the previous version
- Change Agent name
- Change Makefile exec name

## Bot versions deployed in CodinGame

BOSSES elo :

- Wood 2 ligue - BOSS-1 - 28,13

### BbMmPv-5

Submit date: 23.03.2025 22H12
CG elo: 27,18
Ligue: Wood 2
Rank: 1/315

### BbMmPv-6


## Project explanations

### Branches

- main  ->  Bot deployed in CodinGame
- tags  ->  Stable bot versions (Fully functionnal with CodinGame engine)
- dev   ->  Last stable bot

### Entity heritage

!!! NOT UP TO DATE !!!
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
    * MinMaxAlphaBetaOldAgent
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
                0 1 2 3 4 5 6 7
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

#### MinMax algorithm

    - MinMax: Alternatively choose the min and the max value returned by the heuristic function
    - Alpha-Beta pruning: Prune branches whenever the current selected child won't be better than its oncle.
    - Iterative Deepening: Successively run MinMax with an increasing depth. Update the tree on each leaf node. And stop the search if the time constraint is reached. Require a transposition table to be efficient
    - Transposition table: Hashing the position we can create an unique index to lookup a table (Collisions may appear depending on the hash function). On each new level, we can run an ordering process thanks to values stored in the table, in order to prune more branches with Alpha-Beta. This is how the iterative deepening search is faster than the original MinMax with Alpha-Beta.
    - Hash function -  :

### Python tools

* What do I want from python :
    - Benchmark heuristic (See next sections)
        * Create a protocol and use stockfish for a MSE for example. (hard to normalize stockfish output ?)
        * Compare speed and results of all heuristic (First heuristic choice won't be much the same ?)
    - Evaluate a bot ELO (with stockfish)

Info about Stockfish :
    - Under 10ms, ELO 1300 could be rated lower than 600
    - Don't use Stockfish under 800 ELO ..

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

## Roadmap

- Next steps :

    * There is a bug with castle move generation.
        A lot of failure in CG come from castling on an empty square, ally piece etc.

    * Implement iterative deepening in MinMaxAlphaBetaOldAgent -> BbMmabPv
        - Think about 2 methods: for min node and max node

    * Pour quoi BbMmPv-rc a un nombre de nodes calculé qui décroit à chaque tour ? BbMmPv était vraiment constant !

    * Ajouter une protection de coups dans BotPlayer ? Si aucune piece n'est à la soure du coup, ne pas le prendr een compte ... Ou plutot le faire la génération de coups dans le Board ?
    * Fix issue about non legal moves played !
       * k5rn/7p/1p4p1/2p1pp2/2P1P3/8/3N1PPP/2BBQKRN -> d8d2
       * k6r/5b2/1P6/1n2p2p/7b/8/1K6/5q2 -> d8d2
       * nN4kn/8/8/5p2/2p2P2/8/8/4q1BK -> f1e1
       * 1nk3Q1/6b1/8/1p2p3/2p1b3/8/3KP2P/1N3rN1 -> d8g8
       * Looks like the last move is played. Could happen if the board hadn't been updated before generating moves. Possibly due to a wrong last move
    * FIx issue about end game detection where the game should continue !
      * 8/P1p1k1pp/R2p4/2b4P/2B4K/8/5rq1/8 b - - 17 44 -> Detected as DRAW

    * Board: Implement get_castling_rights()

    * Optimize BitBoard :
        * Switch some function pointer at engine start depending on the rule ?
            - It will speed up the engine
            - We won't use boards with different rules in the same executable
        * Time optimization :
            - Create TimedBoard, which inrehit from AbstractBoard, wrapping an AbstractBoard received in constructor parameters.
            - Create a function/main to evaliuate board performances (Will mainly be usefull to optimize BitBoard performances)
                - Simulate N games and create an average time for all methods ?
            - Create BitBoardMotherFucker, inherit from AbtractBoard. Optimizing BitBoard with new dark technics

    * In python script versus, write game results in a history file reserved for these opponents
    * Find a way to print chess board in python.
        Create with Mistral a game UI with button :
           - Bot name selectors (With a human option)
           - New game
           - Previous move
           - Next move
           - Add in CG protocol:
                Available UCI move with its score
           - Show available moves
           - Show move scores
           - Show last move

    * Create MinMaxAlphaBetaTransTableAgent

    * Transform game state from static define to enum
    * Be carefull about evaluation > max_value in heuristic. It shouldn't be possible
    * BotPlayer : from vector to *vector

    * You can offer a draw to your opponent by adding = right after the move, without a space.
    * You can accept a draw offer by outputting draw instead of a move. Note that this is only legal if a draw offer was made the previous turn! Make sure to check the input first.

    * Project architecture :
        * Regroup all .test. files into a tests folder
        * Remove AbstractPLayer
        * Move ChessEngine.hpp in ./srcs

    * Add png in README.md
    * Setup git hooks:
        On commit:
            make test
        On push:
            make format
    * Use valgrind to remove invalid reads


    * Modify Board::Board(): Randomize the board generation

    * Try using smart pointers
    * CG game engine returns illegal castling move ? Report the bug ?


## Bot comments

### BitBoard.MinMaxAgent[50ms].PiecesHeuristicBb

- Timing comparaison with Board.MinMaxAgent[50ms].PiecesHeuristic
    - heuristic->evaluate():    From 2066 ms to 254 ms  -> 8x faster
    - Board():                  From 80 ms to 1500 ms  -> 18x slower .. (1780 & 1165)
    - get_available_moves():    From 2350 ms to 268 ms ->  9x faster
    - apply_move():             From 32 ms to 100 ms   ->  3x slower
    - get_check_state():        From 18 ms to 18 ms    ->     Same
    - get_game_state():         From 1750 ms to 30 ms  -> 60x faster
    - 4 mains functions :       From 4700 ms to 550 ms ->  9x faster !!!!
    - 4 mains functions + Board:From 5000 ms to 2500 ms->  2x faster ...

- Explored nodes per turns
    - Quantity of explored nodes per turn is constant ~19 npms (nodes per milliseconds)
    - Where as the BMm50Pv begin at 40 npms and slow down to apprimately 10 npms at turn 30
    - Until turn 12, BbMm50Pv compute less nodes

## Tests

sf500 100ms  vs mm3         -> 97% win / 42 games

sf1000 100ms vs sf800 None  -> 60% win
sf1000 100ms vs sf1000 10ms -> 60.0% win / 50 games
sf1000 100ms vs sf1000 50ms -> 46.6% win / 50 games
sf1000 100ms vs sf1000 None -> 47.3% win / 74 games
sf1000 50ms  vs sf1000 None -> 48.3% win / 208 games
sf1500 50ms  vs sf1500 None -> 107W 26D 127L / 260 games

In conclusion, time per turn associated to Stockfish doesn't matter above 50ms

BbMm50Pv    vs  BMm50Pv -> 66% win / 200 games

## Externals C++ libraries

### Time
https://stackoverflow.com/questions/21856025/getting-an-accurate-execution-time-in-c-micro-seconds

## Notes (12.01.23)

![codingame_chess_notes](https://user-images.githubusercontent.com/39808296/212033429-26b9978a-d90f-4c99-985f-95419892d8c8.jpg)
