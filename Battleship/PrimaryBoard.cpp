#include "primaryBoard.h"

PrimaryBoard::PrimaryBoard()
{
}

/*
* Constructor
*
* Inputs:
*	std::vector<Boat> boats_list: the list of placed boats
*/
PrimaryBoard::PrimaryBoard(std::vector<Boat> boats_list)
{
	// Loop over boats
	for (Boat b : boats_list) {
		// Loop over boat pieces
		for (int i = 0; i < b.getLength(); i++) {
			// Update grid
			this->setGridPosVal(b.getRPos()[i], b.getCPos()[i], 2);
		}
	}
}

/*
* Method to print the primary board.
*/
void PrimaryBoard::printBoard()
{
	// Print header
	std::cout << "YOUR BOARD" << std::endl;
	std::cout << "O = your boats, X = hits, + = misses" << std::endl;

	// Print column headings
	std::cout << "    A B C D E F G H I J" << std::endl;
	std::cout << "  -----------------------" << std::endl;

	// Print grid including row numbers
	for (int r = 0; r < 10; r++) {
		// Print row number
		if (r < 9) {
			std::cout << r + 1 << " | ";
		}
		else {
			std::cout << r + 1 << "| ";
		}

		// Print grid value
		for (int c = 0; c < 10; c++) {
			std::cout << evalPrintVal(evalGridPos(r, c)) << " ";
		}
		std::cout << "|" << std::endl;
	}
	std::cout << "  -----------------------" << std::endl;
}

PrimaryBoard::~PrimaryBoard()
{
}
