# codingame_chessbot

CodinGame bot programing chess : https://www.codingame.com/ide/puzzle/chess


## Description

I created my own chess engine compatible with both Standard and Chess960 rules. It can take a FEN in parameter and respond available and legal UCI moves. My bot executable can be used with CodinGame protocol through stdout.

The idea is to implement several algorithm and heuristic and benchmark them using python scripts, Stockfish and psyleague.

External libraries are used to test & debug my own chess engine (times, valids moves from a position)


## Get started

### Setup
`make setup`

### Run all tests to assure everything is ok
`make test`

### Copy actual bot source files in one .cpp & Compile it 
`make run`

## Internal python tools usages

Bot names, as python script argument, could be :

### Versus
`python3 python/versus.py {botName} {botName}`

### ELO rating algorithm

Run a bunch of games against Stockfish instances with different ELO.
From this dataset, we then fit linear regression algorithms to predict the given bot ELO. (i.e. With stockfish ELO would have a 50% win rate against the given bot)

`python3 python/versus.py {botName} {botName}`

Displays a graph X: Stockfish ELO - Y: Winrate
    - Try n games different ELO
    - Linear regression degrees 1, 2, 3


## External tools usages

### BrutalTester

https://github.com/dreignier/cg-brutaltester

Run a bunch of games between two bots, using multiple threads.
Use this command line at repository root:

`java -jar target/cg-brutaltester.jar -r "java -jar ../codingame-chess/target/chess-1.0-SNAPSHOT.jar" -p1 "../codingame-chess/mychessbot" -p2 "../codingame-chess/mychessbot" -t 7 -n 10`

### Psyleague

https://github.com/FakePsyho/psyleague

All the following command must be executed at psyleague repository root.

Config file is located at `./psyleague.cfg`

Start psyleague server :

`psyleague run`

Add bots with :

`psyleague bot add {BOT_NAME} -s {MAIN_SRC_PATH}`
`psyleague bot add BbMmabPv-3.1.6 -s ../codingame_chess/mains/maincg`

See current results with :

`psyleague show`

Reset league with :

`rm psyleague.db && rm psyleague.games`

### Callgrind

Callgrind is a profiling tool that records the call history among functions in a program's run as a call-graph.
For graphical visualization of the data, we can use KCachegrind command. (apt install kcachegrind)

https://valgrind.org/docs/manual/cl-manual.html

Here is how we can use callgrind to profile our bot, at cg-brutaltester repository root:

`java -jar target/cg-brutaltester.jar -r "java -jar ../codingame-chess/target/chess-1.0-SNAPSHOT.jar" -p1 "valgrind --tool=callgrind ../codingame_chess/bins/BbMmabPv-3.1.6" -p2 "../codingame_chess/bins/BbMmabPv-3.1.6" -t 1 -n 1`

Then, we can visualize the generated callgrind.out file with KCachegrind:

`kcachegrind callgrind.out.12345`


## Versionning

An unique bot is defined by 4 properties:

    * Chess engine
    * Algorithm
    * Heuristic

These properties combinations are represented in 3 different ways:

    * A short name for executable name (PascalCase) : {chess_engine}{algorithm}{heuristic}{thinking_time}
    * A long name for logging name     (snake_case) : {chess_engine}_{algorithm}[{thinking_time}]_{heuristic}

Each of them have a version number, in respect of the order.

Examples:

    * BbMmPv / bitboard_minimax[50]_piecevalues
    * BbMmttPv-2.4.5  / bitboard_minimaxtranstable[50]_piecevalues

Git tag should be created when bot is stable and a next version begin to be developed.
All bots should be kept locally as executables and source code, to benchmarch them.
Bot versions must be created on their tag commit. For example, an older version shouldn't be impacted by an engine fix (so benchmarks can conclude the fix has really improved the new bot version...)
All versions should have a ELO rating.

Before creating a new version :
- make format
- make test
- Make sure it wins against the previous version
- Change version number in Agent
- Change version number in Makefile
- make run

## Bot versions deployed in CodinGame

### BbMmabPv-3.1.6 (Best version)

* Submit date: 29.04.2025 21H32
* Overall ranking : 32/384
* Ligue: Wood 1 (Best ligue)
    * Rank: 32/66
    * CG elo: 20,55
    * Top 1 elo: 44,90

### BbMmabPv-3.1.3

* Submit date: 17.04.2025 14H47
* Overall ranking : 31/384
* Ligue: Wood 1 (Best ligue)
    * Rank: 31/66
    * CG elo: 20,77
    * Top 1 elo: 44,90

### BbMmabPv-1 (-> BbMmabPv-3.1.2)

* Submit date: 23.03.2025 23H28
* Overall ranking : 34/374
* Ligue: Wood 1 (Best ligue)
    * Rank: 34/60
    * CG elo: 19,13
    * Top 1 elo: 44,90

### BbMmPv-6 (-> BbMmPv-3.2.2)

