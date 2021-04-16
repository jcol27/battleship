#pragma once
#include <iostream>
#include "Boat.h"

/*
* Board class to represent the board of either player as a 10x10 
* int grid. Represents hits and misses which are then hidden in 
* the print method. 0 = empty, 1 = miss, 2 = boat piece, 3 = hit
* boat piece.
*/
class Board
{
private:
	// Variables
	int grid[10][10] = { 0 };
	std::string boardName;
public:
	// Constructors
	Board();

	// Methods
	void updateBoard(int shot_x, int shot_y);
	virtual void printBoard() = 0;
	std::string evalPrintVal(int val);

	// Getter methods
	std::string getBoardName();

	// Setter Methods
	void setBoardName(std::string name);
	void setGridPosVal(int r, int c, int val);
	int evalGridPos(int x, int y);

	// Destructor
	virtual ~Board();
};

