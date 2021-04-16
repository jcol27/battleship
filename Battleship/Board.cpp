#include "Board.h"
#include "Boat.h"

// Default constructor
Board::Board()
{
}

/*
* Method to update the board grid with a fired shot.
* 
* Inputs:
*	int shot_r: the row position of the shot
*	int shot_c: the col position of the shot
*/
void Board::updateBoard(int shot_r, int shot_c)
{
	// If miss
	if (this->grid[shot_r][shot_c] == 0) {
		this->grid[shot_r][shot_c] = 1;
	}
	// If hit
	else if (this->grid[shot_r][shot_c] == 2) {
		this->grid[shot_r][shot_c] = 3;
	}
}

/*
* Method to evaluate what character should be printed to represent a grid
* position depending on the grid value and whether this is the user or 
* opponent's grid.
* 
* Inputs:
*	int val: the value from the grid representing what is in that position
*	int viewer: 0 if printing users board, 1 if printing the user's
*				perspective of the opponents board
* 
* Outputs:
*	std::string: the character to print for that grid position
*/
std::string Board::evalPrintVal(int val) // TODO: update this to be different for primary and tracking boards (ie remove and add a new method to each subclass)
{
	// 0 = nothing, 1 = missed guess, 2 = boat not hit, 3 = boat hit

	if (val == 0) {
		// Empty squares represented by a space
		return " ";
	}
	else if (val == 1) {
		// Misses represented by a plus
		return "+";
	}
	else if (val == 2) {
		// Not hit boat pieces represented by an uppercase o
		return "O";
	}
	else if (val == 3) {
		// Hit boat pieces represented by an uppercase X
		return "X";
	}
	else {
		// If unexpected grid val throw exception
		throw "Unexpected val as input to evalPrintVal()";
	}
}

/*
* Method to get the value of the grid at row r and column c
*/
int Board::evalGridPos(int r, int c)
{
	return this->grid[r][c];
}

/*
* Method to get the board name
*/
std::string Board::getBoardName()
{
	return this->boardName;
}

/*
* Method to set the board name
*/
void Board::setBoardName(std::string name)
{
	this->boardName = name;
}

/*
* Method to set the value of a position in the grid
*/
void Board::setGridPosVal(int r, int c, int val)
{
	this->grid[r][c] = val;
}

// Destructor
Board::~Board()
{
}
