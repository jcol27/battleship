#pragma once
#include <vector>
#include "Boat.h"
#include "Board.h"
#include "PrimaryBoard.h"
#include "TrackingBoard.h"

/*
* Player class from which user and computer classes are derived. 
* Provides methods common to both classes.
*/
class Player
{
private:
public:
	// Variables
	std::vector<Boat> boats;
	PrimaryBoard primaryBoard;
	TrackingBoard trackingBoard;
	int remBoats;

	// Constructor
	Player();

	// Methods
	bool checkValidPlacement(std::vector<Boat> b_list, Boat b);
	void addBoat(Boat b);
	int evalGuess(std::tuple<int,int> guess);
	void createPrimaryBoard(std::vector<Boat> boats_list);
	void updateTrackingBoard(std::tuple<int,int> guess, int result);
	void updatePrimaryBoard(std::tuple<int, int> guess, int result);
	std::string convertPosTupleToString(std::tuple<int, int> pos);
	std::tuple<int, int> convertPosStringToTuple(std::string pos);

	// Getter Methods
	std::vector<Boat> getBoats();
	PrimaryBoard getPrimaryBoard();
	TrackingBoard getTrackingBoard();
	int getRemBoats();

	// Setter Methods
	void setBoats(std::vector<Boat> b_list);
	void setPrimaryBoard(PrimaryBoard primaryBoard);
	void setRemBoats(int remBoats);

	// Destructor
	virtual ~Player();
};

