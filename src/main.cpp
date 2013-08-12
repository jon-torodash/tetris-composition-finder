/********** TETRIS COMPOSITION FINDER ***********
					By Jon Torodash

***************************************************/


#include "Piece.h"
#include "Board.h"
#include <vector>
using namespace std;

//This is called by the first task to check whether the (de)composition 
//would sum up to 10, a Tetris board's size
int sumOfComposition(int a[], int size){
	int summation=0;
	bool oneappears=false;
	for(int i=0; i<size; i++){
		summation+=a[i];
		if(a[i]==1)
			oneappears=true;
	}
	//The program must not consider valid any composition without a 1, so we force a bad result if
	//it lacks a 1 by adding a value that the composing caller function will reject.
	if(!oneappears)
		summation+=99999999;
	return summation;
}


//(De)compositions are made on an array for speed and conciseness,
//But are stored in a vector for later use and recall.
void compCopy(vector<vector<int> >& complist, int a[], int size){
	vector<int> b;
	for(int i=0; i<size; i++)
		b.push_back(a[i]);
	complist.push_back(b);
}

//The first major task: find the partitions. This task is the lighter one and will be called from main.
//This basically treats (de)compositions like a numerical base system.
//In a a given number of slots for a decomposition, the biggest group only be maxboxsize-slots+2
void compose(vector<vector<int> >& complist){
	//Declare what's needed. The compbuilder will have 2 up to 11 slots, and we keep track of the one we're on.
	int* compbuilder;
	int slottowork;
	const int maxboxsize=9;
	//The number of compositions will be counted uniquely for each quantity of available boxes, 1 through 10.
	//We don't care about size 0 (no blocks no tetris) or size 1 (Full 10 width {10} lacks the needed "1" to be a tetris).
	for(int slots=2; slots<11; slots++){
		//Resize and clear everything.
		slottowork=slots-1;
		compbuilder = new int[slots];
		//This will begin our search just where the smaller size left off.
		//We initialize to 1, since we don't care about group sizes of 0.
		//In combinatoric terms, we are interested in non-weak compositions only.
		for(int i=0; i<slots; i++)
			compbuilder[i]=1;
		//The main loop
		//In a partition of a given size, the biggest can only be maxboxsize-slots+2
		//Otherwise at least one slot will have size 0.
		while(compbuilder[0]<maxboxsize-slots+3){
			if(compbuilder[slottowork]<maxboxsize-slots+3){
				//Did we find enough to do something? If so, print, store.
				//In any event, we need to backtrack to try the next.
				if(sumOfComposition(compbuilder,slots)>=10){
					if(sumOfComposition(compbuilder,slots)==10){
						//Need to store a deep copy for later use.
						compCopy(complist,compbuilder,slots);
					}
					//Backtrack and move on. We simulate a numerical base system.
					//Increment the slot we're on by 1, and zero out the lower orders in order to
					//find all good compositions with this new change.
					//Also, first make sure the increment bubbles all the way up.
					while(compbuilder[slottowork]==maxboxsize-slots+3 && slottowork!=-1){
						compbuilder[slottowork]=1;
						compbuilder[--slottowork]++;
					}
					//Are we finished?
					if(slottowork==-1) continue;
					slottowork=slots-1;
				}
				compbuilder[slottowork]++;
			}
			else{//This slot size is beyond its max, so backtrack and move on.
				 //Backtrack - make increment bubble up
				while(compbuilder[slottowork]==maxboxsize-slots+3 && slottowork!=-1){
					compbuilder[slottowork]=1;
					compbuilder[--slottowork]++;
				} 
				//Are we finished?
				if(slottowork==-1) break;
				//Start looking at the front again.
				slottowork=slots-1;
			}
		}
		delete [] compbuilder;
	}
	for (int m=0; m<complist.size(); m++){
		cout<<"{"<<complist[m][0];
		for(int n=1; n<complist[m].size();n++){
			cout<<","<<complist[m][n];
		}
		cout<<"}"<<" composition #"<<m+1<<endl;
	}
	
}


