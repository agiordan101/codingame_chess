
TEST_EXEC = unittests
PERFT_EXEC = perft
DATASETTEST_EXEC = datasettest
CG_EXEC = mychessbot
BOT_EXEC = BbMm2Pv

flag = -g -O2 # -Wall -Wextra

SRCS_PATH = srcs
ALL_FILES = $(wildcard $(SRCS_PATH)/*/*.cpp)
TEST_FILES = $(wildcard $(SRCS_PATH)/*/*.test.cpp)
SRCS = $(filter-out $(TEST_FILES), $(ALL_FILES))

CHESS_ENGINE_CPP = $(wildcard $(SRCS_PATH)/chessengine/*.cpp)
CHESS_ENGINE_TESTCPP = $(wildcard $(SRCS_PATH)/chessengine/*.test.cpp)
CHESS_ENGINE_SRCS = $(filter-out $(CHESS_ENGINE_TESTCPP), $(CHESS_ENGINE_CPP))

BOT_SRCS = $(CHESS_ENGINE_SRCS)\
	$(SRCS_PATH)/gameengine/GameEngine.cpp\
	$(SRCS_PATH)/players/BotPlayer.cpp

MINMAX_SRCS = $(BOT_SRCS)\
	$(SRCS_PATH)/agents/MinMaxAgent.cpp\
	$(SRCS_PATH)/heuristics/PiecesHeuristic.cpp

### Compile project main
all: setup test run

### Install python tools for code formatting
setup:
	@mkdir -p bins
	pip install isort
	pip install black
	pip install flake8

### Format python code
format:
	isort python/*
	black python/*
	flake8 python/*

test: utest datasettest perft

### Compile unit tests main
utest:
	@g++ testmains/unit_tests_main.cpp $(CHESS_ENGINE_CPP) -o ./bins/$(TEST_EXEC)
	@./bins/$(TEST_EXEC)

datasettest:
	@g++ testmains/dataset_tests_main.cpp $(CHESS_ENGINE_SRCS) -o ./bins/$(DATASETTEST_EXEC)
	@./bins/$(DATASETTEST_EXEC)

### Compile perft test main
perft:
	@g++ testmains/perft_tests_main.cpp $(CHESS_ENGINE_SRCS) -o ./bins/$(PERFT_EXEC)
	@./bins/$(PERFT_EXEC)

### Compile the actual bot (and run an engine to play against it ?)
run:
	g++ $(flag) mains/main.cpp $(MINMAX_SRCS) -o ./bins/$(BOT_EXEC)

### Compile the actual bot version and test against codingame chess engine
runcg:
	g++ $(flag) mains/maincg.cpp $(MINMAX_SRCS) -o ./bins/$(MM_EXEC)
	cp ./bins/$(MM_EXEC) ../codingame-chess/$(CG_EXEC)