* Submit date: 23.03.2025 23H28
* Overall ranking : 42/374
* Ligue: Wood 1 (Best ligue)
    * Rank: 42/60
    * CG elo: 15,78
    * Top 1 elo: 44,90

### BbMmPv-5 (-> BbMmPv-2.2.2)

* Submit date: 23.03.2025 22H12
* Overall ranking : 61/374
* Ligue: Wood 2
    * Rank: 1/315
    * CG elo: 27,18
    * BOSS-1 elo: 28,13


## Project explanations

### Branches

- main  ->  Bot deployed in CodinGame
- dev   ->  Last best bot version, with clean repository
- tags  ->  Stable bot versions (Fully functionnal with CodinGame engine)

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
- Transposition table: Hashing the position we can create an unique index to lookup a table (With the depth of the calculation! it is improtant). On each new level, we can run an ordering process thanks to values stored in the table, in order to prune more branches with Alpha-Beta. This is how the iterative deepening search is faster than the original MinMax with Alpha-Beta.
- Hash function -  :

#### MCTS algorithm

* Rollout heuristic sucks in chess, due to: 
    * Huge Branching Factor
    * Draw Bias: Most random playouts between good engines or even random ones lead to draws, not wins/losses
    * Noisy Evaluation in a complex game


* Node / ms :
    * BbMctsPv-3.3.6 = 120
    * BbMctsPv-3.4.6 = 160 -> 210
    * BbMctsPv-3.5.6 = 190 -> 220


### Python tools

