//The header file is concise for the sake of getting a code overview.
//The implementation file, Board.cpp, has much more detailed commentary.
//Compiler directives in order for main to use the Board class.
#ifndef Board_h
#define Board_h

#include "Piece.h"
#include <iostream>
#include <vector>




class Board{
private:
	//This struct is only used by the Board class
	struct Cell{
		char tile;
		int piecetotry;
		bool used;
	};
	Cell** grid;
	int size;
	
public:
	Board();
	bool try_and_place(Piece&, int, int);
	void place(Piece&, int, int);
	void wipe(int*coords, int, int);
	friend std::ostream& operator<< (std::ostream&, Board& a);
	void clear();
	void setSize(int);
	int getTileCount(int, int);
	char getTileImage(int row, int col);
	void setTileCount(int, int, int);
	void setUsed(int, int, bool);
	bool isUsed(int, int);
	~Board();
};

#endif