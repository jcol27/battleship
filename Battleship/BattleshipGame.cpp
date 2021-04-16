#include "BattleshipGame.h"
#include <regex>
#include <algorithm>
#include <vector>
#include <string>
#include <stdio.h>
#include <cctype>
#include <iostream>
#include <tuple>
#include <chrono>
#include <fstream>
#include <iostream>

BattleshipGame::BattleshipGame()
{
	// Game constructor
	//Board player_board(user);
	//Board ai_board(ai);
	//this->player_board = player_board;
	//this->ai_board = ai_board;

}

/*
* Method to convert a direction char to a direction int
* 
* Inputs:
*	char direction: a direction e.g. u/d/l/r
* 
* Outputs:
*	int: the direct represented as an int. 0 = up, 1 = right,
*		2 = down, 3 = left
*/
int convertDirectionToInt(char direction) {
	int dir = -1;

	// Parse direction char
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
	default:
		throw "Unexpected direction char error.";
	}

	return dir;
}

/*
* Method to convert a column char to a column int
* e.g. A or a to 1
* 
* Inputs:
*	char start_col: the column e.g. a,b,c
*
* Outputs:
*	int: the start column as an int
*/
int convertColToInt(char start_col) {
	start_col = tolower(start_col);
	return (int)start_col - 97;
}

