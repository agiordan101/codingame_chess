
EXEC = ./chessproject
TESTEXEC = ./unittests
CGEXEC = ../codingame-chess/mychessbot.out

flag = -g -O2

SRCS_PATH = srcs
ALL_FILES = $(wildcard $(SRCS_PATH)/*/*.cpp)
TEST_FILES = $(wildcard $(SRCS_PATH)/*/*.test.cpp)

SRCS = $(filter-out $(TEST_FILES), $(ALL_FILES))

### Compile & Run main.cpp
all:
	g++ $(flag) main.cpp $(SRCS) -o $(EXEC)
	$(EXEC)

### Compile & Run unit tests
test:
	g++ main.test.cpp $(ALL_FILES) -o $(TESTEXEC)
	$(TESTEXEC)

### Compile & Copy the resulting executable into codingame chess engine folder, for its GameRunner/GameManager classes
cg:
	g++ $(flag) maincg.cpp $(SRCS) -o $(CGEXEC)

### Compile & Copy the resulting executable into codingame chess engine folder to test my board implementation
cgtest:
	g++ maincg.test.cpp $(SRCS) -o $(CGEXEC)
