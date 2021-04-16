#pragma once
#include "Board.h"
#include <tuple>
#include <vector>

class PrimaryBoard :
    public Board
{
private:
public:
    // Constructor
    PrimaryBoard();
    PrimaryBoard(std::vector<Boat> boats_list);

    // Methods
    void printBoard();

    // Destructor
    virtual ~PrimaryBoard();
};