/*
* Main function
*/
int main() {
	// Setup
	User user = User();
	Ai ai = Ai();
	user.setRemBoats(5);
	ai.setRemBoats(5);
	bool play = true;
	std::string response;
	int difficulty = 3;

	// Changing this will break certain ai algorithms, random (difficult = 1) should still work though
	std::vector<std::string> boat_names = { "Destroyer", "Submarine", "Cruiser", "Battleship", "Carrier" };
	std::vector<int> boat_counts = { 1,1,1,1,1 };
	std::vector<int> boat_lengths = { 2,3,3,4,5 };

	// Main loop
	while (play) {
		// Game specific setup
		std::cout << "Begin!" << std::endl;
		bool gameover = false;
		bool player_turn = 0 + (rand() % (1 - 0 + 1)) == 1;
		bool won = true;
		std::string guess_str;
		std::string boat_input;

		ai.shuffleCheckerboard();
		ai.resetGrid();

		// Generate user board from user input
		std::cout << "Place your boats (give a starting coordinate e.g. A1 and a direction e.g. u/d/l/r so e.g A1 d" << std::endl;

		// Get boat positions
		for (int i = 0; i < boat_names.size(); i++) {
			std::cout << boat_names[i] << " (length " << boat_lengths[i] << "): " << std::endl;
			getline (std::cin, boat_input);
			std::cout << "You entered: " << boat_input << std::endl;

			// Check that user input is of the correct format, if not retry
			while (!(std::regex_match(boat_input, std::regex("^[a-jA-J]\\d\\s[udlrUDLR]$")) || std::regex_match(boat_input, std::regex("^[a-jA-J]\\d0\\s[udlrUDLR]$")))) {
				std::cout << "Invalid input. Please retry (e.g. A1 d):" << std::endl;
				std::cout << boat_names[i] << " (length " << boat_lengths[i] << "): " << std::endl;
				getline(std::cin, boat_input);
			}

			// Parse user input
			char start_col = tolower(boat_input[0]);
			char direction = boat_input.back();
			boat_input.pop_back(); // Pop direction char
			boat_input.pop_back(); // Pop space
			int start_row = stoi(boat_input.substr(1));

			// Check that user input is a valid boat placement, if not retry
			while (!(user.checkValidPlacement(user.getBoats(), user.createTempBoat(boat_names[i], boat_lengths[i], start_row, start_col, direction)))) {
				std::cout << "Invalid position. Either out of bounds or overlaps existing boat. Please retry" << std::endl;
				std::cout << boat_names[i] << " (length " << boat_lengths[i] << "): " << std::endl;
				getline(std::cin, boat_input);
				start_col = tolower(boat_input[0]);
				direction = boat_input.back();
				boat_input.pop_back(); // Pop direction char
				boat_input.pop_back(); // Pop space
				start_row = stoi(boat_input.substr(1));
			}

			// Create boat as specified and add to user boats list
			int direction_int = convertDirectionToInt(direction);
			int start_col_int = convertColToInt(start_col);
			Boat b = Boat(boat_lengths[i], start_row - 1, start_col_int, direction_int);
			b.setName(boat_names[i]);
			user.addBoat(b);

			// Update and print the user board
			user.createPrimaryBoard(user.getBoats());
			//user.setTrackingBoard();
			user.getPrimaryBoard().printBoard();
		}

		// Generate ai board
		ai.chooseBoatLocations(difficulty);
		ai.createPrimaryBoard(ai.getBoats());

		std::cout << "Boards have been generated. Time to play!" << std::endl;

		// Main game loop
		while (!gameover) {
			// Alternate user and ai turns
			if (player_turn) {
				// Print user boards and get users guess
				std::cout << "Your turn!" << std::endl;
				user.getTrackingBoard().printBoard();
				user.getPrimaryBoard().printBoard();
				std::cout << "Guess a square: (e.g. A1)" << std::endl;
				std::cin >> guess_str;
				
				guess_str = user.getValidGuess(guess_str);

				// Get the outcome of the users guess and present it
				std::string temp = guess_str;
				std::tuple<int, int> guess = user.convertPosStringToTuple(guess_str);
				int res = ai.evalGuess(guess);
				if (res == 0) {
					std::cout << guess_str << ": Miss!" << std::endl;
				}
				else if (res == 1) {
					std::cout << guess_str << ": Hit!" << std::endl;
				}
				else if (res == 2) {
					std::cout << guess_str << ": Hit! Boat destroyed!" << std::endl;
				}
				else {
					throw "Unexpected return from evalUserGuess()";
				}

				// Update user tracking board
				user.updateTrackingBoard(user.convertPosStringToTuple(guess_str), res);	

				// Check if game over
				if (ai.getRemBoats() == 0) {
					gameover = true;
					won = false;
				}
			}
			// Ai turn
			else {
				std::cout << "Ai is thinking..." << std::endl;
				
				// Generate ai guess
				std::tuple<int,int> ai_guess = ai.guessLocation(difficulty);

				// Eval ai guess (user primary board)
				int res = user.evalGuess(ai_guess);
				
				// Update ai tracking board
				ai.updateTrackingBoard(ai_guess, res);

				// Update user primary board
				user.updatePrimaryBoard(ai_guess, res);

				// Print ai guess and outcome
				std::cout << "Opponent guesses ";

				std::string ai_guess_str = ai.convertPosTupleToString(ai_guess);

				if (res == 0) {
					std::cout << ai_guess_str << ": Miss!" << std::endl;
				}
				else if (res == 1) {
					std::cout << ai_guess_str << ": Hit!" << std::endl;
				}
				else if (res == 2) {
					std::cout << ai_guess_str << ": Hit! Boat destroyed!" << std::endl;
				}
				else {
					throw "Unexpected return from evalUserGuess()";
				}

				// Check if game over
				if (user.getRemBoats() == 0) {
					gameover = true;
					won = true;
					break;
				}
			}
			player_turn = !player_turn;
		}

		// End of game
		std::cout << "Game over!.";
		if (won) {
			std::cout << "You won. Play again? (y/n)" << std::endl;
		}
		else {
			std::cout << "You lost. Play again? (y/n)" << std::endl;
		}
		std::cin >> response;
		while (response != "y" && response != "n") {
			std::cout << "Invalid response. Play again? (y/n)" << std::endl;
			std::cin >> response;
		}
		if (response == "y") {
			gameover = false;
		}
		if (response == "n") {
			play = false;
		}
	}

	// End of session
	std::cout << "Thanks for playing!" << std::endl;
}