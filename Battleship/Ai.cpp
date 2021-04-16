#include "Ai.h"
#include <tuple>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <math.h>

// Constructor
Ai::Ai()
{
    srand(static_cast<unsigned int>(time(NULL)));
    this->random_seed = rand() % 2;
}

/*
* Method for the ai to choose which positions to place its boats in. 
* Multiple algorithms depending on difficulty.
* Uses randomness to prevent easy antistratting by the human player
* over multiple games.
* 
* Difficulty = 0:
*   Random placement
* Difficulty = 1:
*   75% chance edge placement with distance from edge = 2, 25% chance random
* Difficulty = 2:
*   75% chance edge placement with distance from edge = 1, 25% chance random
* 
* Inputs:
*   int difficulty: variable to indicate the ai strategy
* 
* See:
*   Sakuta, Makoto, and Hiroyuki Iida. "Evaluation of attacking 
*   and placing strategies in the battleship game without considering 
*   opponent models." Proceedings of 1st International Forum on 
*   Information and Computer Technology. 2003.
*
*/
void Ai::chooseBoatLocations(int difficulty)
{
    // Initialization
    std::vector<int> lengths = { 2, 3, 3, 4, 5 };
    std::vector<std::string> boat_names = { "Destroyer", "Submarine", "Cruiser", "Battleship", "Carrier" };
    srand(static_cast<unsigned int>(time(NULL)));
    int prob = rand() % 100 + 1; // 1-100 prob variable

    // Switch based on difficulty
    switch (difficulty) {
    case 0:
        randomBoatLocations(lengths, boat_names);
        break;
    case 1:
        // 75% change of ed2
        if (prob > 25) {
            ED2BoatLocations(lengths, boat_names);
        }
        else {
            randomBoatLocations(lengths, boat_names);
        }
        break;
    case 2:
    case 3:
        // 75% change of ed1
        if (prob > 25) {
            ED1BoatLocations(lengths, boat_names);
        }
        else {
            randomBoatLocations(lengths, boat_names);
        }
        break;
    default:
        throw  "Unexpected difficulty as input to chooseBoatLocations()";
    }
}

/*
* Method to guess the location of an enemy boat piece
* 
* Inputs:
*   int difficulty: variable to indicate the ai strategy
* 
* Outputs:
*   std::tuple<int, int>: a tuple containing the row and column of the shot
*/
std::tuple<int, int> Ai::guessLocation(int difficulty)
{

    switch (difficulty) {
    case 0:
        return randomGuess();
    case 1:
        return randomNBGuess();
    case 2:
        //return cbCompleteEnumGuess();
        return randomNBGuess();
    case 3: 
        //return cbMonteCarloSimGuess();
        return randomNBGuess();
    default:
        throw  "Unexpected difficulty as input to guessLocation()";
    }
}

/*
* Method to check if a boat placement is valid according to the ED2 strategy. Note:
* boat overlap is checked separately.
* 
* Inputs:
*   Boat b: the boat to check
* 
* Outputs:
*   bool: true if valid, false otherwise
*/
bool Ai::checkValidPlacementED2(Boat b)
{
    // Check boat is within ED2 bounds
    for (int i = 0; i < b.getLength(); i++) {
        if ((b.getRPos()[i] >= 2 && b.getRPos()[i] <= 7) && (b.getCPos()[i] >= 2 && b.getCPos()[i] <= 7 )) {
            return false;
        }
    }
    return true;
}

/*
* Method to check if a boat placement is valid according to the ED1 strategy. Note:
* boat overlap is checked separately
* 
* Inputs:
*   Boat b: the boat to check
*
* Outputs:
*   bool: true if valid, false otherwise
*/
bool Ai::checkValidPlacementED1(Boat b)
{
    // Check boat is within ED1 bounds
    for (int i = 0; i < b.getLength(); i++) {
        if ((b.getRPos()[i] >= 1 && b.getRPos()[i] <= 8) && (b.getCPos()[i] >= 1 && b.getCPos()[i] <= 8)) {
            return false;
        }
    }
    return true;
}

