TEST_EXEC = unittests
PERFT_EXEC = perft
DATASETTEST_EXEC = datasettest
TIMETEST_EXEC = timetest
RANDOMTEST_EXEC = randomtest

DEBUG_EXEC = debug

CG_EXEC = mychessbot
BOT_VERSION = BbMmabPv-25ms-13.1.8

flag = -O3 -Wall -Wextra -Werror -Wno-unknown-pragmas

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
	$(SRCS_PATH)/agents/MinMaxAlphaBetaAgent.cpp\
	$(SRCS_PATH)/agents/AbstractAgent.hpp\
	$(SRCS_PATH)/agents/MctsAgent.cpp\
	$(SRCS_PATH)/heuristics/PiecesHeuristic.cpp

### Compile project main
all: setup test run

### Install python tools for code formatting
setup:
	mkdir -p bins cgmains
	pip install isort
	pip install black
	pip install "black[jupyter]"
	pip install flake8

### Format python code
format:
	clang-format -i -- */*.cpp */*/*.cpp */*/*.hpp
	isort python/*
	black python/*

test: utest datasettest perft

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

randomtest:
	g++ -g3 $(flag) testmains/random_tests_main.cpp $(BOT_SRCS) -o ./bins/$(RANDOMTEST_EXEC)
	./bins/$(RANDOMTEST_EXEC)

### Compile perft test main
perft:
	@g++ testmains/perft_tests_main.cpp $(CHESS_ENGINE_SRCS) -o ./bins/$(PERFT_EXEC)
	@./bins/$(PERFT_EXEC)

### Compile the actual bot for debugging
debug:
	g++ -g3 $(flag) testmains/unit_tests_main.cpp $(CHESS_ENGINE_CPP) -o ./bins/$(DEBUG_EXEC)

### Group source code in one file mains/maincg.cpp, save it in local, and compile the bot
run:
	python3 python/codingame_file_creator.py
	clang-format -i -- ./mains/maincg.cpp
	cp ./mains/maincg.cpp ./cgmains/cgmain_$(BOT_VERSION).cpp

	g++ $(flag) ./mains/maincg.cpp -o ./bins/mychessbot
	cp ./bins/mychessbot ./bins/$(BOT_VERSION)

### Just test some cpp behaviors
poc:
	g++ $(flag) mains/main_poc.cpp $(BOT_SRCS) -o ./bins/main_poc
	./bins/main_poc
