
TEST_EXEC = unittests
CG_EXEC = mychessbot
MM_EXEC = mm2
MMBB_EXEC = mmbb
BITBOARDTEST_EXEC = bbtest

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
all: test mmbb mm2

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

### Compile unit tests main
test:
	@g++ mains/unittests_main.cpp $(CHESS_ENGINE_CPP) -o ./bins/$(TEST_EXEC)
	./bins/$(TEST_EXEC)

### Compile the bots and copy them into codingame chess engine folder
mm2:
	g++ $(flag) mains/maincg.cpp $(MINMAX_SRCS) -o ./bins/$(MM_EXEC)
	cp ./bins/$(MM_EXEC) ../codingame-chess/$(CG_EXEC)

mmbb:
	g++ $(flag) mains/maincg.cpp $(MINMAX_SRCS) -o ./bins/$(MMBB_EXEC)
	cp ./bins/$(MMBB_EXEC) ../codingame-chess/$(CG_EXEC)

bbtest:
	g++ $(flag) mains/main_test_bitboard.cpp $(MINMAX_SRCS) -o ./bins/$(BITBOARDTEST_EXEC)
	./bins/$(BITBOARDTEST_EXEC)
