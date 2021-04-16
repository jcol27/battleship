#pragma once
#include "Player.h"
#include <string>
#include <vector>
#include "Boat.h"
#include "Board.h"

/*
* User class which stores the data and methods for the
* human player. Inherits from Player.
*   
*/
class User :
    public Player
{
private:
    // Variables
    std::string name;
public:
    // Methods
    User();
    Boat createTempBoat(std::string name, int length, int start_row, char start_col, char direction);
    std::string getValidGuess(std::string guess);

    // Getter Methods
    std::string getName();

    // Setter Methods
    void setName(std::string name);

    // Destructor
    virtual ~User();
};

