all:
		test -d build || mkdir build
		g++ src/main.cpp src/Board.cpp src/Piece.cpp -o build/tetris