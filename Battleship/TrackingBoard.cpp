#include "trackingBoard.h"

/*
* Constructor
*/
TrackingBoard::TrackingBoard()
{
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			this->unidentifiedSquaresList.push_back(std::make_tuple(i, j));
		}
	}
}

/*
* Method to print the tracking board.
*/
void TrackingBoard::printBoard()
{
	// Print header
	std::cout << "OPPONENT'S BOARD" << std::endl;
	std::cout << "X = hits, + = misses" << std::endl;

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

/*
* Method to get the list of unidentified squares
*/
std::vector<std::tuple<int, int>> TrackingBoard::getUnidentifiedSquaresList()
{
	return this->unidentifiedSquaresList;
}

/*
* Method to get the list of squares that are hits
*/
std::vector<std::tuple<int, int>> TrackingBoard::getHitsList()
{
	return this->hitsList;
}

/*
* Method to remove a square from the unidentified squares list
* 
* Inputs:
*	std::tuple<int, int> square: the square to be removed
*/
void TrackingBoard::removeUnidentifiedSquare(std::tuple<int, int> square)
{
	int index = 0;
	for (std::tuple<int, int> s : this->getUnidentifiedSquaresList()) {
		if (s == square) {
			break;
		}
		else {
			index++;
		}
	}
	if (index == this->getUnidentifiedSquaresList().size()) {
		std::cout << "Tried to remove guess (" << std::get<0>(square) << "," << std::get<1>(square) << ") but it was not in the unidentified squares list." << std::endl;
		throw "Tried to remove previously removed square from the unidentified squares list";
	}
	this->unidentifiedSquaresList.erase(this->unidentifiedSquaresList.begin() + index, this->unidentifiedSquaresList.begin() + index + 1);
}

/*
* Method to add a square to the hits list
* 
* Inputs:
*	std::tuple<int, int> square: the square to be added
*/
void TrackingBoard::addHitToList(std::tuple<int, int> square)
{
	this->hitsList.push_back(square);
}

/*
* Destructor
*/
TrackingBoard::~TrackingBoard()
{
}