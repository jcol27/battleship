#include "Boat.h"

// Constructor
Boat::Boat(int length, int start_r, int start_c, int direction)
{
    this->length = length;

    // Create the positions of the pieces of the boat
    for (int i = 0; i < length; i++) {
        switch (direction) {
        case 0:
            this->setpos(start_r - i, start_c);
            break;
        case 1:
            this->setpos(start_r, start_c + i);
            break;
        case 2:
            this->setpos(start_r + i, start_c);
            break;
        case 3:
            this->setpos(start_r, start_c - i);
            break;
        default:
            throw "Invalid direction";
        }
    }

    // Create the vector of piece statuses for the boat
    // Status = 0 for not hit, 1 for hit
    std::vector<int> status;
    for (int i = 0; i < length; i++) {
        status.push_back(0);
    }
    this->status = status;
}

// Methods

/*
* Method to check if a shot hits a boat and if so then
* update that boat.
* 
* Inputs: 
*   int shot_r: the row of the shot
*   int shot_c: the column of the shot (note: int)
*/
void Boat::update(int shot_r, int shot_c)
{
    // Loop over boat piece positions and check if any match the shot
    for (int i = 0; i < length; i++) {
        if (shot_r == getRPos()[i] && shot_c == getCPos()[i]) {
            this->status[i] = 1;
            break;
        }
    }
}

/*
* Method to check if a boat was hit by a shot.
* 
* Inputs:
*   int shot_r: the row of the shot
*   int shot_c: the column of the shot (note: int)
* 
* Outputs:
*   bool: true if hit, false otherwise
*/
bool Boat::checkHit(int shot_r, int shot_c)
{
    // Loop over boat piece positions and check if any match the shot
    for (int i = 0; i < getLength(); i++) {
        if (shot_r == getRPos()[i] && shot_c == getCPos()[i]) {
            return true;
        }
    }
    return false;
}

/*
* Method to check if a boat is destroyed by taking the sum of statuses
* 
* Outputs:
*   bool: true if destroyed, false otherwise
*/
bool Boat::checkDestroyed()
{
    // Status vector has 0 if fine, 1 if hit, so if sum == length then destroyed
    int sum = 0;
    int count = 0;
    
    for (int i : getStatus()) {
        sum += i;
        count += 1;
    }

    return (sum == count);
}

// Getter Methods

/*
* Method to get the length of the boat
*/
int Boat::getLength()
{
    return this->length;
}

/*
* Method to get the vector of row positions of the boat pieces
*/
std::vector<int> Boat::getRPos()
{
    return this->pos_r;
}

/*
* Method to get the vector of column positions of the boat pieces
*/
std::vector<int> Boat::getCPos()
{
    return this->pos_c;
}

/*
* Method to get the vector of statuses for the boat pieces
*/
std::vector<int> Boat::getStatus()
{
    return this->status;
}

/*
* Method to set the position of a boat piece
* 
* Inputs:
*   int r: the row of the piece
*   int c: the col of the piece (note: int)
*/
void Boat::setpos(int r, int c)
{
    this->pos_r.push_back(r);
    this->pos_c.push_back(c);
    /*
    // Add r to the row vector
    std::vector<int> v = this->pos_r;
    v.push_back(r);
    this->pos_r = v;

    // Add c to the col vector
    v = this->pos_c;
    v.push_back(c);
    this->pos_c = v;
    */
}

/*
* Method to set the boat name e.g. Destroyer
*/
void Boat::setName(std::string name)
{
    this->name = name;
}

// Destructor
Boat::~Boat()
{
}
