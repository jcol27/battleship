#pragma once
#include "Player.h"
#include <string>
#include <vector>
#include "Boat.h"
#include "Board.h"
#include <algorithm>
#include <random>

/*
* Ai class which stores the data and methods for the
* computer player. Inherits from Player.
*
*/
class Ai :
    public Player
{
private:
public:
    int random_seed;

    // Checkerboard patterns for the checkerboard guess method
    // Expressed as a vector of vectors of squares as tuples

    // Large checkerboard (A=13, B=13, C=8)
    std::vector<std::vector<std::tuple<int, int>>> large_checkerboard_positions_1 = {
        {std::make_tuple(0,0), std::make_tuple(0,4), std::make_tuple(0,8), std::make_tuple(2,2), std::make_tuple(2,6), 
        std::make_tuple(4,0), std::make_tuple(4,4), std::make_tuple(4,8), std::make_tuple(6,2), std::make_tuple(6,6), 
        std::make_tuple(8,0), std::make_tuple(8,4), std::make_tuple(8,8) }, // A
        {std::make_tuple(1,1), std::make_tuple(1,5), std::make_tuple(1,9), std::make_tuple(3,3), std::make_tuple(3,7), 
        std::make_tuple(5,1), std::make_tuple(5,5), std::make_tuple(5,9), std::make_tuple(7,3), std::make_tuple(7,7), 
        std::make_tuple(9,1), std::make_tuple(9,5), std::make_tuple(9,9) }, // B
        {std::make_tuple(1,3), std::make_tuple(1,7), std::make_tuple(3,1), std::make_tuple(3,5), 
        std::make_tuple(5,3), std::make_tuple(5,7), std::make_tuple(7,1), std::make_tuple(7,5)} // C
    }; // Pattern 1

    std::vector<std::vector<std::tuple<int, int>>> large_checkerboard_positions_2 = {
        {std::make_tuple(0,1), std::make_tuple(0,5), std::make_tuple(0,9), std::make_tuple(2,3), std::make_tuple(2,7),
        std::make_tuple(4,1), std::make_tuple(4,5), std::make_tuple(4,9), std::make_tuple(6,3), std::make_tuple(6,7),
        std::make_tuple(8,1), std::make_tuple(8,5), std::make_tuple(8,9) }, // A
        {std::make_tuple(1,0), std::make_tuple(1,4), std::make_tuple(1,8), std::make_tuple(3,2), std::make_tuple(3,6),
        std::make_tuple(5,0), std::make_tuple(5,4), std::make_tuple(5,8), std::make_tuple(7,2), std::make_tuple(7,6),
        std::make_tuple(9,0), std::make_tuple(9,4), std::make_tuple(9,8) }, // B
        {std::make_tuple(1,2), std::make_tuple(1,6), std::make_tuple(3,4), std::make_tuple(3,8),
        std::make_tuple(5,2), std::make_tuple(5,6), std::make_tuple(7,4), std::make_tuple(7,8)} // C
    }; // Pattern 2
    
    // Small checkerboard (A = 13, B = 12, C = 25)
    std::vector<std::vector<std::tuple<int, int>>> small_checkerboard_positions_1 = {
        {std::make_tuple(0, 0), std::make_tuple(0, 4), std::make_tuple(0, 8), std::make_tuple(2, 2), 
        std::make_tuple(2, 6), std::make_tuple(4, 0), std::make_tuple(4, 4), std::make_tuple(4, 8), 
        std::make_tuple(6, 2), std::make_tuple(6, 6), std::make_tuple(8, 0), std::make_tuple(8, 4), 
        std::make_tuple(8, 8)}, // A
        {std::make_tuple(0, 2), std::make_tuple(0, 6), std::make_tuple(2, 0), std::make_tuple(2, 4), 
        std::make_tuple(2, 8), std::make_tuple(4, 2), std::make_tuple(4, 6), std::make_tuple(6, 0), 
        std::make_tuple(6, 4), std::make_tuple(6, 8), std::make_tuple(8, 2), std::make_tuple(8, 6)}, // B
        {std::make_tuple(1, 1), std::make_tuple(1, 3), std::make_tuple(1, 5), std::make_tuple(1, 7), 
        std::make_tuple(1, 9), std::make_tuple(3, 1), std::make_tuple(3, 3), std::make_tuple(3, 5), 
        std::make_tuple(3, 7), std::make_tuple(3, 9), std::make_tuple(5, 1), std::make_tuple(5, 3), 
        std::make_tuple(5, 5), std::make_tuple(5, 7), std::make_tuple(5, 9), std::make_tuple(7, 1), 
        std::make_tuple(7, 3), std::make_tuple(7, 5), std::make_tuple(7, 7), std::make_tuple(7, 9), 
        std::make_tuple(9, 1), std::make_tuple(9, 3), std::make_tuple(9, 5), std::make_tuple(9, 7), 
        std::make_tuple(9, 9) }  // C
    }; // Pattern 1

    std::vector<std::vector<std::tuple<int, int>>> small_checkerboard_positions_2 = {
        {std::make_tuple(1, 1), std::make_tuple(1, 5), std::make_tuple(1, 9), std::make_tuple(3, 3),
        std::make_tuple(3, 7), std::make_tuple(5, 1), std::make_tuple(5, 5), std::make_tuple(5, 9),
        std::make_tuple(7, 3), std::make_tuple(7, 7), std::make_tuple(9, 1), std::make_tuple(9, 5),
        std::make_tuple(9, 9)}, // A
        {std::make_tuple(1, 3), std::make_tuple(1, 7), std::make_tuple(3, 1), std::make_tuple(3, 5),
        std::make_tuple(3, 9), std::make_tuple(5, 3), std::make_tuple(5, 7), std::make_tuple(7, 1),
        std::make_tuple(7, 5), std::make_tuple(7, 9), std::make_tuple(9, 3), std::make_tuple(9, 7)}, // B
        {std::make_tuple(0, 0), std::make_tuple(0, 2), std::make_tuple(0, 4), std::make_tuple(0, 6),
        std::make_tuple(0, 8), std::make_tuple(2, 0), std::make_tuple(2, 2), std::make_tuple(2, 4),
        std::make_tuple(2, 6), std::make_tuple(2, 8), std::make_tuple(4, 0), std::make_tuple(4, 2),
        std::make_tuple(4, 4), std::make_tuple(4, 6), std::make_tuple(4, 8), std::make_tuple(6, 0),
        std::make_tuple(6, 2), std::make_tuple(6, 4), std::make_tuple(6, 6), std::make_tuple(6, 8),
        std::make_tuple(8, 0), std::make_tuple(8, 2), std::make_tuple(8, 4), std::make_tuple(8, 6),
        std::make_tuple(8, 8) }  // C
    }; // Pattern 2

    int grid[10][10] = { 0 };
    int frequency_grid[10][10] = { 0 };

    // Constructor
    Ai();

    // Methods
    void chooseBoatLocations(int difficulty);
    std::tuple<int, int> guessLocation(int difficulty);
    bool checkValidPlacementED2(Boat b);
    bool checkValidPlacementED1(Boat b);
    std::tuple<int, int> getPossibleBoatLocation(int difficulty);
    bool checkValidPlacementEnumeration(std::vector<Boat> b_list, Boat b, std::vector<std::tuple<int,int>> misses);
    void shuffleCheckerboard();

    // Logic algorithms
    void randomBoatLocations(std::vector<int> lengths, std::vector<std::string> boat_names);
    void ED2BoatLocations(std::vector<int> lengths, std::vector<std::string> boat_names);
    void ED1BoatLocations(std::vector<int> lengths, std::vector<std::string> boat_names);
    std::tuple<int, int> randomGuess();
    std::tuple<int, int> randomNBGuess();
    std::tuple<int, int> cbCompleteEnumGuess();
    std::tuple<int, int> cbMonteCarloSimGuess();
    bool editGrid(int length);
    bool editGrid2(int length, std::vector<std::tuple<int, int, int>> notMiss);
    void resetGrid();

    // Algorithm methods (inactive) see: https://dspace.mit.edu/bitstream/handle/1721.1/100395/Silver_Monte-carlo.pdf?sequence=1&isAllowed=y
    ///int search(std::vector<int> h);
    //int rollout(int* s, std::vector<int> h, int depth);
    //int simulate(int* s, std::vector<int> h, int depth);

    // Destructor
    virtual ~Ai();
};

