
EXEC = ./bins/chessproject
TESTEXEC = ./bins/unittests
CGEXEC1 = ../codingame-chess/mychessbot
CGEXEC2 = ../codingame-chess/mychessbot2

flag = -g -O2

SRCS_PATH = srcs
ALL_FILES = $(wildcard $(SRCS_PATH)/*/*.cpp)
TEST_FILES = $(wildcard $(SRCS_PATH)/*/*.test.cpp)

SRCS = $(filter-out $(TEST_FILES), $(ALL_FILES))

### Compile project main
all:
	g++ $(flag) mains/main.cpp $(SRCS) -o $(EXEC)

### Compile unit tests main
test:
	@g++ mains/main.test.cpp $(ALL_FILES) -o $(TESTEXEC)
	$(TESTEXEC)

### Compile the bot into codingame chess engine folder, for its GameRunner/GameManager classes
cg:
	g++ $(flag) mains/maincg.cpp $(SRCS) -o $(CGEXEC1)

cg2:
	g++ $(flag) mains/maincg.cpp $(SRCS) -o $(CGEXEC2)

### Compile a random bot into codingame chess engine folder to test my board implementation
cgtest:
	g++ mains/maincg.test.cpp $(SRCS) -o $(CGEXEC1)