/*
* Method to get a possible boat start location based on the 
* strategy. NOTE: may generate a location that is not 
* actually possible, but saves time by eliminating a lot of definitely
* impossible start locations.
* 
* Inputs:
*   int difficulty: variable to indicate the ai strategy
* 
* Outputs:
*   std::tuple<int, int>: a square expressed as a (row, col) tuple
*/
std::tuple<int, int> Ai::getPossibleBoatLocation(int difficulty)
{
    int start_r;
    int start_c;
    switch (difficulty) {
    case 0:
        // Standard random location generation
        start_r = rand() % 10;
        start_c = rand() % 10;
        break;
    case 1:
        // Location generation based on ED2
        start_r = rand() % 10;
        //*start_c;
        if (start_r >= 2 || start_r <= 7) {
            if (rand() % 2 == 0) {
                start_c = rand() % 2;
            }
            else {
                start_c = rand() % 2 + 8;
            }
        }
        else {
            start_c = rand() % 10;
        }
        break;
    case 2:
        // Location generation based on ED1
        start_r = rand() % 10;
        //*start_c;
        if (start_r >= 1 || start_r <= 8) {
            if (rand() % 2 == 0) {
                start_c = 0;
            }
            else {
                start_c = 9;
            }
        }
        else {
            start_c = rand() % 10;
        }
        break;
    default:
        throw  "Unexpected difficulty as input to getPossibleBoatLocation()";
    }
    return std::make_tuple(start_r, start_c);
}

