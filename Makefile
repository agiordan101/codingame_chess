
TEST_EXEC = unittests
CG_EXEC = mychessbot
MM2_EXEC = mm2
MM3_EXEC = mm3
MMITERDEEP_EXEC = mmid

flag = -g -O2

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

MINMAXITERDEEP_SRCS = $(BOT_SRCS)\
	$(SRCS_PATH)/agents/MinMaxIterDeepAgent.cpp\
	$(SRCS_PATH)/heuristics/PiecesHeuristic.cpp


### Compile project main
all: test mm2 mm3

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
	@g++ mains/main.test.cpp $(ALL_FILES) -o ./bins/$(TEST_EXEC)
	./bins/$(TEST_EXEC)

### Compile the bots and copy them into codingame chess engine folder
mm2:
	g++ $(flag) mains/maincg_$(MM2_EXEC).cpp $(MINMAX_SRCS) -o ./bins/$(MM2_EXEC)
	cp ./bins/$(MM2_EXEC) ../codingame-chess/$(CG_EXEC)

mm3:
	g++ $(flag) mains/maincg_$(MM3_EXEC).cpp $(MINMAX_SRCS) -o ./bins/$(MM3_EXEC)
	cp ./bins/$(MM3_EXEC) ../codingame-chess/$(CG_EXEC)

mmid:
	g++ $(flag) mains/maincg_$(MMITERDEEP_EXEC).cpp $(MINMAXITERDEEP_SRCS) -o ./bins/$(MMITERDEEP_EXEC)
	cp ./bins/$(MMITERDEEP_EXEC) ../codingame-chess/$(CG_EXEC)