int main (int argc, char * const argv[]) {
	char gostepbystep; char pause; bool stepbystep;
	cout<<"***If you want the program to run through in its entirety, uninterrupted,"
		<<" enter '0'. If you want it to go step by step for each task, enter any other input.";
	gostepbystep = getchar();
	stepbystep = (gostepbystep == '0') ? false : true;
	cout<<endl<<endl;
	cout<<"About to initalize the variables needed for the program: pieces, places to store compositions, and the board."<<endl;
	if(stepbystep){
		cout<<"Hit Enter to continue.";
		getchar();
	}
	//We initialize the needed variables and our Piece prototype list.
	vector<vector<int> > complist; // This will hold the compositions for the board that we found.
	Piece piecelist[18]; //A table describing the pieces
	int pieceplacer=0; //For seeding the above table
	int piecetotry; //This will save us unnecessary function calls later in the program
	bool donefornow; //This will let us know, once we've placed a piece in a cell, to stop placing there fow now.
	long long partitionperms[10]={0,0,0,0,0,0,0,0,0,0}; //Incrementing and finalizing the count for these is the holy grail.
														//Seed the values we know.
	partitionperms[0]=0; //This value will not actually be used, since we avoid weak compositions.
	partitionperms[1]=1; //Trivial case: only a single vertical piece can fill a partition size of width 1.
	int row, col; //Our coordinates for the program as we move through rows and cols on the boards.
	
	//Our board
	Board b;
	
	//Our piece list: the first six numbers are three x,y paired offset coords from the tile that is leftmost and downmost
	//in the piece. The 7th character is the char for what the tile will look like in the board for easier viewing,
	//and the 8th value is how much higher the next open tile in the current column will be after placing this piece.
	//The naming and ordering of the pieces is irrelevant. We will always refer to them from the table (piecelist) hereon.
	
	//Straight piece
	Piece I1(1,0,2,0,3,0,'-',1);
	piecelist[pieceplacer++]=I1;
	//Square
	Piece O(0,1,1,1,1,0,'O',2);
	piecelist[pieceplacer++]=O;
	//The Z
	Piece Z1(1,0,1,-1,2,-1,'Z',1);
	piecelist[pieceplacer++]=Z1;
	Piece Z2(0,1,1,1,1,2,'%',2);
	piecelist[pieceplacer++]=Z2;
	//The S
	Piece S1(1,0,1,1,2,1,'S',1);
	piecelist[pieceplacer++]=S1;
	Piece S2(0,1,1,0,1,-1,'4',2);
	piecelist[pieceplacer++]=S2;
	//The T
	Piece T1(1,-1,1,0,2,0,'T',1);
	piecelist[pieceplacer++]=T1;
	Piece T2(0,1,1,1,0,2,'E',3);
	piecelist[pieceplacer++]=T2;
	Piece T3(1,0,1,1,2,0,'W',1);
	piecelist[pieceplacer++]=T3;
	Piece T4(1,0,1,-1,1,1,'3',1);
	piecelist[pieceplacer++]=T4;
	//The J
	Piece J1(1,0,1,1,1,2,'J',1);
	piecelist[pieceplacer++]=J1;
	Piece J2(1,0,2,-1,2,0,'=',1);
	piecelist[pieceplacer++]=J2;
	Piece J3(0,1,0,2,1,2,'P',3);
	piecelist[pieceplacer++]=J3;
	Piece J4(1,0,2,0,0,1,'\\',2);
	piecelist[pieceplacer++]=J4;
	//The L
	Piece L1(1,0,0,1,0,2,'L',3);
	piecelist[pieceplacer++]=L1;
	Piece L2(1,0,2,0,2,1,'/',1);
	piecelist[pieceplacer++]=L2;
	Piece L3(1,0,1,-1,1,-2,'1',1);
	piecelist[pieceplacer++]=L3;
	Piece L4(0,1,1,1,2,1,'r',2);
	piecelist[pieceplacer++]=L4;
	
	cout<<endl<<"Initialization finished. About to find the (de)compositions of the board."<<endl;
	if(stepbystep){
		cout<<"Hit Enter to continue.";
		getchar();
	}
	 cout<<endl<<endl<<"The following are the ways to compose the tetris board:"<<endl<<endl;
	 //Find the total number of compositions
	 compose(complist);
	 cout<<"Done finding the compositions."<<endl<<endl;
	 cout<<"Ready to construct the perfect fills for boards size 1 - 9."<<endl;
	 if(stepbystep){
		cout<<"Hit Enter to continue.";
		getchar();
	 }
	//The second major task: finding the number of rectangular formulations for each width of a tetris board, 2 to 9.
	//Since we don't use the straight vertical, no sense in checking size(1)==1. A tetris cannot exist with a width 10.
	for(int size=2; size<10; size++){
		b.setSize(size);
		b.clear();
		cout<<endl<<endl<<"The following are the board width "<<size<<" perfect rectangular configurations:"<<endl<<endl;
		row=0, col=0;
		//We cut for size n when we backtrack off the board left.
		while(col!=-1){ 
			//Do we have a full config? If so, print and the backtrack.
			if(col==size){
				cout<<b<<endl<<"Board size "<<size<<", solution #"<<++partitionperms[size]<<endl<<endl;
				//Find the most recently placed piece by tracing backward, looking for the "used" flag.
				//First get back on the board from the prior height increment.
				row=3; col--;
				while(!b.isUsed(row,col)){
					row--;
					if(row==-1){//Move modularly
						row=3;
						col--;
					}
				}
				//Remove the piece (-1 since we incremented the piece to try when placing in this cell)
				b.wipe(piecelist[b.getTileCount(row,col)-1].coords(),row,col);
				b.setUsed(row,col,false);
			}
			donefornow=false;
			piecetotry=b.getTileCount(row,col);
			while(piecetotry<18){
				//cout<<b<<endl<<endl;
				//Does the next piece in line to be attempted for this cell fit?
				if(b.try_and_place(piecelist[piecetotry],row,col)){
					//Place the piece.
					b.place(piecelist[piecetotry],row,col);
					//Keep track of trying this cell.
					b.setTileCount(row,col,piecetotry+1);
					b.setUsed(row, col, true);
					//Find out how high it now sets us
					row+=piecelist[piecetotry].height();
					//If other tiles are covering the expected open cell, we
					//need to keep looking sequentially for the next available.
					if(row==4){
						row=0;
						col++;
					}
					while(b.getTileImage(row,col)!=' '){
						row++;
						if(row==4){//Move modularly
							row=0;
							col++;
							if(col==size)
								break;
						}
					}
					//Use this to break out of the loop for a given cell after placing in it
					donefornow=true;
				}
				else{
					b.setTileCount(row,col,++piecetotry);
				}
				if(donefornow) break;
			}
			//Did we finish trying to place in the cell because we exhausted all possible options? BACKTRACK.
			if(piecetotry==18){
				//Reset our efforts on that cell
				b.setTileCount(row,col,0);
				//Find the cell of the most recently prior placed piece by tracing backward.
				while(!b.isUsed(row,col) && col>-1){
					row--;
					if(row==-1){//Move modularly
						row=3;
						col--;
						if(col==-1)
							break;
					}
				}
				//Remove the last placed piece (-1 since we incremented the piece to try next when placing in this cell)
				if(col==-1)
					continue;
				b.wipe(piecelist[b.getTileCount(row,col)-1].coords(),row,col);
				b.setUsed(row,col,false);
			}
			//We must have managed to successfully place a piece.
			else{
				//Need to check for bleed-over if starting a new column. If there is none,
				//then we're double-counting a smaller-sized tetris board and need to backtrack.
				//Of course, if col==size, we've found a full config and ignore this check.
				if(col<size && row==0){
					int bleedrow=0;
					for(bleedrow=0;bleedrow<4;bleedrow++)
						if(b.isUsed(bleedrow,col))
						   break;
					//There is no bleed-over.
					if(bleedrow==4){
						while(!b.isUsed(row,col)){
							row--;
							if(row==-1){//Move modularly
								row=3;
								col--;
							}
							if(col==-1)
								break;
						}
						if(col==-1)
							continue;
						//Remove the piece (-1 since we incremented the piece to try when placing in this cell)
						b.wipe(piecelist[b.getTileCount(row,col)-1].coords(),row,col);
						b.setUsed(row,col,false);
						continue;
					}
				}
			}
		}
		
		//Next col
		col++;
		row=0;
	}
	
	//Synthesize the results of both tasks.
	//Report what we found for each board size.
	cout<<endl<<"All done counting fills for smaller board sizes:"<<endl;
	for(int y=0; y<10;y++)
		cout<<"Board size 4x"<<y<<" has "<<partitionperms[y]<<" fill permutations."<<endl;
	cout<<endl<<endl<<"About to put together these permutation numbers with the results of the first task."<<endl;
	if(stepbystep){
		cout<<"Hit Enter to continue.";
		getchar();
	}
	//Declare variables for the final count.
	int onescount;
	long long finalsolution1=0, finalsolution2=0, solutionsonthiscomposition;
	//For each composition in the list...
	for(int i=0; i<complist.size(); i++){
		solutionsonthiscomposition=1; onescount=0;
		//...multiply into each box of the composition the number of perfect tilings for that box's size
		for(int j=0; j<complist[i].size(); j++){
			solutionsonthiscomposition*=partitionperms[complist[i][j]];
			if(complist[i][j]==1)
				onescount++;
		}
		finalsolution1+=solutionsonthiscomposition;
		finalsolution2+=solutionsonthiscomposition*onescount;
	}
	cout<<endl<<endl<<"*** The GRAND TOTAL number of tetris configurations that can be made on the bottom of the board with no excess tiles in Tetris is: "<<finalsolution1;
	cout<<endl<<endl<<"*** The GRAND TOTAL number of ways one can ACTIVATE such a tetris is: "<<finalsolution2;
	return 0;
}