!!! NOT UP TO DATE !!!
* What do I want from python :
    - Benchmark heuristic (See next sections)
        * Create a protocol and use stockfish for a MSE for example. (hard to normalize stockfish output ?)
        * Compare speed and results of all heuristic (First heuristic choice won't be much the same ?)
    - Evaluate a bot ELO (with stockfish)

Info about Stockfish :
    - Under 10ms, ELO 1300 could be rated lower than 600
    - Don't use Stockfish under 800 ELO ..

#### Heuristics benchmarks

!!! NOT UP TO DATE !!!
Heuristic values should be between -1 and 1. Will be easier for deep learning, tree searchs and comparaison between heuristics :
    -1 : Black wins
    0 : Balanced game
    1 : White wins

Python script that creates a dataset : FEN / policy / best UCI move
Do a MSE between heuristic policies and Stockfish results ?
    -> Each heuristics could have their performance score
    -> Heuristics graph - x:Mean time y:MSE


### Unit testing

!!! NOT UP TO DATE !!!
All unit tests file must have the class name their are testing, with ".test.cpp" as suffix.

Inside the file :

- A main function (Named as "mainTest{className}"): that call all unit test launchers.
- One region per class method, with multiple unit test functions and their associated launcher.
- Unit test functions (Named as "{methodName}_{InputExplanations}_{ExpectedBehavior}"): Take testing data and expected results in parameter. Each function should test a specific behavior of the class method, compare the result and display explanations if a difference is found.
- Unit test launchers (Named as "{methodName}_testLauncher"): Call their corresponding unit test method, at least one time, with data directly in parameter.

## Roadmap

- Next steps :

    * Trying to optimize _find_white_pawn_moves() :

        * Incl.| Self timings
        * 5.67 | 1.83 for BbMmabPv-3.1.8

    * Is there a way to continue MCTS iterations while opponent is thinking ?

    * Improve Board performances
        * Main bottlenecks from BOARD class (callgrind statistics) :
            * Board::create_fen() :
                *  7% in BbMctsPv-3.7.6
                * 11% in BbMmabPv-3.1.6
                *  8% in BbMmabPv-3.1.8
                *  6% in BbMmabPv-3.2.8 (= BbMmabPv-4.1.8)
            * Board::_compute_game_state() :
                * 22% in BbMctsPv-3.7.6
                * 28% in BbMmabPv-3.1.6
                * 20% in BbMmabPv-3.1.8
                * 18% in BbMmabPv-3.2.8 (= BbMmabPv-4.1.8)
            * Board::_find_move() :
                * 12% in BbMctsPv-3.7.6
                * 19% in BbMmabPv-3.1.6
                * 14% in BbMmabPv-3.1.8
                * 12% in BbMmabPv-3.2.8 (= BbMmabPv-4.1.8)
            * Board::_apply_move() :
                *  8% in BbMctsPv-3.7.6
                * 13% in BbMmabPv-3.1.6
                *  9% in BbMmabPv-3.1.8
                *  7% in BbMmabPv-3.2.8 (= BbMmabPv-4.1.8)
            * Board::_find_white_pawns_moves() :
                *  5% in BbMctsPv-3.7.6
                *  8% in BbMmabPv-3.1.6
                *  6% in BbMmabPv-3.1.8
                *  5% in BbMmabPv-3.2.8 (= BbMmabPv-4.1.8)
                    * Can directly check if pawn is BITMASK_LINE_2 or BITMASK_LINE_8/1
                    * Try to get rid of while()

            * Board::_apply_function_on_all_pieces() (Seems coherent as many logic is below):
                * 14% in BbMctsPv-3.7.6
                * 22% in BbMmabPv-3.1.6
                * 17% in BbMmabPv-3.1.8

    * Improve Heuristic
        * Revert code that made Heuristic version 6 ? Do that when upgrading the heuristic
        Pos  Name            Score  Games     %      Mu  Sigma  Errors              Created
            ---  --------------  -----  -----  ----  ------  -----  ------  -------------------
            1  BbMmabPv-3.1.5  28.19    826  100%  29.055  0.288      10  2025/05/24 17:18:04
            2  BbMmabPv-3.1.6  28.05    842  100%  28.897  0.283      18  2025/05/24 17:18:04

    * Re-implemente transposition table ?
        * Board zobrist :
            * Rebase zobrist board on main
            * Create 2 Boards : Board (Bb) and ZobristBoard (Bbz)

    * Understand why brutaltester and psyleague don't reutrn the same results depending on thread numbers :
        * Seems like having multipe threads at the same time make bot crash randomly (probably by timeout ?)

        * Psyleague threads test :
            * 1 threads (102 games) :
                Pos  Name            Score  Games    %      Mu  Sigma  Errors              Created
                ---  --------------  -----  -----  ---  ------  -----  ------  -------------------
                1  BbMmabPv-3.1.5  23.62    102  20%  25.703  0.695       0  2025/05/28 15:06:04
                2  BbMmabPv-3.1.6  22.21    102  20%  24.297  0.695       8  2025/05/28 15:06:06
            * 7 threads (408 games) :
                Pos  Name            Score  Games    %      Mu  Sigma  Errors              Created
                ---  --------------  -----  -----  ---  ------  -----  ------  -------------------
                1  BbMmabPv-3.1.6  23.97    408  81%  25.039  0.356     117  2025/05/28 14:56:35
                2  BbMmabPv-3.1.5  23.89    408  81%  24.961  0.356     122  2025/05/28 14:56:35

    * Why all promotions are knight ???
    
    * Just save all FEN encounter in one turn, to anticipate how much transposition table will be helpful
    
    * MCMS: Monte Carlo Minimax Search
        🔍 Core Idea:
        MCMS combines minimax search with Monte Carlo sampling. Instead of evaluating every child node exhaustively (as in standard minimax), MCMS uses random sampling to reduce the branching factor.

        📦 How It Works:
        At each node, instead of exploring all child moves:
            * Sample a subset of actions randomly (Monte Carlo).
            * Use minimax backup rules (max at your turn, min at opponent’s).
            Optionally, simulate multiple playouts per node to stabilize the value.

    * A way to improve BbMmabPv-3.1.6 to BbMmabPv-3.2.6 :
        * Use Node as MCTS to store game state and not recompute them (apply_move, get_game_state)
        * Only expand the last depth

    * Create 2 heuristics: Why ?
        * MaterialHeuristic: Just material
        * PiecePositionHeuristic : Material + position + control
        * KEEP HEURISTIC IDEAS FOR LATER FOR GOD SAKE !

    * Try moves caching with zobrist hash as key. Save fen inside so we can detect collisions (fen is obligatory calculated for Five move repetition rule)

    * In python script versus, write game results in a history file reserved for these opponents

    * DRAW offers (Need a winrate estimation first) :
        * You can offer a draw to your opponent by adding = right after the move, without a space:
            * If estimated winrate < 0.4, offer a draw (as white)
        * You can accept a draw offer by outputting draw instead of a move. Note that this is only legal if a draw offer was made the previous turn! Make sure to check the input first.
            * If estimated winrate > 0.6, accept a draw (as white)

    * Pour quoi BbMmPv-rc a un nombre de nodes calculé qui décroit à chaque tour ? BbMmPv était vraiment constant !

    * Optimize BitBoard :
        * Re-implement Five repetition rule: Don't create FEN, save bitboard numbers instead
        * Switch some function pointer at engine start depending on the rule ?
            - It will speed up the engine
            - We won't use boards with different rules in the same executable
        * Time optimization :
            - Create TimedBoard, which inrehit from AbstractBoard, wrapping an AbstractBoard received in constructor parameters.
            - Create a function/main to evaliuate board performances (Will mainly be usefull to optimize BitBoard performances)
                - Simulate N games and create an average time for all methods ?
            - Create BitBoardMotherFucker, inherit from AbtractBoard. Optimizing BitBoard with new dark technics

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

    * Transform game state from static define to enum
    * BotPlayer : from vector to *vector

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


## Externals C++ libraries

## Notes (12.01.23)

![codingame_chess_notes](https://user-images.githubusercontent.com/39808296/212033429-26b9978a-d90f-4c99-985f-95419892d8c8.jpg)
