#include "User.h"
#include <regex>

// Constructor
User::User()
{

}

// Methods

/*
* Method to create a temporary boat object for the purpose of testing
* its validity. Boat placements are described using a starting square,
* a direction, and a length.
* 
* Inputs:
*	std::string name: the name of the boat e.g. Destroyer
*	int length: the length of the boat
*	int start_row: the row of the starting square 
*	char start_col: the col of the starting square (note char e.g. A or a)
*	char direction: the direction the boat faces (u/d/l/r, u=up etc)
* 
* Outputs:
*	Boat: a boat object with the inputted characteristics
*/
Boat User::createTempBoat(std::string name, int length, int start_row, char start_col, char direction)
{
	// Parse start col
	start_col = std::tolower(start_col);
	start_col = (int)start_col - 97;

	// Parse direction
	int dir = -1;
	switch (direction) {
	case 'u':
		dir = 0;
		break;
	case 'd':
		dir = 2;
		break;
	case 'l':
		dir = 3;
		break;
	case 'r':
		dir = 1;
		break;
	}

	// Create and return boat object
	Boat b = Boat(length, start_row - 1, start_col, dir);
	b.setName(name);
	return b;
}

/*
* Method to check if a user inputted guess is valid (including if it has 
* already been guessed). Uses recursion.
* 
* Inputs:
*	std::string guess: the current guess as a string e.g. A1, b4 etc
* 
* Outputs:
*	std::string: a valid guess as a string e.g. A1, b4 etc.
*/
std::string User::getValidGuess(std::string guess)
{
	// Check that user input is of the correct format, if not retry
	while (!(std::regex_match(guess, std::regex("^[a-jA-J]\\d$")) || std::regex_match(guess, std::regex("^[a-jA-J]\\d0$")))) {
		std::cout << "Invalid guess. Please retry (e.g. A1 - J10):" << std::endl;
		std::cin >> guess;
	}
	bool unique_guess = false;
	std::tuple<int, int> tuple_guess = convertPosStringToTuple(guess);
	for (std::tuple<int, int> square : this->getTrackingBoard().getUnidentifiedSquaresList()) {
		if (std::get<0>(tuple_guess) == std::get<0>(square) && std::get<1>(tuple_guess) == std::get<1>(square)) {
			unique_guess = true;
			break;
		}
	}
	std::string new_guess;
	while (!(unique_guess)) {
		std::cout << "You have already guessed " << guess << ". Please retry." << std::endl;
		std::cin >> new_guess;
		guess = getValidGuess(new_guess);
		break;
	}

	return guess;
}

// Getter Methods

/*
* Method to get the name of the user
*/
std::string User::getName()
{
	return this->name;
}

// Setter Methods

/*
* Method to set the name of the user
*/
void User::setName(std::string name)
{
	this->name = name;
}

// Destructor
User::~User()
{
}