#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include "sudoku.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own
   function definitions to the end of this file. */

/* pre-supplied function to load a Sudoku board from a file */
void load_board(const char* filename, char board[9][9]) {
  cout << "Loading Sudoku board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in) {
    cout << "Failed!\n";
  }
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9) {
    for (int n=0; n<9; n++) {
      assert(buffer[n] == '.' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer,512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << '\n';
  assert(row == 9);
}

/* internal helper function */
void print_frame(int row) {
  if (!(row % 3)) {
    cout << "  +===========+===========+===========+\n";
  } else {
    cout << "  +---+---+---+---+---+---+---+---+---+\n";
  }
}

/* internal helper function */
void print_row(const char* data, int row) {
  cout << (char) ('A' + row) << " ";
  for (int i=0; i<9; i++) {
    cout << ( (i % 3) ? ':' : '|' ) << " ";
    cout << ( (data[i]=='.') ? ' ' : data[i]) << " ";
  }
  cout << "|\n";
}

/* pre-supplied function to display a Sudoku board */
void display_board(const char board[9][9]) {
  cout << "    ";
  for (int r=0; r<9; r++) {
    cout << (char) ('1'+r) << "   ";
  }
  cout << '\n';
  for (int r=0; r<9; r++) {
    print_frame(r);
    print_row(board[r],r);
  }
  print_frame(9);
}

/* add your functions here */


// this function checks if sudoku board is complete
bool is_complete(const char board[9][9]) {

	// nested for loop that goes through each element in the matrix
	//  and checks if the element is '.' if so returns false
	for (int row = 0; row < 9; row++) {
		for(int column = 0; column < 9; column++) {
			if (board[row][column] < '1' || board[row][column] > '9')
				return false;
		}
	}
	// if no '.' were found true is return as the board is complete
	return true;
}

// This function attempts to place a digit at a certain position if it is a valid move
bool make_move(const char position[3], char digit, char board[9][9]) {

	int row_index = (position[0]- 'A'); // Converting the letter in the position string to an integer from 0 to 8 (the row index)
	int column_index = (position[1] - '1'); // converting the number in the position string to  an integer from 0 to 8 (the column index)

	// checks if the digit is a valid value between 1 and 9
	if(digit < '1' || digit > '9' )
		return false;

	// check if the row and column index provided are out of bounds
	if(row_index < 0 || row_index > 8 || column_index < 0 || column_index > 8 )
		return false;

	// check if the desired position is empty
	if(board[row_index][column_index] != '.')
		return false;

	// check if the digit is already present within the row
	for(int column =0; column<9; column++ ) {
		if (board[row_index][column] == digit){
			return false;
		}
	}

	// check if the digit is already present within the column
	for (int row=0; row<9; row++) {
		if(board[row][column_index] ==  digit)
			return false;
	}

	// check if the digit is already present within the current 3x3 block

	int i = row_index/3 + 1, j = column_index/3 + 1; // finding out which block the digit is in

	// checking all the cells within the block
	for (int row = i*3-3; row < i*3; row++) {
		for(int column=j*3-3; column < j*3; column++) {
			if(board[row][column] == digit) {
				return false;
			}
		}
	}

	// if all checks are passed update board

	board[row_index][column_index] = digit;

	return true;

}

// this function saves the board to a certain file
bool save_board(const char* filename, char  board[9][9]) {

	// opening file
	ofstream outFile;
	outFile.open(filename);

	if (!outFile)
		return false;

	// iterating through each element in board and writing it to the file
	for (int row = 0; row < 9; row++) {
		for(int column = 0; column < 9; column++) {
			outFile << board[row][column];
			if (outFile.fail()) // making sure the write operation was successful
				return false;
		}
		outFile << '\n';
	}
	outFile.close();
	return true;
}


// This recursive function attempts to solve the sudoku board
bool solve_board(char board[9][9]){

	static char position[] = {'A', '0'}; // keeps track of which cell the function is working on across function calls
	char digit = '1';
	static bool finished = false; // flag is used to tell that a conclusion on the board has been reached

	// row and column are used to keep track of the last cell that was changed (for backtracking)
	char row = position[0];
	char column = position[1];


	static int count = 0; // used to track the number of function calls
	count++;

	// if the finished flag is set the position is reset to point to the start position
	// this ensures that when the function is re-used to solve another board, the position points to the start of the board
	if(finished) {
		finished = false;
		position[0] = 'A';
		position[1] = '0';
		count = 0;
	}

	// base case: if the board is completely filled it means the board was solved therefore returning true
	if(is_complete(board)){
		save_board("something123.dat", board);
		finished = true;
		cout << "The number of function calls was: " << count << endl; // printing the number of function calls
		return true;
	}

	// this while loop increments the position by going to the next cell until an empty cell is found
	while(board[(int)(position[0] - 'A')][(int)(position[1] - '1')] != '.'){
		if(position[1] >= '9'){
			position[0] = (char)(position[0] + 1);
			position[1]	= '1';
		}
		else {
			position[1] =  (char)(position[1] + 1);
		}
	}

	// once the an empty cell is found an attemp to place a digit, starting from 1, will be made using the make_move
	while(digit <= '9'){
		// if it is valid to place that digit at the curent position a recursive call will be made to solve the rest of the board
		if(make_move(position, digit, board)) {
			// if the call returns true it means the board was solved, therefore keep returning true
			if (solve_board(board)){
				return true;
			}
			// otherwise delete the digit that was placed at the current position and re-try to solve the board with the next digit
			else{
				board[(int)(position[0] - 'A')][(int)(position[1] - '1')] = '.';
			}
		}
		digit = (char)(digit + 1);
	}
	// if all digits were tried and no solution was found return to the previous recursive call and try the remaining digits for the previous cell position

	position[0] = row;
	position[1] = column;

	// checks if the starting position is reached again, and at this point all combinations of digits have been attempted therefore no solution is possible
	// for this board thus, setting the finished flag
	if(position[0] == 'A' && position[1] == '1') {
		finished = true;
		count = 0;
	}
	return false;
}
