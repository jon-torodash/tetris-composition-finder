//Compiler directives in order for main to use the Piece class.
#ifndef Piece_h
#define Piece_h

class Piece{
private:
	//The lowest, leftest tile of each piece/rotation is considered its "index" tile for the sake of orderly
	//placement on the Tetris board in the search. The other 3 tiles that make up the piece have their coordinates
	//stores as relative cartesian offsets from this index tile. This completely represents the piece.
	int coordinates [6];
	//Its representation
	char tile;
	//Once placed, how many cells higher the program's attention will shift based on what is now open on the board.
	int howtall;
public:
		//Nothing interesting happens in the constructor, so it is defined here in the header file.
		Piece(const int a,const int b,const int c,const int d,const int e,const int f,const char i,const int j) : tile(i), howtall(j){
			coordinates[0]=a;
			coordinates[1]=b;
			coordinates[2]=c;
			coordinates[3]=d;
			coordinates[4]=e;
			coordinates[5]=f;
		}
	//Default constructor, getters and setters. Nothing unusual.
	Piece(){}
	int* coords();
	char image();
	int height();
};

#endif