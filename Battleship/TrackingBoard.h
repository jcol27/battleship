#pragma once
#include "Board.h"
#include <tuple>
#include <vector>

class TrackingBoard :
    public Board
{
private:
    // Variables
    std::vector<std::tuple<int, int>> unidentifiedSquaresList;
    std::vector<std::tuple<int, int>> hitsList;
public:
    // Constructor
    TrackingBoard();

    // Methods
    void printBoard();
    void removeUnidentifiedSquare(std::tuple<int, int> square);
    void addHitToList(std::tuple<int, int> square);

    // Getter Methods
    std::vector<std::tuple<int, int>> getUnidentifiedSquaresList();
    std::vector<std::tuple<int, int>> getHitsList();

    // Destructor
    virtual ~TrackingBoard();
};