/*
* Method to check if a boat placement is valid. This method is specifically for the
* enumeration algorithm as it checks confirmed miss squares.
* 
* Inputs:
*   std::vector<Boat> b_list: the list of boats already placed
*   Boat b: the boat whose placement is being checked
*   std::vector<std::tuple<int, int> misses: the vector of confirmed miss squares
* 
* Outputs:
*   bool: true if valid, false otherwise
*/
bool Ai::checkValidPlacementEnumeration(std::vector<Boat> b_list, Boat b, std::vector<std::tuple<int,int>> misses)
{
    // Check boat is within bounds (A-I,1-10)
    for (int i = 0; i < b.getLength(); i++) {
        if (b.getRPos()[i] < 0 || b.getRPos()[i] > 9 || b.getCPos()[i] < 0 || b.getCPos()[i] > 9) {
            return false;
        }
    }

    // Check boat doesn't overlap a confirmed miss square
    for (std::tuple<int, int> square : misses) {
        // Loop through pieces of each boat
        for (int i = 0; i < b.getLength(); i++) {
            if (b.getRPos()[i] == std::get<0>(square) && b.getCPos()[i] == std::get<1>(square)) {
                return false;
            }
        }
    }

    // Check boat doesn't overlap another previously placed boat
    for (Boat x : b_list) {
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
* Method to shuffle the subvectors of the checkerboard positions lists. This
* adds randomness to the order that the squares are checked to prevent identical
* play in repeated games.
*/
void Ai::shuffleCheckerboard()
{
    srand(unsigned(time(NULL)));

    // Shuffle positions within subvectors
    for (int i = 0; i < 3; i++) {
        std::random_shuffle(std::begin(small_checkerboard_positions_1[i]), std::end(small_checkerboard_positions_1[i]));// , e);
        std::random_shuffle(std::begin(small_checkerboard_positions_2[i]), std::end(small_checkerboard_positions_2[i]));// , e);
        std::random_shuffle(std::begin(large_checkerboard_positions_1[i]), std::end(large_checkerboard_positions_1[i]));// , e);
        std::random_shuffle(std::begin(large_checkerboard_positions_2[i]), std::end(large_checkerboard_positions_2[i]));// , e);
    }
}

/*
* Method to place the Ai's boats randomly at the beginning of the game.
* Called by Ai::chooseBoatLocations(int difficulty);
* 
* Inputs:
*   std::vector<int> lengths: the vector of the lengths of the boats in order
*   std::vector<std::string> boat_names: the vector of the names of the boats in order
*/
void Ai::randomBoatLocations(std::vector<int> lengths, std::vector<std::string> boat_names)
{
    std::vector<Boat> b_list;
    // Random placement
    for (int i = static_cast<int>(lengths.size()) - 1; i >= 0; i--) {
        int length = lengths[i];
        int direction = rand() % 4;
        int start_r = std::get<0>(getPossibleBoatLocation(0));
        int start_c = std::get<1>(getPossibleBoatLocation(0));

        // Create the boat
        Boat b = Boat(length, start_r, start_c, direction);
        b.setName(boat_names[i]);

        // Loop until valid position obtained
        while (!checkValidPlacement(b_list, b)) {
            length = lengths[i];
            direction = rand() % 4;
            start_r = std::get<0>(getPossibleBoatLocation(0));
            start_c = std::get<1>(getPossibleBoatLocation(0));

            b = Boat(length, start_r, start_c, direction);
            b.setName(boat_names[i]);
        }

        // Add the boat to the ai boats list
        this->addBoat(b);
        b_list.push_back(b);
    }
}

/*
* Method to place the Ai's boats at the beginning of the game according to the ED2 strategy.
* Called by Ai::chooseBoatLocations(int difficulty);
*
* Inputs:
*   std::vector<int> lengths: the vector of the lengths of the boats in order
*   std::vector<std::string> boat_names: the vector of the names of the boats in order
*/
void Ai::ED2BoatLocations(std::vector<int> lengths, std::vector<std::string> boat_names)
{
    // Edge placement with dist = 2
    std::vector<Boat> b_list;
    for (int i = static_cast<int>(lengths.size()) - 1; i >= 0; i--) {
        int length = lengths[i];
        int direction = rand() % 4;
        int start_r = std::get<0>(getPossibleBoatLocation(1));
        int start_c = std::get<1>(getPossibleBoatLocation(1));

        // Create the boat
        Boat b = Boat(length, start_r, start_c, direction);
        b.setName(boat_names[i]);

        // Loop until valid position obtained
        while (!(checkValidPlacement(b_list, b) && checkValidPlacementED2(b))) {
            length = lengths[i];
            direction = rand() % 4;
            start_r = std::get<0>(getPossibleBoatLocation(1));
            start_c = std::get<1>(getPossibleBoatLocation(1));

            b = Boat(length, start_r, start_c, direction);
            b.setName(boat_names[i]);
        }

        // Add the boat to the ai boats list
        this->addBoat(b);
        b_list.push_back(b);
    }
}

/*
* Method to place the Ai's boats at the beginning of the game according to the ED1 strategy.
* Called by Ai::chooseBoatLocations(int difficulty);
*
* Inputs:
*   std::vector<int> lengths: the vector of the lengths of the boats in order
*   std::vector<std::string> boat_names: the vector of the names of the boats in order
*/
void Ai::ED1BoatLocations(std::vector<int> lengths, std::vector<std::string> boat_names)
{
    // Edge placement with dist = 1
    std::vector<Boat> b_list;
    for (int i = static_cast<int>(lengths.size()) - 1; i >= 0; i--) {
        int length = lengths[i];
        int direction = rand() % 4;
        int start_r = std::get<0>(getPossibleBoatLocation(2));
        int start_c = std::get<1>(getPossibleBoatLocation(2));

        // Create the boat
        Boat b = Boat(length, start_r, start_c, direction);
        b.setName(boat_names[i]);

        // Loop until valid position obtained
        while (!(checkValidPlacement(b_list, b) && checkValidPlacementED1(b))) {
            length = lengths[i];
            direction = rand() % 4;
            start_r = std::get<0>(getPossibleBoatLocation(2));
            start_c = std::get<1>(getPossibleBoatLocation(2));

            b = Boat(length, start_r, start_c, direction);
            b.setName(boat_names[i]);
        }

        // Add the boat to the ai boats list
        this->addBoat(b);
        b_list.push_back(b);
    }
}

/*
* Method to generate a random guess. Used by Ai::guessLocation
* 
* Outputs:
*   std::tuple<int, int>: the guess as a (row, col) tuple
*/
std::tuple<int, int> Ai::randomGuess()
{
    std::tuple<int, int> guess; // (row, col)
    std::vector<std::tuple<int, int>> unknowns;
    std::vector<std::tuple<int, int>> hits;
    int index;
    int gotGuess = false;

    // Choose random squre from the unidentified squares list
    // Note: the square will be removed later
    unknowns = getTrackingBoard().getUnidentifiedSquaresList();
    index = rand() % static_cast<int>(unknowns.size());
    guess = unknowns[index];

    return guess;
}

/*
* Method to generate a random guess, or if there are unresolved hits,
* then to generate a neighbouring guess. Used by Ai::guessLocation.
*
* Outputs:
*   std::tuple<int, int>: the guess as a (row, col) tuple
*/
std::tuple<int, int> Ai::randomNBGuess()
{
    std::tuple<int, int> guess; // (row, col)
    std::vector<std::tuple<int, int>> unknowns;
    std::vector<std::tuple<int, int>> hits;
    int gotGuess = false;

    // Random with neighbouring (RNB)
    for (std::tuple<int, int> hit : getTrackingBoard().getHitsList()) {
        // Create vector of positions adjacent to hit
        std::vector<std::tuple<int, int>> adjacent = {
            std::make_tuple(std::get<0>(hit) - 1, std::get<1>(hit)),
            std::make_tuple(std::get<0>(hit), std::get<1>(hit) - 1),
            std::make_tuple(std::get<0>(hit) + 1, std::get<1>(hit)),
            std::make_tuple(std::get<0>(hit), std::get<1>(hit) + 1),
        };

        for (std::tuple<int, int> pos : adjacent) {
            // Check valid pos
            if (std::get<0>(pos) >= 0 && std::get<0>(pos) <= 9 && std::get<1>(pos) >= 0 && std::get<1>(pos) <= 9) {
                for (std::tuple<int, int> square : getTrackingBoard().getUnidentifiedSquaresList()) {
                    if (square == pos) {
                        guess = pos;
                        gotGuess = true;
                        break;
                    }
                }

                // Break since a guess has been found
                if (gotGuess) {
                    break;
                }
            }
        }

        // Break since a guess has been found
        if (gotGuess) {
            break;
        }
    }

    // If no neighbouring square found, choose a random square
    if (!gotGuess) {
        // Recursion
        guess = guessLocation(0);
    }

    return guess;
}

/*
* Method to generate a guess via checkerboarding initially, then 
* by complete enumeration after a certain number of guesses.
* Used by Ai::guessLocation. Generally not very efficient, takes
* around 5s per move. A purely grid based method would no doubt be 
* faster, but this is more of a proof of concept. In practical use, 
* prefer diff=3, which has a good mix of both speed and accuracy.
*
* Outputs:
*   std::tuple<int, int>: the guess as a (row, col) tuple
*/
std::tuple<int, int> Ai::cbCompleteEnumGuess()
{
    std::tuple<int, int> guess; // (row, col)
    std::vector<std::tuple<int, int>> unknowns;
    std::vector<std::tuple<int, int>> hits;
    int unknowns_size;
    int gotGuess = false;
    int mp_x_2 = 50;

    unknowns = getTrackingBoard().getUnidentifiedSquaresList();
    hits = getTrackingBoard().getHitsList();
    unknowns_size = static_cast<int>(unknowns.size());

    // Checkerboarding for unknowns > mp_x
    if (unknowns_size > mp_x_2) {
        int checker_index = 100 - unknowns_size;
        if (random_seed == 0) {
            if (checker_index < 13) {
                guess = small_checkerboard_positions_1[0][checker_index];
            }
            else if (checker_index < 25) {
                guess = small_checkerboard_positions_1[1][checker_index - static_cast <long long>(13)]; // Cast to please VS
            }
            else {
                guess = small_checkerboard_positions_1[2][checker_index - static_cast <long long>(25)];
            }
        }
        else {
            if (checker_index < 13) {
                guess = small_checkerboard_positions_2[0][checker_index];
            }
            else if (checker_index < 25) {
                guess = small_checkerboard_positions_2[1][checker_index - static_cast <long long>(13)]; // Cast to please VS
            }
            else {
                guess = small_checkerboard_positions_2[2][checker_index - static_cast <long long>(25)];
            }
        }
    }

    // Minimum possible squares (enumeration probability algorithm)
    else {
        int frequency_grid[10][10] = { 0 };
        std::vector<Boat> boats_list;
        int iters = 0;

        // Create vector of hits + unknowns since both are possible boat start points
        std::vector<std::tuple<int, int>> notMiss;
        notMiss.reserve(unknowns.size() + hits.size());
        notMiss.insert(notMiss.end(), unknowns.begin(), unknowns.end());
        notMiss.insert(notMiss.end(), hits.begin(), hits.end());

        // Create vector of misses
        std::vector<std::tuple<int, int>> misses;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                misses.push_back(std::make_tuple(i, j));
            }
        }
        for (std::tuple<int, int> s : notMiss) {
            std::vector<std::tuple<int, int>>::iterator position = std::find(misses.begin(), misses.end(), s);
            misses.erase(position);
        }

        // Boat 1 (carrier, l = 5)
        for (std::tuple<int, int> pos1 : notMiss) {
            boats_list = {};
            // Direction of boat 1 down or right (since up and left are duplicates)
            for (int dir1 = 1; dir1 < 3; dir1++) {
                // If position is valid
                Boat b1 = Boat(5, std::get<0>(pos1), std::get<1>(pos1), dir1);
                if (checkValidPlacementEnumeration(boats_list, b1, misses)) {
                    boats_list.push_back(b1);
                    // Boat 2 (battleship, l = 4)
                    for (std::tuple<int, int> pos2 : notMiss) {
                        for (int dir2 = 1; dir2 < 3; dir2++) {
                            Boat b2 = Boat(4, std::get<0>(pos2), std::get<1>(pos2), dir2);
                            if (checkValidPlacementEnumeration(boats_list, b2, misses)) {
                                boats_list.push_back(b2);
                                // Boat 3 (cruiser, l = 3)
                                for (std::tuple<int, int> pos3 : notMiss) {
                                    for (int dir3 = 1; dir3 < 3; dir3++) {
                                        Boat b3 = Boat(3, std::get<0>(pos3), std::get<1>(pos3), dir2);
                                        if (checkValidPlacementEnumeration(boats_list, b3, misses)) {
                                            boats_list.push_back(b3);
                                            // Boat 4 (submarine, l = 3)
                                            for (std::tuple<int, int> pos4 : notMiss) {
                                                for (int dir4 = 1; dir4 < 3; dir4++) {
                                                    Boat b4 = Boat(3, std::get<0>(pos4), std::get<1>(pos4), dir4);
                                                    if (checkValidPlacementEnumeration(boats_list, b4, misses)) {
                                                        boats_list.push_back(b4);
                                                        // Boat 5 (Destroyer, l = 2)
                                                        for (std::tuple<int, int> pos5 : notMiss) {
                                                            for (int dir5 = 1; dir5 < 3; dir5++) {
                                                                Boat b5 = Boat(2, std::get<0>(pos5), std::get<1>(pos5), dir5);
                                                                if (checkValidPlacementEnumeration(boats_list, b5, misses)) {
                                                                    boats_list.push_back(b5);
                                                                    iters++;
                                                                    // Valid configuration, update frequency grid
                                                                    for (Boat b : boats_list) {
                                                                        for (int i = 0; i < b.getLength(); i++) {
                                                                            frequency_grid[b.getRPos()[i]][b.getCPos()[i]] += 1;
                                                                        }
                                                                    }
                                                                    boats_list.pop_back();

                                                                }
                                                            }
                                                        }
                                                        boats_list.pop_back();
                                                    }
                                                }
                                            }
                                            boats_list.pop_back();
                                        }
                                    }
                                }
                                boats_list.pop_back();
                            }
                        }
                    }
                    boats_list.pop_back();
                }
            }
        }

        // Remove confirmed hits from frequency grid
        for (std::tuple<int, int> h : hits) {
            frequency_grid[std::get<0>(h)][std::get<1>(h)] = 0;
        }

        // Find highest probability square
        int best_r = -1;
        int best_c = -1;
        int best_freq = 0;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (frequency_grid[i][j] > best_freq) {
                    best_r = i;
                    best_c = j;
                    best_freq = frequency_grid[i][j];
                }
            }
        }
        guess = std::make_tuple(best_r, best_c);
    }

    return guess;
}

