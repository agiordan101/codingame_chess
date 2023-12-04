
### Compile & Run main.cpp
run:
	time g++ main.cpp srcs/*/* -o chessproject.out
	time ./chessproject.out

### Compile & Run unit tests
test:
	g++ main.test.cpp srcs/*/* -o unittests.out
	./unittests.out

### Compile & Copy the resulting executable into codingame chess engine folder, for its GameRunner/GameManager classes
cgrun:
	g++ maincg.cpp srcs/*/* -o mychessbot.out
	cp mychessbot.out ../codingame-chess/.

### Compile & Copy the resulting executable into codingame chess engine folder to test my board implementation
cgtest:
	g++ maincg.test.cpp srcs/*/* -o mychessbot.out
	cp mychessbot.out ../codingame-chess/.
