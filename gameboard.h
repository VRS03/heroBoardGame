/*
    Filename: "gameboard.h"
    Author: Viraj Saudagar

    This file defines the GameBoard class which contains the Grid of 
    BoardCells that exhibit polymorphic behavior. The class contains 
    the functionality required to run the game and execute custom moves
    for the hero. 

*/

#ifndef _GAMEBOARD_H
#define _GAMEBOARD_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>

#include "boardcell.h"
#include "grid.h"

using namespace std;

class GameBoard {
	private: 
	    Grid<BoardCell*> board;
        size_t numRows;
        size_t numCols;
        size_t HeroRow; // Hero's position row
	    size_t HeroCol; // Hero's position column
        int numMonsters;
        int numSuperMonsters;
        int numAbysses;
        int numBats;
        bool wonGame; // false, unless the Hero reached the exit successfully

		
	public: 
		/* default constructor */
        GameBoard() {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 50;
            numBats = 2;
            wonGame = false;
            
            this -> numRows = 15;
            this -> numCols = 40;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* param constructor */
        GameBoard(size_t numRows, size_t numCols) {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 20;
            numBats = 3;
            wonGame = false;
            
            this -> numRows = numRows;
            this -> numCols = numCols;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* destructor */
        virtual ~GameBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    delete board(row, col);
                }
            }
        }

        void blankBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    board(row, col) = new Nothing(row,col);
                }
            }
        }

        char getCellDisplay(size_t r, size_t c) {
            return board(r,c)->display();
        }

        void setCell(BoardCell* myCell, size_t r, size_t c) {
            board(r,c) = myCell;
        }
    
        void freeCell(size_t r, size_t c) {
            delete board(r,c);
        }

        // fills board with by randomly placing...
        //  - Hero (H) in the first three columns
        //  - EscapeLadder (*) in last three columns
        //  - 3 vertical Walls (+), each 1/2 of board height, in middle segment
        //  - Abyss cells (#), quantity set by numAbysses, in middle segment
        //  - Baddies [Monsters (m), Super Monsters (M), & Bats (~)] in middle segment;
        //    number of Baddies set by numMonsters, numSuperMonsters, & numBats
        void setupBoard(int seed) {
            srand(seed);
            size_t r,c;

            r = rand() % numRows;
            c = rand() % 3;
            delete board(r,c);
            board(r,c) = new Hero(r,c);
            HeroRow = r;
            HeroCol = c;

            r = rand() % numRows;
            c = numCols - 1 - (rand() % 3);
            delete board(r,c);
            board(r,c) = new EscapeLadder(r,c);
            
            int sizeMid = numCols - 6;

            c = 3 + (rand() % sizeMid);
            for (r = 0; r < numRows/2; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }
            size_t topc = c;

            while (c == topc || c == topc-1 || c == topc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows-1; r > numRows/2; --r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);           
            }
            size_t botc = c;

            while (c == topc || c == topc-1 || c == topc+1 || c == botc || c == botc-1 || c == botc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows/4; r < 3*numRows/4; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }

            for (int i = 0; i < numMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c);  
                board(r,c)->setPower(1);        
            }

            for (int i = 0; i < numSuperMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c); 
                board(r,c)->setPower(2);               
            }

            for (int i = 0; i < numBats; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Bat(r,c); 
            }

            for (int i = 0; i < numAbysses; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Abyss(r,c);              
            }
        }

        // neatly displaying the game board 
		void display( ) {
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            for (size_t row = 0; row < board.numrows(); row++) {
                cout << '|';
                for (size_t col = 0; col < board.numcols(row); col++) {
                    cout << board(row,col)->display();
                }
                cout << '|';
                cout << endl;
            }
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            
        }
		
        bool getWonGame() {
            return wonGame;
        }
        
        // distributing total number of monsters so that 
        //  ~1/3 of num are Super Monsters (M), and
        //  ~2/3 of num are Regular Monsters (m)
        void setNumMonsters(int num) {
            numSuperMonsters = num/3;
            numMonsters = num - numSuperMonsters;
        }

        void setNumAbysses(int num) {
            numAbysses = num;
        }

        void setNumBats(int num) {
            numBats = num;
        }

        size_t getNumRows() {
            return numRows;
        }

        size_t getNumCols() {
            return numCols;
        }

        
        //---------------------------------------------------------------------------------
        // void getHeroPosition(size_t& row, size_t& col)
        //
        // getter for Hero's position, which are private data members
        //      int HeroRow;
	    //      int HeroCol;
        // note: row and col are passed-by-reference
        //---------------------------------------------------------------------------------
        void getHeroPosition(size_t& row, size_t& col) {
            row = this->HeroRow;
            col = this->HeroCol;
        }

        
        //---------------------------------------------------------------------------------
        // void setHeroPosition(size_t row, size_t col)
        //
        // setter for Hero's position, which are private data members
        //      int HeroRow;
	    //      int HeroCol;
        //---------------------------------------------------------------------------------
        void setHeroPosition(size_t row, size_t col) {
            this->HeroRow = row;
            this->HeroCol = col;
        }

        
        //---------------------------------------------------------------------------------
        // findHero()
        //
        // updater for Hero's position, which are private data members
        //      int HeroRow;
	    //      int HeroCol;
        // this function should find Hero in board and update
        //      HeroRow and HeroCol with the Hero's updated position;
        // if Hero cannot be found in board, then set Hero's position to (-1,-1)
        //---------------------------------------------------------------------------------
        void findHero() {
            
            for(size_t r = 0; r < this->board.numrows(); r++){

                for(size_t c = 0; c < this->board.numcols(r); c++){
                    
                    if(this->board(r, c)->display() == 'H'){
                        this->HeroRow = r;
                        this->HeroCol = c;
                        setHeroPosition(r, c);
                        return;
                    }

                }

            }

            setHeroPosition(-1, -1);
        
        }

        /*
            Simulates the movement of the baddies currently on the board for an entire round. 
            Returns true if the hero baddies captured the hero or false if they did not. 
        */
        bool moveBaddies(){

            bool gotHero = false;

            // Traverse the board
            for(size_t r = 0; r < board.numrows(); r++){

                for(size_t c = 0; c < board.numcols(r); c++){

                    if(board(r, c)->isBaddie() && board(r, c)->getMoved() == false){

                        
                        // TODO: at some point, update the myRow & myCol values in each monster object.
                        size_t newR, newC;
                        board(r, c)->attemptMoveTo(newR, newC, HeroRow, HeroCol);


                        // 1. Baddie tries to move out-of-bounds in rows. 
                        try {
                            if (newR < 0 || newR >= numRows) { 
                                throw runtime_error("Baddie trying to move out-of-bounds with an invalid row");
                            } 
                        }
                        catch (runtime_error& excpt) {
                            
                            cout << excpt.what() << endl;
                            newR = r;
                            cout << "Changing row for Baddie position to stay in-bounds" << endl;

                        }

                        // 2. Baddie tries to move out-of-bounds in columns. 
                        try{
                            if(newC < 0 || newC >= numCols){
                                throw runtime_error("Baddie trying to move out-of-bounds with an invalid column");
                            }
                        }
                        catch(runtime_error& excpt){
                            cout << excpt.what() << endl;
                            newC = c;
                        }

                        // 3. Baddie tries to move on a Wall cell OR the escape cell.
                        try{
                            if(board(newR, newC)->display() == '+' || board(newR, newC)->display() == '*'){
                                throw runtime_error("Baddie is trying to move on a Wall cell or Escape Cell");
                            }
                        }
                        catch(runtime_error& excpt){

                            cout << excpt.what() << endl;

                            
                            // Moving perfeclty horizontal to a wall
                            if((newR == r && board(newR, newC)->display() == '+') || (newR == r && board(newR, newC)->display() == '*')){
                                newC = c;
                                newR = r;
                            } // Moving perfeclty vertical to a wall
                            else if((newC == c && board(newR, newC)->display() == '+') || (newC == c && board(newR, newC)->display() == '*')){
                                newC = c;
                                newR = r;
                            } // Moving diagonal to a wall. 
                            else{
                                // 1. Horizontal Movement is ignored. 
                                if(board(newR, c)->display() != '+' || board(newR, c)->display() != '*'){
                                    newC = c;
                                }
                                else{
                                    newR = r;
                                    newC = c;
                                }
                                // Check if ignoring horizontal still hits a wall.
                            }
                        

                            cout << "Changing Row and/or Column for Baddie position to avoid wall or escape cell" << endl;

                        }

                        // 4. Baddie Tries to move on an Abyss cell.
                        try{
                            if(board(newR, newC)->display() == '#'){
                                throw runtime_error("Baddie is trying to move on a abyss cell");
                            }
                        }
                        catch(runtime_error& excpt){

                            cout << excpt.what() << endl;
                            delete board(r, c);
                            board(r, c) = new Nothing(r, c);
                            continue;

                        }

                        // 5. TODO: Do we need this? -> Baddie moves into another baddie

                        // 6. Baddie moves into the hero.
                        try{
                            if(board(newR, newC)->display() == 'H'){
                                throw runtime_error("Baddie is trying to move on the hero cell");
                            }
                        }
                        catch(runtime_error& excpt){

                            cout << excpt.what() << endl;
                            board(r, c)->setMoved(true);
                            delete board(newR, newC);
                            BoardCell* temp = board(r, c);
                            board(newR, newC) = temp;
                            board(r, c) = new Nothing(r, c);
                            this->wonGame = false;
                            gotHero = true;
                            continue;

                        }

                        // Set moved to true
                        board(r, c)->setMoved(true);

                        // Execture the move.
                        if(newR == r && newC == c){
                            // same position so no new move.
                            continue;
                        }
                        else{

                            board(r, c)->update(newR, newC);
    
                            delete board(newR, newC);
                            BoardCell* temp = board(r, c);
                            board(newR, newC) = temp;
                            board(r, c) = new Nothing(r, c);

                            
                        }

                    }

                }

            }

            findHero();
            return gotHero;

        }

        void setBaddieMovedToFalse(){

            for(size_t r = 0; r < numRows; r++){
                for(size_t c = 0; c < numCols; c++){

                    if(board(r, c)->isBaddie()){
                        board(r, c)->setMoved(false);
                    }

                }
            }

        }

        /*  
            Simulates the movement of the hero and the baddies during a round of gameplay based on the 
            input heroNextMove deicision. The function returns true if the hero is still on the board
            at the end of the round & false if the hero is not on the board at the end of the round.
        */
        bool makeMoves(char HeroNextMove) {


            // determine where hero proposes to move to
            setBaddieMovedToFalse();
            size_t newR, newC;
            board(HeroRow,HeroCol)->setNextMove(HeroNextMove);
            board(HeroRow,HeroCol)->attemptMoveTo(newR,newC,HeroRow,HeroCol);

            // 1. Hero tries to move out-of-bounds in rows. 
            
            try {
                // hero attempts to move out-of-bounds in rows
                if (newR < 0 || newR >= numRows) { 
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid row");
                } 
            }
            catch (runtime_error& excpt) {
                
                cout << excpt.what() << endl;
                newR = HeroRow;
                cout << "Changing row for Hero position to stay in-bounds" << endl;

            }

            // 2. Hero tries to move out-of-bounds in columns. 
            try{
                if(newC < 0 || newC >= numCols){
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid column");
                }
            }
            catch(runtime_error& excpt){
                cout << excpt.what() << endl;
                newC = HeroCol;
            }

            // 3. Hero tries to move on a Wall cell. 
            try{
                if(board(newR, newC)->display() == '+'){
                    throw runtime_error("Hero is trying to move on a Wall cell");
                }
            }
            catch(runtime_error& excpt){

                cout << excpt.what() << endl;

                
                // Moving perfeclty horizontal to a wall
                if(newR == HeroRow && board(newR, newC)->display() == '+'){
                    newC = HeroCol;
                    newR = HeroRow;
                } // Moving perfeclty vertical to a wall
                else if(newC == HeroCol && board(newR, newC)->display() == '+'){
                    newC = HeroCol;
                    newR = HeroRow;
                } // Moving diagonal to a wall. 
                else{
                    // 1. Horizontal Movement is ignored. 
                    if(board(newR, HeroCol)->display() != '+'){
                        newC = HeroCol;
                    }
                    else{
                        newR = HeroRow;
                        newC = HeroCol;
                    }
                    // Check if ignoring horizontal still hits a wall.
                }
            

                cout << "Changing Row and/or Column for Hero position to avoid wall" << endl;

            }

            // 4. Hero reaches escape ladder.
            try{
                if(board(newR, newC)->display() == '*'){
                    throw runtime_error("Hero is trying to escape");
                }
            }
            catch(runtime_error& excpt){

                cout << excpt.what() << endl;
                findHero();
                delete board(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                this->wonGame = true;
                return false;

            }

            // 5. Hero tries to move on an abyss cell. 
            try{
                if(board(newR, newC)->display() == '#'){
                    throw runtime_error("Hero is trying to move on a abyss cell");
                }
            }
            catch(runtime_error& excpt){

                cout << excpt.what() << endl;
                delete board(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                findHero();
                return false;

            }

            // 6. Hero tries to move on a baddie.
            try{
                if(board(newR, newC)->isBaddie()){
                    throw runtime_error("Hero is trying to move on a baddie cell");
                }
            }
            catch(runtime_error& excpt){

                cout << excpt.what() << endl;
                findHero();
                delete board(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                return false;

            }


            // Execute Move.

            if(newR == HeroRow && newC == HeroCol){
                findHero();
                // Move baddies
                moveBaddies();
                return true;
            }
            else{
                board(HeroRow, HeroCol)->update(newR, newC);
                delete board(newR, newC);
                BoardCell* temp = board(HeroRow, HeroCol);
                board(newR, newC) = temp;
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);

                findHero();
            }

            // Move baddies
            bool baddieGotHero = moveBaddies();
            
            if(baddieGotHero){
                return false;
            }
            else{
                return true;
            }

        }

    
};

#endif //_GAMEBOARD_H