/*
* Method to generate a guess via checkerboarding initially, then
* by monte carlo enumeration after a certain number of guesses.
* Used by Ai::guessLocation. Good mix of performance and speed
* given the performance penalty of object orientation. Performance
* will improve as game progresses due to the fixed move time. Default
* movetime is 2 seconds.
*
* Outputs:
*   std::tuple<int, int>: the guess as a (row, col) tuple
*/
std::tuple<int, int> Ai::cbMonteCarloSimGuess()
{
    std::tuple<int, int> guess; // (row, col)
    std::vector<std::tuple<int, int>> unknowns;
    std::vector<std::tuple<int, int>> hits;
    int unknowns_size;
    int gotGuess = false;
    int mp_x_3 = 66;
    double searchtime = 3.0; //seconds

    // MPx with time constrained Monte Carlo simulation 
    unknowns = getTrackingBoard().getUnidentifiedSquaresList();
    hits = getTrackingBoard().getHitsList();
    unknowns_size = static_cast<int>(unknowns.size());

    auto start = std::chrono::steady_clock::now();
    bool timeout = false;

    std::vector<Boat> boats_list;

    // Create vector of hits + unknowns since both are possible boat start points
    std::vector<std::tuple<int, int>> notMiss;
    notMiss.reserve(unknowns.size() + hits.size());
    notMiss.insert(notMiss.end(), unknowns.begin(), unknowns.end());
    notMiss.insert(notMiss.end(), hits.begin(), hits.end());

    // Create vector of misses
    std::vector<std::tuple<int, int>> misses;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            misses.push_back(std::make_tuple(i, j));
        }
    }
    for (std::tuple<int, int> s : notMiss) {
        std::vector<std::tuple<int, int>>::iterator position = std::find(misses.begin(), misses.end(), s);
        misses.erase(position);
    }

    // Create notmiss for each boat based on possible positions with dir = 0 or 1 (ie. row, col, dir tuple)
    // dir = 0 is right, dir = 1 is down
    std::vector<std::tuple<int, int, int>> notMiss2 = {};
    std::vector<std::tuple<int, int, int>> notMiss3 = {};
    std::vector<std::tuple<int, int, int>> notMiss4 = {};
    std::vector<std::tuple<int, int, int>> notMiss5 = {};

    for (std::tuple<int, int> s : notMiss) {
        int r = std::get<0>(s);
        int c = std::get<1>(s);
        
        // Boat l=2
        if (r < 9 && c < 9) {
            notMiss2.push_back(std::make_tuple(r, c, 0));
            notMiss2.push_back(std::make_tuple(r, c, 1));
        }
        else if (r == 9 && c != 9) {
            notMiss2.push_back(std::make_tuple(r, c, 0));
        }
        else if (c == 9 && r != 9) {
            notMiss2.push_back(std::make_tuple(r, c, 1));
        }

        // Boat l = 3
        if (r < 8 && c < 8) {
            notMiss3.push_back(std::make_tuple(r, c, 0));
            notMiss3.push_back(std::make_tuple(r, c, 1));
        }
        else if (r >= 8 && c <= 7) {
            notMiss3.push_back(std::make_tuple(r, c, 0));
        }
        else if (c >=8 && r <= 7) {
            notMiss3.push_back(std::make_tuple(r, c, 1));
        }

        // Boat l = 4
        if (r < 7 && c < 7) {
            notMiss4.push_back(std::make_tuple(r, c, 0));
            notMiss4.push_back(std::make_tuple(r, c, 1));
        }
        else if (r >= 7 && c <= 6) {
            notMiss4.push_back(std::make_tuple(r, c, 0));
        }
        else if (c >= 7 && r <= 6) {
            notMiss4.push_back(std::make_tuple(r, c, 1));
        }

        // Boat l = 5
        if (r < 6 && c < 6) {
            notMiss5.push_back(std::make_tuple(r, c, 0));
            notMiss5.push_back(std::make_tuple(r, c, 1));
        }
        else if (r >= 6 && c <= 5) {
            notMiss5.push_back(std::make_tuple(r, c, 0));
        }
        else if (c >= 6 && r <= 5) {
            notMiss5.push_back(std::make_tuple(r, c, 1));
        }
    }
    int iters = 0;
    while (!timeout) {
        
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                grid[i][j] = 0;
            }
        }

        if (editGrid2(5, notMiss5)) {
            if (editGrid2(4, notMiss4)) {
                if (editGrid2(3, notMiss3)) {
                    if (editGrid2(3, notMiss3)) {
                        if (editGrid2(2, notMiss2)) {
                            // Valid config, update frequency grid
                            for (int i = 0; i < 10; i++) {
                                for (int j = 0; j < 10; j++) {
                                    if (this->grid[i][j] == 2) {
                                        frequency_grid[i][j] = frequency_grid[i][j] + static_cast<int>(pow(100.0 - getTrackingBoard().getUnidentifiedSquaresList().size(), 2.0)/10);
                                    }
                                }
                            }
                            iters++;
                        }
                        iters++;
                    }
                    iters++;
                }
                iters++;
            }
            iters++;
        }

        // Check if movetime exceeded
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        iters++;
        if (elapsed.count() > searchtime) {
            timeout = true;
        }
    }
    std::cout << "Iters = " << iters << std::endl;

    // Checkerboarding for unknowns > mp_x
    if (unknowns_size > mp_x_3) {
        int checker_index = 100 - unknowns_size;
        if (random_seed == 0) {
            if (checker_index < 13) {
                guess = large_checkerboard_positions_1[0][checker_index];
            }
            else if (checker_index < 26) {
                guess = large_checkerboard_positions_1[1][checker_index - static_cast <long long>(13)]; // Cast to please VS
            }
            else {
                guess = large_checkerboard_positions_1[2][checker_index - static_cast <long long>(26)];
            }
        }
        else {
            if (checker_index < 13) {
                guess = large_checkerboard_positions_2[0][checker_index];
            }
            else if (checker_index < 26) {
                guess = large_checkerboard_positions_2[1][checker_index - static_cast <long long>(13)]; // Cast to please VS
            }
            else {
                guess = large_checkerboard_positions_2[2][checker_index - static_cast <long long>(26)];
            }
        }
    }
    else {
        // Remove confirmed hits from frequency grid
        for (std::tuple<int, int> h : hits) {
            frequency_grid[std::get<0>(h)][std::get<1>(h)] = 0;
        }

        // Remove confirmed misses from frequency grid
        for (std::tuple<int, int> m : misses) {
            frequency_grid[std::get<0>(m)][std::get<1>(m)] = 0;
        }

        // Remove one tile empty spaces
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                bool onetileempty = true;
                // Down
                if (i + 1 < 10) {
                    if (getTrackingBoard().evalGridPos(i + 1, j) != 1) {
                        onetileempty = false;
                    }
                }
                // Up
                if (i - 1 > -1) {
                    if (getTrackingBoard().evalGridPos(i - 1, j) != 1) {
                        onetileempty = false;
                    }
                }
                // Left
                if (j + 1 < 10) {
                    if (getTrackingBoard().evalGridPos(i, j + 1) != 1) {
                        onetileempty = false;
                    }
                }
                // Right
                if (j - 1 > -1) {
                    if (getTrackingBoard().evalGridPos(i, j - 1) != 1) {
                        onetileempty = false;
                    }
                }

                if (onetileempty) {
                    frequency_grid[i][j] = 0;
                }
            }
        }

        // Find highest probability square
        int best_r = -1;
        int best_c = -1;
        int best_empty = 0;
        int best_freq = 0;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (frequency_grid[i][j] != 0) {
                    int adjacent = 0;
                    int empty = 0;
                    if (i + 1 < 10) {
                        if (getTrackingBoard().evalGridPos(i + 1, j) != 1) {
                            empty++;
                        }
                        adjacent++;
                    }
                    if (i - 1 > -1) {
                        if (getTrackingBoard().evalGridPos(i - 1, j) != 1) {
                            empty++;
                        }
                        adjacent++;
                    }
                    if (j + 1 < 10) {
                        if (getTrackingBoard().evalGridPos(i, j + 1) != 1) {
                            empty++;
                        }
                        adjacent++;
                    }
                    if (j - 1 < 10) {
                        if (getTrackingBoard().evalGridPos(i, j - 1) != 1) {
                            empty++;
                        }
                        adjacent++;
                    }

                    if (frequency_grid[i][j]*empty > best_freq*best_empty) {
                        best_r = i;
                        best_c = j;
                        best_empty = empty;
                        best_freq = frequency_grid[i][j];
                    }
                }
            }
        }
        guess = std::make_tuple(best_r, best_c);

        // In case frequency_grid empty
        if (guess == std::make_tuple(-1, -1)) {
            guess = randomNBGuess();
        }
    }

    return guess;
}

