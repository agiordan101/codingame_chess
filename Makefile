
TEST_EXEC = unittests
PERFT_EXEC = perft
DATASETTEST_EXEC = datasettest
TIMETEST_EXEC = timetest
CG_EXEC = mychessbot
BOT_EXEC = BbMmPv

flag = -g -O2 # -Wall -Wextra

SRCS_PATH = srcs
ALL_FILES = $(wildcard $(SRCS_PATH)/*/*.cpp)
TEST_FILES = $(wildcard $(SRCS_PATH)/*/*.test.cpp)
SRCS = $(filter-out $(TEST_FILES), $(ALL_FILES))

CHESS_ENGINE_CPP = $(wildcard $(SRCS_PATH)/chessengine/*.cpp)
CHESS_ENGINE_TESTCPP = $(wildcard $(SRCS_PATH)/chessengine/*.test.cpp)
CHESS_ENGINE_SRCS = $(filter-out $(CHESS_ENGINE_TESTCPP), $(CHESS_ENGINE_CPP))

TEST_SRCS = $(CHESS_ENGINE_SRCS)\
	$(SRCS_PATH)/heuristics/PiecesHeuristic.cpp

BOT_SRCS = $(CHESS_ENGINE_SRCS)\
	$(SRCS_PATH)/gameengine/GameEngine.cpp\
	$(SRCS_PATH)/players/BotPlayer.cpp\
	$(SRCS_PATH)/agents/MinMaxAgent.cpp\
	$(SRCS_PATH)/heuristics/PiecesHeuristic.cpp

BOTTEST_SRCS = $(CHESS_ENGINE_SRCS)\
	$(SRCS_PATH)/gameengine/GameEngineIntTests.cpp\
	$(SRCS_PATH)/players/BotPlayer.cpp\
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
	clang-format -i -- */*.cpp */*/*.cpp */*/*.hpp
	isort python/*
	black python/*

test: utest datasettest perft cgtest

### Compile unit tests main
utest:
	@g++ testmains/unit_tests_main.cpp $(CHESS_ENGINE_CPP) -o ./bins/$(TEST_EXEC)
	@./bins/$(TEST_EXEC)

datasettest:
	@g++ testmains/dataset_tests_main.cpp $(CHESS_ENGINE_SRCS) -o ./bins/$(DATASETTEST_EXEC)
	@./bins/$(DATASETTEST_EXEC)

timetest:
	@g++ testmains/time_tests_main.cpp $(BOT_SRCS) -o ./bins/$(TIMETEST_EXEC)
	@./bins/$(TIMETEST_EXEC)

### Compile perft test main
perft:
	@g++ testmains/perft_tests_main.cpp $(CHESS_ENGINE_SRCS) -o ./bins/$(PERFT_EXEC)
	@./bins/$(PERFT_EXEC)

### Compile the actual bot with a special GameEngine, to compare codingame chess engine and mine
cgtest:
	g++ testmains/cg_tests_main.cpp $(BOTTEST_SRCS) -o ./bins/$(CG_EXEC)
	cp ./bins/$(CG_EXEC) ../codingame-chess/$(CG_EXEC)

### Compile the actual bot to run it against the codingame engine
runcg:
	g++ $(flag) mains/maincg.cpp $(BOT_SRCS) -o ./bins/$(CG_EXEC)
	cp ./bins/$(CG_EXEC) ../codingame-chess/$(CG_EXEC)

### Compile the actual bot (and run an engine to play against it ?)
run:
	g++ $(flag) mains/main.cpp $(BOT_SRCS) -o ./bins/$(BOT_EXEC)
