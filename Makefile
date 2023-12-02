
### Compile & Run project main
run:
	g++ main.cpp srcs/*/* -o chessproject.out
	./chessproject.out

### Compile and copy the resulting executable into codingame chess engine folder, for its GameRunner/GameManager classes
cgrun:
	g++ main_codingame.cpp srcs/*/* -o mychessbot.out
	cp mychessbot.out ../codingame-chess/.

### Compile & Run unittests
test:
	g++ main.test.cpp srcs/*/* -o unittests.out
	./unittests.out