bool Ai::editGrid(int length)
{
    int dir, start_r, start_c;
    dir = rand() % 2; // 0 for right, 1 for down
    if (dir == 0) {
        start_r = rand() % 10;
        start_c = rand() % (10 - length + 1);
    }
    else {
        start_r = rand() % (10 - length + 1);
        start_c = rand() % 10;
    }
    for (int i = 0; i < length; i++) {
        this->grid[start_r + i*dir][start_c + i*(1-dir)] = 2;
    }
    bool valid = true;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            // 1 on tracking board must match 0 on test grid
            // 2 on tracking board must match 2 on test grid
            if (this->getTrackingBoard().evalGridPos(i, j) == 1 && this->grid[i][j] != 0) {
                valid = false;
                break;
            }
            if (this->getTrackingBoard().evalGridPos(i, j) == 2 && this->grid[i][j] != 2) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            break;
        }
    }

    return valid;
}

bool Ai::editGrid2(int length, std::vector<std::tuple<int, int, int>> notMiss)
{
    int dir, start_r, start_c;
    int index = rand() % static_cast<int>(notMiss.size());

    start_r = std::get<0>(notMiss[index]);
    start_c = std::get<1>(notMiss[index]);
    dir = std::get<2>(notMiss[index]);

    for (int i = 0; i < length; i++) {
        this->grid[start_r + i * dir][start_c + i * (1 - dir)] = 2;
    }
    bool valid = true;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            // 1 on tracking board must match 0 on test grid
            // 2 on tracking board must match 2 on test grid
            if (this->getTrackingBoard().evalGridPos(i, j) == 1 && this->grid[i][j] != 0) {
                valid = false;
                break;
            }
            if (this->getTrackingBoard().evalGridPos(i, j) == 2 && this->grid[i][j] != 2) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            break;
        }
    }

    return valid;
}

void Ai::resetGrid()
{
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            this->grid[i][j] = 0;
        }
    }
}

// Destructor
Ai::~Ai()
{
}
