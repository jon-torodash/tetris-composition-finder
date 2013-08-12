#include "Board.h"

//There is a counterintuitive use of the grid. The coordinates for each piece are cartesian offsets
//since we're moving from left to right, then down to up, but the board itself is a matrix, which in
//C++ follows the standard pattern where rows represent the y axis, columns the x axis.

	Board::Board(){
		//1 column will be standard. a Board's size can be altered later with setSize(int).
		size=1;
		Board::grid = new Cell*[4];
		for(int i=0;i<4;i++)
			grid[i]=new Cell[1];
	}
	
	Board::~Board(){
		//Destructor must be written to avoid memory leaks.
		for(int i=0; i<4; i++)
			delete [] grid[i];
		delete [] grid;
	}

//This is the brains of the operation: a board analyzes whether it can place a piece.
bool Board::try_and_place(Piece& chunk, int row, int col){
		int* coords=chunk.coords();
		int hoffset, voffset;
		//We need to check each tile
		for(int i=0;i<6;i+=2){
			voffset=row+coords[i+1]; hoffset=col+coords[i];
			//Does the piece have a tile sticking out of the board's left or right borders?
			if(hoffset<0 || hoffset>size-1)
				return false;
			//Does the piece have a tile sticking sticking out of the top or bottom borders?
			if(voffset<0 || voffset>3)
				return false;
			//Does a tile need to go into a cell that is occupied already by a tile from another piece?
			if(Board::grid[voffset][hoffset].tile!=' ')
				return false;
		}
		//No problems = Success!
		return true;
	}

void Board::place(Piece& chunk, int row, int col){
	//There is no need to check validity.
	//This function is only called when a fit is found.
	//We fill the cells by altering char used in them.
		int* coords=chunk.coords();
		grid[row][col].tile=chunk.image();
		for(int i=0;i<6;i+=2)
			Board::grid[row+coords[i+1]][col+coords[i]].tile=chunk.image();
	}

//Removes a placed piece
void Board::wipe(int* coords,int row, int col){
	for(int i=0;i<6;i+=2)
		Board::grid[row+coords[i+1]][col+coords[i]].tile=' ';
	Board::grid[row][col].tile=' ';
}

//This is the board print function: the std ostream gets extended funcionality.
std::ostream& operator<< (std::ostream& output, Board& a){
		for(int i=3;i>=0;i--){
			output<<"| ";
			for(int j=0;j<a.size;j++){
				output<<" "<<a.grid[i][j].tile<<" ";
			}
			output<<" "<<"|"<<std::endl;
			if(i!=0){
				output<<"|";
				for(int j=0;j<(a.size*3)+1;j++)
					output<<" ";
				output<<" "<<"|"<<std::endl;
			}
		}
		output<<"|";
		for(int j=0;j<(a.size*3)+2;j++)
			output<<"_";
		output<<"|";
		return output;
	}

//Wipe everything.
void Board::clear(){
	for(int i=0;i<4;i++){
		for(int j=0; j<size; j++){
		grid[i][j].tile=' ';
		grid[i][j].piecetotry=0;
		grid[i][j].used=false;
	}
	}
}

//Alters the number of columns
void Board::setSize(int a){
	for(int i=0;i<4;i++){
		delete [] grid[i];
		grid[i]=new Cell[a];
	}
	size=a;
}

//Accessor & Mutator methods

//Keeps track of what the program tried to put in this cell.
int Board::getTileCount(int row, int col){
	return grid[row][col].piecetotry;
}

//What piece's tile is in the cell.
char Board::getTileImage(int row, int col){
	return grid[row][col].tile;
}

//Keeps/Stores track of what the program tried to put in this cell.
void Board::setTileCount(int row, int col, int set){
	grid[row][col].piecetotry=set;
}

//Keeps track of whether the cell is used.
void Board::setUsed(int row, int col, bool status){
	grid[row][col].used=status;
}

//Checks the set flag.
bool Board::isUsed(int row, int col){
	return grid[row][col].used;
}