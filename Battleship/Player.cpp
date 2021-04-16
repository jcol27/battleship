#include "Player.h"
#include "Board.h"

// Constructor
Player::Player()
{
    this->remBoats = -1;
}

// Methods

/*
* Method to evaluate a guess, determine whether it is a hit, kill, or miss,
* and return an int representing this outcome. Also updates Player.remBoats.
* 
* Inputs:
*   std::string guess: a string with a guess of the form e.g. A1, B6, C10
* Outputs:
*   int: 0 for miss, 1 for hit, 2 for kill
*/
int Player::evalGuess(std::tuple<int,int> guess)
{
    // Update the player's board with the guess
    this->getPrimaryBoard().updateBoard(std::get<0>(guess), std::get<1>(guess));

    // Check the outcome of the guess and return an int representing it
    for (Boat& b : boats) {
        if (b.checkHit(std::get<0>(guess), std::get<1>(guess))) {
            b.update(std::get<0>(guess), std::get<1>(guess));
            if (b.checkDestroyed()) {
                this->setRemBoats(this->getRemBoats()-1);
                return 2; // Kill
            }
            else {
                return 1; // Hit
            }
        }
    }

    return 0; // Miss
}

/*
* Method to check if a possible boat placement is valid. Checks both if the
* placement is within bounds and does not overlap another previously placed
* boat.
* 
* Inputs:
*   std::vector<Boat> b_list: a vector of the boats already placed
*   Boat b: the boat whose position is to be checked
* Outputs:
*   bool: false if placement invalid, true if valid
*/
bool Player::checkValidPlacement(std::vector<Boat> b_list, Boat b)
{
    // Check boat is within bounds (A-I,1-10)
    for (int i = 0; i < b.getLength(); i++) {
        if (b.getRPos()[i] < 0 || b.getRPos()[i] > 9 || b.getCPos()[i] < 0 || b.getCPos()[i] > 9) {
            return false;
        }
    }

    // Check boat doesn't overlap another previously placed boat
    for (Boat& x : b_list) {
        // Loop through pieces of each boat
        for (int i = 0; i < x.getLength(); i++) {
            for (int j = 0; j < b.getLength(); j++) {
                if (x.getRPos()[i] == b.getRPos()[j] && x.getCPos()[i] == b.getCPos()[j]) {
                    return false;
                }
            }
        }
    }
    return true;
}

/*
* Method to generate and set the player's primary board
* Input:
*   std::vector<Boat> boats_list: a list of the player's placed boats
*/
void Player::createPrimaryBoard(std::vector<Boat> boats_list)
{
    this->primaryBoard = PrimaryBoard(boats_list);
}

/*
* Method to update the tracking board.
* 
* Input:
*   std::tuple<int, int> guess: the guess as a (row, col) tuple 0-9.
*   int result: the result of the guess. 1 = miss, 2 = hit, 3 = kill
*/
void Player::updateTrackingBoard(std::tuple<int,int> guess, int result)
{
    int r = std::get<0>(guess);
    int c = std::get<1>(guess);

    // Update the tracking grid (use 2 for hit and kill)
    this->trackingBoard.setGridPosVal(r, c, (result == 2 || result == 1) ? 3 : 1);
    
    // Update unidentified squares list
    this->trackingBoard.removeUnidentifiedSquare(std::make_tuple(r, c));
    
    // Update hit list
    if (result == 1 || result == 2) {
        this->trackingBoard.addHitToList(std::make_tuple(r, c));
    }
}

/*
* Method to update the primary board.
* 
* Input:
*   std::tuple<int, int> guess: the guess as a (row, col) tuple 0-9.
*   int result: the result of the guess. 1 = miss, 2 = hit, 3 = kill
*/
void Player::updatePrimaryBoard(std::tuple<int, int> guess, int result)
{
    int r = std::get<0>(guess);
    int c = std::get<1>(guess);

    // Update the tracking grid (use 3 for hit and kill)
    // This improves RNB guess performance by stopping NB guesses after a boat kill // TODO: wrong 2
    this->primaryBoard.setGridPosVal(r, c, (result == 2 || result == 1) ? 3 : 1);
}

/*
* Method to convert a grid position from a tuple e.g. (0,2) to
* a string e.g. "A3"
*
* Inputs:
*	std::tuple<int,int> pos: the position as a tuple
*
* Outputs:
*	std::string: the position as a alphanumeric string
*/
std::string Player::convertPosTupleToString(std::tuple<int, int> pos) {

    int row = std::get<0>(pos);
    int col = std::get<1>(pos);

    std::string s = "";
    char c = static_cast<char>(col) + 'a';
    if (row == 9) {
        std::string r = "10";
        s.push_back(c);
        s.append(r);
    }
    else {
        char r = static_cast<char>(row) + '1';
        s.push_back(c);
        s.push_back(r);
    }

    return s;
}

/*
* Method to convert a grid position from a string e.g. "A3" to
* a tuple e.g. (0,2)
*
* Inputs:
*	std::string: the position as a alphanumeric string
*
* Outputs:
*	std::tuple<int,int> pos: the position as a tuple
*/
std::tuple<int, int> Player::convertPosStringToTuple(std::string pos) {
    char col = tolower(pos[0]);
    int r = stoi(pos.substr(1)) - 1;
    int c = col - 'a';
    return std::make_tuple(r, c);
}

// Getter Methods

/*
* Method to get the list of boats the player has placed
*/
std::vector<Boat> Player::getBoats()
{
    return this->boats;
}

/*
* Method to get the players board (including hit and miss information)
*/
PrimaryBoard Player::getPrimaryBoard()
{
    return this->primaryBoard;
}

TrackingBoard Player::getTrackingBoard()
{
    return this->trackingBoard;
}

/*
* Method to get the number of remaining (not destroyed) boats
*/
int Player::getRemBoats()
{
    return this->remBoats;
}

// Setter Methods

/*
* Method to set the list of boats the player has placed
*/
void Player::setBoats(std::vector<Boat> b_list)
{
    this->boats = b_list;
}

/*
* Method to set the number of remaining (not destroyed) boats
*/
void Player::setRemBoats(int remBoats)
{
    this->remBoats = remBoats;
}

/*
* Method to set the player's board
* Input:
*   Board board: a board object representing the player's board
*/
void Player::setPrimaryBoard(PrimaryBoard primaryBoard)
{
    this->primaryBoard = primaryBoard;
}

/*
* Method to add a boat to the player's boats list
*/
void Player::addBoat(Boat b)
{
    this->boats.push_back(b);
}

// Destructor
Player::~Player()
{
}
