#include "pch.h"
#include "CppUnitTest.h"
#include "../Battleship/Ai.h"
#include "../Battleship/BattleshipGame.h"
#include "../Battleship/Board.h"
#include "../Battleship/Boat.h"
#include "../Battleship/Player.h"
#include "../Battleship/PrimaryBoard.h"
#include "../Battleship/TrackingBoard.h"
#include "../Battleship/User.h"
#include "../Battleship/Ai.cpp"
#include "../Battleship/BattleshipGame.cpp"
#include "../Battleship/Board.cpp"
#include "../Battleship/Boat.cpp"
#include "../Battleship/Player.cpp"
#include "../Battleship/PrimaryBoard.cpp"
#include "../Battleship/TrackingBoard.cpp"
#include "../Battleship/User.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BasicTests
{
	// Tests for helper functions in BattleShipGame.cpp
	TEST_CLASS(Helper_Functions)
	{
	public:
		// Tests for std::string convertPosTupleToString(std::tuple<int,int>)
		TEST_METHOD(TestConvertPosTupleToString)
		{
			User u = User();
			std::string a = u.convertPosTupleToString(std::make_tuple(0, 0));
			std::cout << a << "== a1" << std::endl;
			Assert::IsTrue(a == "a1");
			
			a = u.convertPosTupleToString(std::make_tuple(0, 9));
			std::cout << a << "== j1" << std::endl;
			Assert::IsTrue(a == "j1");
			
			a = u.convertPosTupleToString(std::make_tuple(9, 0));
			std::cout << a << "== a10" << std::endl;
			Assert::IsTrue(a == "a10");

			a = u.convertPosTupleToString(std::make_tuple(9, 9));
			std::cout << a << "== j10" << std::endl;
			Assert::IsTrue(a == "j10");
		}

		// Tests for std::tuple<int,int> convertPosStringToTuple(std::string)
		TEST_METHOD(TestConvertPosStringToTuple)
		{
			User u = User();
			std::tuple<int, int> a = u.convertPosStringToTuple("a1");
			std::cout << std::to_string(std::get<0>(a)) << "," << std::to_string(std::get<0>(a)) << "== 0,0" << std::endl;
			Assert::IsTrue(a == std::make_tuple(0, 0));

			a = u.convertPosStringToTuple("j1");
			std::cout << std::to_string(std::get<0>(a)) << "," << std::to_string(std::get<0>(a)) << "== 0,9" << std::endl;
			Assert::IsTrue(a == std::make_tuple(0, 9));

			a = u.convertPosStringToTuple("a10");
			std::cout << std::to_string(std::get<0>(a)) << "," << std::to_string(std::get<0>(a)) << "== 9,0" << std::endl;
			Assert::IsTrue(a == std::make_tuple(9, 0));

			a = u.convertPosStringToTuple("j10");
			std::cout << std::to_string(std::get<0>(a)) << "," << std::to_string(std::get<0>(a)) << "== 9,9" << std::endl;
			Assert::IsTrue(a == std::make_tuple(9, 9));
		}

		// Tests for int converColToInt(char col)
		TEST_METHOD(TestConvertColToInt)
		{
			char col = 'a';
			Assert::IsTrue(convertColToInt(col) == 0);

			col = 'A';
			Assert::IsTrue(convertColToInt(col) == 0);

			col = 'J';
			Assert::IsTrue(convertColToInt(col) == 9);

			col = 'J';
			Assert::IsTrue(convertColToInt(col) == 9);
		}

		// Tests for int convertDirectionToInt(char direction)
		TEST_METHOD(TestConvertDirectionToInt) 
		{
			char dir = 'u';
			Assert::IsTrue(convertDirectionToInt(dir) == 0);

			dir = 'd';
			Assert::IsTrue(convertDirectionToInt(dir) == 2);

			dir = 'l';
			Assert::IsTrue(convertDirectionToInt(dir) == 3);

			dir = 'r';
			Assert::IsTrue(convertDirectionToInt(dir) == 1);
		}
	};

	// Tests for Boat class methods
	TEST_CLASS(Boat_Class) {
		// Test the setpos() method
		TEST_METHOD(TestSetPos) 
		{
			// Technically this is a constructor only method
			// and so the operation performed below (to add a piece
			// location to an already constructed boat is illegal, but
			// for the purposes of a test is fine)
			Boat b = Boat(3, 4, 7, 2);
			int r = 7;
			int c = 7;
			b.setpos(r,c);
			bool pass = false;
			for (int i = 0; i < b.getRPos().size(); i++) {
				if (b.getRPos()[i] == r && b.getCPos()[i] == c) {
					pass = true;
					break;
				}
			}
			Assert::IsTrue(pass);
		}

		// Test the checkHit() method
		TEST_METHOD(TestCheckHit) 
		{
			Boat b1 = Boat(2, 1, 1, 1);
			Boat b2 = Boat(3, 4, 7, 2);
			Boat b3 = Boat(3, 5, 4, 1);
			Boat b4 = Boat(4, 2, 2, 2);
			Boat b5 = Boat(5, 8, 1, 1);

			std::vector<Boat> boats_list = { b1, b2, b3, b4, b5 };

			for (int r = 0; r < 10; r++) {
				for (int c = 0; c < 10; c++) {
					bool inAnyBoat = false;
					for (Boat& b : boats_list) {
						bool inBoat = false;
						for (int i = 0; i < b.getRPos().size(); i++) {
							if (b.getRPos()[i] == r && b.getCPos()[i] == c) {
								inBoat = true;
								inAnyBoat = true;
							}
						}
						if (inBoat) {
							Assert::IsTrue(b.checkHit(r, c) == true);
						}
					}
					if (!inAnyBoat) {
						for (Boat& b : boats_list) {
							Assert::IsTrue(b.checkHit(r, c) == false);
						}
					}

				}
			}
		}
		// Test the update function
		TEST_METHOD(TestUpdate) {
			Boat b1 = Boat(2, 1, 1, 1);
			Boat b2 = Boat(3, 4, 7, 2);
			Boat b5 = Boat(5, 8, 1, 1);

			// Updates on hit
			// Boat 1
			std::vector<int> correctStatus = { 1,0 };
			b1.update(1,1);
			Assert::IsTrue(b1.getStatus() == correctStatus);

			correctStatus = { 1,1 };
			b1.update(1, 2);
			Assert::IsTrue(b1.getStatus() == correctStatus);

			// Boat 2
			correctStatus = { 0,1,0 };
			b2.update(5, 7);
			Assert::IsTrue(b2.getStatus() == correctStatus);

			correctStatus = { 1,1,0 };
			b2.update(4, 7);
			Assert::IsTrue(b2.getStatus() == correctStatus);

			correctStatus = { 1,1,1 };
			b2.update(6, 7);
			Assert::IsTrue(b2.getStatus() == correctStatus);

			// Boat 5
			correctStatus = { 0,0,0,1,0 };
			b5.update(8, 4);
			Assert::IsTrue(b5.getStatus() == correctStatus);

			Boat b1_1 = Boat(2, 1, 1, 1);
			Boat b5_1 = Boat(5, 8, 1, 1);

			// Updates on Miss
			// Boat 1
			correctStatus = { 0,0 };
			b1_1.update(9, 9);
			Assert::IsTrue(b1_1.getStatus() == correctStatus);

			correctStatus = { 1,0 };
			b1_1.update(1, 1);
			b1_1.update(1, 0);
			b1_1.update(1, 3);
			b1_1.update(0, 2);
			Assert::IsTrue(b1_1.getStatus() == correctStatus);

			// Boat 5
			correctStatus = { 0,0,0,1,0 };
			b5_1.update(8, 4);
			b5_1.update(1, 1);
			b5_1.update(1, 8);
			b5_1.update(8, 0);
			b5_1.update(8, 6);
			Assert::IsTrue(b5_1.getStatus() == correctStatus);
		}

		// Test the checkDestroyed() method
		TEST_METHOD(TestCheckDestroyed)
		{
			Boat b1 = Boat(2, 1, 1, 1);
			Boat b5 = Boat(5, 8, 1, 1);

			// Boat b1
			Assert::IsTrue(b1.checkDestroyed() == false);
			b1.update(1, 1);
			Assert::IsTrue(b1.checkDestroyed() == false);
			b1.update(1, 2);
			Assert::IsTrue(b1.checkDestroyed() == true);

			// Boat b5
			Assert::IsTrue(b5.checkDestroyed() == false);
			b5.update(8, 2);
			Assert::IsTrue(b5.checkDestroyed() == false);
			b5.update(8, 5);
			Assert::IsTrue(b5.checkDestroyed() == false);
			b5.update(8, 1);
			Assert::IsTrue(b5.checkDestroyed() == false);
			b5.update(8, 4);
			Assert::IsTrue(b5.checkDestroyed() == false);
			b5.update(8, 3);
			Assert::IsTrue(b5.checkDestroyed() == true);
		}

	};

	// Tests for the Board abstract class methods
	TEST_CLASS(Board_Class) 
	{
		TEST_METHOD(TestEvalPrintVal) 
		{
			// TODO: update this test
			User u = User();

			// Test for primary board (boards are interchangeable in this case)
			/*
			Assert::IsTrue(u.getPrimaryBoard().evalPrintVal(0, 0) == " ");
			Assert::IsTrue(u.getPrimaryBoard().evalPrintVal(0, 1) == " ");
			Assert::IsTrue(u.getPrimaryBoard().evalPrintVal(1, 0) == "+");
			Assert::IsTrue(u.getPrimaryBoard().evalPrintVal(1, 1) == "+");
			Assert::IsTrue(u.getPrimaryBoard().evalPrintVal(2, 0) == "O");
			Assert::IsTrue(u.getPrimaryBoard().evalPrintVal(2, 1) == " ");
			Assert::IsTrue(u.getPrimaryBoard().evalPrintVal(3, 0) == "X");
			Assert::IsTrue(u.getPrimaryBoard().evalPrintVal(3, 1) == "X");
			*/
		}

		// Test updateBoard()
		TEST_METHOD(TestUpdateBoard)
		{
			Boat b1 = Boat(2, 1, 1, 1);
			Boat b2 = Boat(3, 4, 7, 2);
			Boat b3 = Boat(3, 5, 4, 1);
			Boat b4 = Boat(4, 2, 2, 2);
			Boat b5 = Boat(5, 8, 1, 1);
			std::vector<Boat> boats_list = { b1, b2, b3, b4, b5 };
			PrimaryBoard b = PrimaryBoard(boats_list);

			int correctGrid[10][10] = {
				{1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
				{0, 3, 2, 0, 0, 0, 0, 0, 0, 0},
				{0, 0, 2, 0, 0, 1, 0, 0, 0, 0},
				{0, 0, 3, 0, 0, 0, 0, 0, 0, 0},
				{1, 0, 3, 0, 0, 0, 1, 3, 0, 0},
				{0, 1, 2, 0, 2, 3, 2, 2, 1, 0},
				{0, 0, 0, 0, 0, 0, 0, 3, 0, 0},
				{0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
				{0, 2, 2, 3, 3, 2, 0, 1, 0, 1},
				{0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
			};

			// Misses
			b.updateBoard(0,0);
			b.updateBoard(4,0);
			b.updateBoard(5,1);
			b.updateBoard(9,1);
			b.updateBoard(7,2);
			b.updateBoard(2,5);
			b.updateBoard(9,6);
			b.updateBoard(0,7);
			b.updateBoard(8,7);
			b.updateBoard(4,6);
			b.updateBoard(8,9);
			b.updateBoard(0,9);
			b.updateBoard(5,8);

			// Hits
			b.updateBoard(1,1);
			b.updateBoard(3,2);
			b.updateBoard(4,2);
			b.updateBoard(8,3);
			b.updateBoard(8,4);
			b.updateBoard(5,5);
			b.updateBoard(6,7);
			b.updateBoard(4,7);

			for (int r = 0; r < 10; r++) {
				for (int c = 0; c < 10; c++) {
					Assert::IsTrue(b.evalGridPos(r, c) == correctGrid[r][c]);
				}
			}
		}
	};

	// Tests for the TrackingBoard class
	TEST_CLASS(TrackingBoard_Class)
	{
		TEST_METHOD(TestRemoveUnidentifiedSquare)
		{
			TrackingBoard t = TrackingBoard();
			int count = 0;
			for (int r = 0; r < 10; r++) {
				for (int c = 0; c < 10; c++) {
					// Remove some random squares and test
					if (rand() % 4 == 0) {
						std::tuple<int, int> square = std::make_tuple(r, c);
						t.removeUnidentifiedSquare(square);
						bool inVector = false;
						for (int i = 0; i < static_cast<int>(t.getUnidentifiedSquaresList().size()); i++) {
							if (t.getUnidentifiedSquaresList()[i] == square) {
								inVector = true;
							}
						}
						Assert::IsTrue(!inVector);
						count++;
					}
				}
			}

			// Redundancy check that the correct number was removed
			Assert::IsTrue(static_cast<int>(t.getUnidentifiedSquaresList().size()) == 100 - count);

		}
		TEST_METHOD(TestAddHitToList)
		{
			TrackingBoard t = TrackingBoard();
			int count = 0;
			for (int r = 0; r < 10; r++) {
				for (int c = 0; c < 10; c++) {
					// Remove some random squares and test
					if (rand() % 4 == 0) {
						std::tuple<int, int> square = std::make_tuple(r, c);
						t.addHitToList(square);
						bool inVector = false;
						for (int i = 0; i < static_cast<int>(t.getHitsList().size()); i++) {
							if (t.getHitsList()[i] == square) {
								inVector = true;
							}
						}
						Assert::IsTrue(inVector);
						count++;
					}
				}
			}

			// Redundancy check that the correct number was removed
			Assert::IsTrue(static_cast<int>(t.getHitsList().size()) == count);
		}
	};

	// Tests for Player class methods
	TEST_CLASS(Player_Class) 
	{
		TEST_METHOD(TestCheckValidPlacement_WithinGrid) 
		{
			// Tests for user.checkValidPlacement() only concerning if the
			// placement is within the grid

			std::vector<Boat> boats_list;
			User u = User();
			
			// Boat exiting left border
			Boat b = Boat(2, 0, 0, 3);
			boats_list = {};
			Assert::IsTrue(u.checkValidPlacement(boats_list, b) == false);

			// Boat exiting right border
			b = Boat(2, 0, 9, 1);
			Assert::IsTrue(u.checkValidPlacement(boats_list, b) == false);

			// Boat exiting top border
			b = Boat(2, 0, 0, 0);
			Assert::IsTrue(u.checkValidPlacement(boats_list, b) == false);

			// Boat exiting bottom border
			b = Boat(2, 9, 9, 2);
			Assert::IsTrue(u.checkValidPlacement(boats_list, b) == false);
		}

		TEST_METHOD(TestCheckValidPlacement_BoatOverlap) 
		{
			// Tests for player.checkValidPlacement() only concerning if the
			// placement overlaps another boat

			User u = User();
			Boat b1 = Boat(2, 1, 1, 1);
			Boat b2 = Boat(3, 4, 7, 2);
			Boat b3 = Boat(3, 5, 4, 1);
			Boat b4 = Boat(4, 1, 2, 2);
			Boat b5 = Boat(5, 4, 1, 1);
			// Boats 1 and 4, and 4 and 5 overlap
			
			std::vector<Boat> boats_list = { b2, b3, b4, b5 };
			Assert::IsTrue(u.checkValidPlacement(boats_list, b1) == false);
			
			boats_list = {b1, b3, b4, b5};
			Assert::IsTrue(u.checkValidPlacement(boats_list, b2) == true);

			boats_list = {b1, b2 ,b4, b5};
			Assert::IsTrue(u.checkValidPlacement(boats_list, b3) == true);

			boats_list = {b1, b2, b3, b5};
			Assert::IsTrue(u.checkValidPlacement(boats_list, b4) == false);

			boats_list = {b1, b2, b3, b4};
			Assert::IsTrue(u.checkValidPlacement(boats_list, b5) == false);
		}

		TEST_METHOD(TestEvalGuess) 
		{
			// Tests the player.evalGuess method to see if it returns the correct
			// evaluation of the guess and (indirectly) whether it updates the boat 
			// statuses properly
			User u = User();
			Boat b1 = Boat(2, 1, 1, 1);
			Boat b2 = Boat(3, 4, 7, 2);
			Boat b3 = Boat(3, 5, 4, 1);
			Boat b4 = Boat(4, 2, 2, 2);
			Boat b5 = Boat(5, 8, 1, 1);
			std::vector<Boat> boats_list = { b1, b2, b3, b4, b5 };
			std::tuple<int, int> guess;
			for (Boat& b : boats_list) {
				u.addBoat(b);
			}

			// Test misses
			guess = std::make_tuple(0, 0);
			Assert::IsTrue(u.evalGuess(guess) == 0);

			guess = std::make_tuple(1, 3);
			Assert::IsTrue(u.evalGuess(guess) == 0);

			guess = std::make_tuple(6, 2);
			Assert::IsTrue(u.evalGuess(guess) == 0);

			guess = std::make_tuple(5, 3);
			Assert::IsTrue(u.evalGuess(guess) == 0);

			guess = std::make_tuple(5, 8);
			Assert::IsTrue(u.evalGuess(guess) == 0);

			guess = std::make_tuple(8, 6);
			Assert::IsTrue(u.evalGuess(guess) == 0);

			// Test hits
			guess = std::make_tuple(1, 1); // Beginning of boat
			Assert::IsTrue(u.evalGuess(guess) == 1);

			guess = std::make_tuple(3, 2); // Middle of boat
			Assert::IsTrue(u.evalGuess(guess) == 1);

			guess = std::make_tuple(4, 2); // Middle of boat
			Assert::IsTrue(u.evalGuess(guess) == 1);

			guess = std::make_tuple(5, 2); // End of boat
			Assert::IsTrue(u.evalGuess(guess) == 1);

			guess = std::make_tuple(5, 5); // Middle of boat
			Assert::IsTrue(u.evalGuess(guess) == 1);

			guess = std::make_tuple(5, 7); // Middle of boat
			Assert::IsTrue(u.evalGuess(guess) == 1);

			guess = std::make_tuple(6, 7); // End of boat
			Assert::IsTrue(u.evalGuess(guess) == 1);

			guess = std::make_tuple(8, 1); // Beginning of boat
			Assert::IsTrue(u.evalGuess(guess) == 1);

			guess = std::make_tuple(8, 2); // Middle of boat
			Assert::IsTrue(u.evalGuess(guess) == 1);

			guess = std::make_tuple(8, 4); // Middle of boat
			Assert::IsTrue(u.evalGuess(guess) == 1);

			guess = std::make_tuple(8, 5); // End of boat
			Assert::IsTrue(u.evalGuess(guess) == 1);

			// Test kills
			guess = std::make_tuple(1, 2); // End of boat
			Assert::IsTrue(u.evalGuess(guess) == 2);

			guess = std::make_tuple(2, 2); // Beginning of boat
			Assert::IsTrue(u.evalGuess(guess) == 2);

			guess = std::make_tuple(4, 7); // Beginning of boat
			Assert::IsTrue(u.evalGuess(guess) == 2);

			guess = std::make_tuple(8, 3); // Middle of boat
			Assert::IsTrue(u.evalGuess(guess) == 2);
		}
	};

	// Tests for Ai Class methods
	TEST_CLASS(Ai_Class)
	{
		TEST_METHOD(TestGetPossibleBoatLocation_diff0)
		{
			Ai ai1 = Ai();
			for (int i = 0; i < 100; i++) {
				int start_r = std::get<0>(ai1.getPossibleBoatLocation(0));
				int start_c = std::get<1>(ai1.getPossibleBoatLocation(0));
				Assert::IsTrue(start_r >= 0 && start_r <= 9 && start_c >= 0 && start_c <= 9);
			}
		}

		TEST_METHOD(TestGetPossibleBoatLocation_diff1)
		{
			Ai ai1 = Ai();
			for (int i = 0; i < 100; i++) {
				int start_r = std::get<0>(ai1.getPossibleBoatLocation(1));
				int start_c = std::get<1>(ai1.getPossibleBoatLocation(1));
				Assert::IsTrue(start_r >= 0 && start_r <= 9 && start_c >= 0 && start_c <= 9);
				Assert::IsTrue(start_r <= 1 || start_r >= 8 || start_c <= 1 || start_c >= 8);
			}
		}

		TEST_METHOD(TestGetPossibleBoatLocation_diff2)
		{
			Ai ai1 = Ai();
			for (int i = 0; i < 100; i++) {
				int start_r = std::get<0>(ai1.getPossibleBoatLocation(2));
				int start_c = std::get<1>(ai1.getPossibleBoatLocation(2));
				Assert::IsTrue(start_r >= 0 && start_r <= 9 && start_c >= 0 && start_c <= 9);
				Assert::IsTrue(start_r == 0 || start_r == 9 || start_c == 0 || start_c == 9);
			}
		}

		/* Broken since organisation change to ai,
		* but the code is the same and passed originally
		* so I haven't written new tests
		TEST_METHOD(TestCheckValidPlacementED2)
		{
			Ai ai1 = Ai();
			// Boats that start outside and move inside
			Boat b1 = Boat(2, 2, 1, 1);
			Boat b2 = Boat(2, 1, 2, 2);
			Boat b3 = Boat(2, 7, 1, 1);
			Boat b4 = Boat(2, 8, 2, 0);
			Boat b5 = Boat(2, 7, 8, 3);
			Boat b6 = Boat(2, 8, 7, 0);
			Boat b7 = Boat(2, 2, 8, 3);
			Boat b8 = Boat(2, 1, 7, 2);

			// Boats that start inside (technically prohibited by ai.chooseBoatLocations()
			// but for completeness
			Boat b9 = Boat(2, 4, 4, 1);
			Boat b10 = Boat(5, 4, 4, 1);

			std::vector<Boat> b_list1 = { b1, b2, b3, b4, b5, b6, b7, b8, b9, b10 };
			for (Boat& b : b_list1) {
				Assert::IsTrue(ai1.checkValidPlacementED2(b) == false);
			}

			// Boats that are are valid
			Boat b11 = Boat(2, 0, 4, 2);
			Boat b12 = Boat(5, 2, 1, 2);
			Boat b13 = Boat(3, 9, 7, 1);

			std::vector<Boat> b_list2 = { b11, b12, b13 };
			for (Boat& b : b_list2) {
				Assert::IsTrue(ai1.checkValidPlacementED2(b) == true);
			}
		}

		TEST_METHOD(TestCheckValidPlacementED1)
		{
			Ai ai1 = Ai();
			// Boats that start outside and move inside
			Boat b1 = Boat(2, 1, 0, 1);
			Boat b2 = Boat(2, 0, 1, 2);
			Boat b3 = Boat(2, 8, 0, 1);
			Boat b4 = Boat(2, 9, 1, 0);
			Boat b5 = Boat(2, 8, 9, 3);
			Boat b6 = Boat(2, 9, 8, 0);
			Boat b7 = Boat(2, 1, 9, 3);
			Boat b8 = Boat(2, 0, 8, 2);

			// Boats that start inside (technically prohibited by ai.chooseBoatLocations()
			// but for completeness
			Boat b9 = Boat(2, 4, 4, 1);
			Boat b10 = Boat(5, 4, 5, 1);

			std::vector<Boat> b_list1 = { b1, b2, b3, b4, b5, b6, b7, b8, b9, b10 };
			for (Boat& b : b_list1) {
				Assert::IsTrue(ai1.checkValidPlacementED1(b) == false);
			}

			// Boats that are are valid
			Boat b11 = Boat(2, 0, 4, 1);
			Boat b12 = Boat(5, 2, 0, 2);
			Boat b13 = Boat(3, 9, 7, 1);

			std::vector<Boat> b_list2 = { b11, b12, b13 };
			for (Boat& b : b_list2) {
				Assert::IsTrue(ai1.checkValidPlacementED1(b) == true);
			}
		}

		TEST_METHOD(TestCheckValidPlacementEnumeration_true)
		{
			// Definitely true
			for (int i = 0; i < 100; i++) {
				Ai ai1 = Ai();
				ai1.chooseBoatLocations(0);
				std::vector<Boat> b_list = ai1.getBoats();
				int index = rand() % b_list.size();
				Boat b = b_list[index];
				b_list.erase(b_list.begin() + index, b_list.begin() + index + 1);

				std::vector<std::tuple<int, int>> misses;

				for (int r = 0; r < 10; r++) {
					for (int c = 0; c < 10; c++) {
						if (misses.size() < 20) {
							if (rand() % 4 == 0) {
								// Not technically a proper use of checkValidPlacement but fine for a test
								// Basically checking if a square is empty
								// TODO: could replace by creating the ai board and checking it
								std::vector<Boat> b_list_long = b_list;
								b_list_long.push_back(b);
								if (ai1.checkValidPlacement(b_list_long, Boat(1, r, c, 0))) {
									misses.push_back(std::make_tuple(r, c));
								}
							}
						}
					}
				}
				Assert::IsTrue(ai1.checkValidPlacementEnumeration(b_list, b, misses) == true);
			}
		}

		TEST_METHOD(TestCheckValidPlacementEnumeration_false)
		{
			// Definitely false
			for (int i = 0; i < 100; i++) {
				Ai ai1 = Ai();
				ai1.chooseBoatLocations(0);
				std::vector<Boat> b_list = ai1.getBoats();
				int index = rand() % b_list.size();
				Boat b = b_list[index];
				b_list.erase(b_list.begin() + index, b_list.begin() + index + 1);

				std::vector<std::tuple<int, int>> misses;
				int count = 0;

				for (int r = 0; r < 10; r++) {
					for (int c = 0; c < 10; c++) {
						if (misses.size() < 20) {
							if (rand() % 4 == 0) {
								// Not technically a proper use of checkValidPlacement but fine for a test
								// Basically checking if a square is empty
								// TODO: could replace by creating the ai board and checking it
								std::vector<Boat> b_list_long = {};
								b_list_long.push_back(b);
								if (!ai1.checkValidPlacement(b_list_long, Boat(1, r, c, 0))) {
									count++;
								}
								misses.push_back(std::make_tuple(r, c));
							}
						}
					}
				}

				if (count == 0) {
					int index2 = rand() % b.getRPos().size();
					misses.push_back(std::make_tuple(b.getRPos()[index2], b.getCPos()[index2]));
				}

				Assert::IsTrue(ai1.checkValidPlacementEnumeration(b_list, b, misses) == false);
			}
		}

		TEST_METHOD(TestChooseBoatLocations_diff0)
		{
			Ai ai1 = Ai();
			ai1.chooseBoatLocations(0);
			for (int i = 0; i < ai1.getBoats().size(); i++) {
				std::vector<Boat> boats;
				for (int j = 0; j < ai1.getBoats().size(); j++) {
					if (j != i) {
						boats.push_back(ai1.getBoats()[j]);
					}
				}
				Assert::IsTrue(ai1.checkValidPlacement(boats, ai1.getBoats()[i]));
			}
		}

		TEST_METHOD(TestChooseBoatLocations_diff1)
		{
			Ai ai1 = Ai();
			ai1.chooseBoatLocations(1);
			for (int i = 0; i < ai1.getBoats().size(); i++) {
				std::vector<Boat> boats;
				for (int j = 0; j < ai1.getBoats().size(); j++) {
					if (j != i) {
						boats.push_back(ai1.getBoats()[j]);
					}
				}
				Assert::IsTrue(ai1.checkValidPlacement(boats, ai1.getBoats()[i]));
				Assert::IsTrue(ai1.checkValidPlacementED2(ai1.getBoats()[i]));
			}
		}

		TEST_METHOD(TestChooseBoatLocations_diff2)
		{
			Ai ai1 = Ai();
			ai1.chooseBoatLocations(2);
			for (int i = 0; i < ai1.getBoats().size(); i++) {
				std::vector<Boat> boats;
				for (int j = 0; j < ai1.getBoats().size(); j++) {
					if (j != i) {
						boats.push_back(ai1.getBoats()[j]);
					}
				}
				Assert::IsTrue(ai1.checkValidPlacement(boats, ai1.getBoats()[i]));
				Assert::IsTrue(ai1.checkValidPlacementED1(ai1.getBoats()[i]));
			}
		}

		TEST_METHOD(TestGuessLocation_diff0)
		{
			Ai ai1 = Ai();
			std::tuple<int, int> guess;
			for (int i = 0; i < 100; i++) {
				guess = ai1.guessLocation(0);
				ai1.updateTrackingBoard(guess, 0);
			}
			Assert::IsTrue(ai1.getTrackingBoard().getUnidentifiedSquaresList().size() == 0);
		}

		TEST_METHOD(TestGuessLocation_diff1)
		{
			Ai ai1 = Ai();
			std::vector<std::tuple<int, int>> guesses = {
				std::make_tuple(0, 0), std::make_tuple(0, 3),std::make_tuple(0, 9),std::make_tuple(3, 3),
				std::make_tuple(3, 4),std::make_tuple(4, 3),std::make_tuple(4, 9),std::make_tuple(7, 0),
				std::make_tuple(9, 0),std::make_tuple(9, 5), std::make_tuple(9, 9)
			};
			std::vector<std::tuple<int, int>> adjacents = {
				std::make_tuple(0, 1), std::make_tuple(0, 2), std::make_tuple(0, 4), std::make_tuple(0, 8),
				std::make_tuple(1, 0), std::make_tuple(1, 3), std::make_tuple(1, 9), std::make_tuple(2, 3),
				std::make_tuple(2, 4), std::make_tuple(3, 2), std::make_tuple(3, 5), std::make_tuple(3, 9),
				std::make_tuple(4, 2), std::make_tuple(4, 4), std::make_tuple(4, 8), std::make_tuple(5, 3),
				std::make_tuple(5, 9), std::make_tuple(6, 0), std::make_tuple(7, 1), std::make_tuple(8, 0),
				std::make_tuple(8, 5), std::make_tuple(8, 9), std::make_tuple(9, 1), std::make_tuple(9, 4),
				std::make_tuple(9, 6), std::make_tuple(9, 8)
			};
			for (std::tuple<int, int> g : guesses) {
				ai1.updateTrackingBoard(g, 1);
			}

			int start_length = static_cast<unsigned int>(ai1.getTrackingBoard().getUnidentifiedSquaresList().size());

			for (int i = 0; i < start_length; i++) {
				std::tuple<int,int> guess = ai1.guessLocation(1);
				if (adjacents.size() > 0) {
					bool in_adjacents = false;
					int index = 0;
					for (std::tuple<int, int> a : adjacents) {
						if (std::get<0>(guess) == std::get<0>(a) && std::get<1>(guess) == std::get<1>(a)) {
							in_adjacents = true;
							break;
						}
						index++;
					}
					Assert::IsTrue(in_adjacents);
					adjacents.erase(adjacents.begin() + index, adjacents.begin() + index + 1);
				}
				ai1.updateTrackingBoard(guess, 0);
			}

			Assert::IsTrue(ai1.getTrackingBoard().getUnidentifiedSquaresList().size() == 0);
		}

		TEST_METHOD(TestGuessLocation_diff2)
		{
			Ai ai1 = Ai();
			ai1.guessLocation(2);
		}
		*/
	};

	/*
	* Deprecated: used for testing timings and permutation counts for complete/monte carlo enumeration
	
	TEST_CLASS(Permutations)
	{
		TEST_METHOD(RandomInitialGuesses)
		{
			// Parameters
			int perms = 0;
			int max_iters = 1;
			int hit_percentage = 17;
			int num_guesses = 60;
			Ai ai1 = Ai();

			// Timing
			auto start = std::chrono::steady_clock::now();

			for (int iter = 0; iter < max_iters; iter++) {
				// Create unknowns list
				std::vector<std::tuple<int, int>> unknowns = {};
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 10; j++) {
						unknowns.push_back(std::make_tuple(i, j));
					}
				}


				// Get n random guesses
				std::vector<std::tuple<int, int>> guesses;
				for (int i = 0; i < num_guesses; i++) {
					int r = rand() % 10;
					int c = rand() % 10;
					bool unique = false;
					int count = 0;

					while (!unique) {
						for (int j = 0; j < guesses.size(); j++) {
							if (std::get<0>(guesses[j]) == r && std::get<1>(guesses[j]) == c) {
								break;
							}
							else {
								count++;
							}
						}
						if (count == guesses.size()) {
							unique = true;
							break;
						}
						else {
							r = rand() % 10;
							c = rand() % 10;
							count = 0;
						}
					}
					guesses.push_back(std::make_tuple(r, c));
				}


				// Split into hits and misses
				std::vector<std::tuple<int, int>> misses = {};
				std::vector < std::tuple<int, int>> hits = {};
				for (int i = 0; i < guesses.size(); i++) {
					if (rand() % 100 > hit_percentage) {
						misses.push_back(guesses[i]);
					}
					else {
						hits.push_back(guesses[i]);
					}
				}

				// Update unknowns
				for (std::tuple<int, int> m : misses) {
					for (int index = 0; index < unknowns.size(); index++) {
						if (std::get<0>(m) == std::get<0>(unknowns[index]) && std::get<1>(m) == std::get<1>(unknowns[index])) {
							unknowns.erase(unknowns.begin() + index, unknowns.begin() + index + 1);
							break;
						}
					}
				}
				for (std::tuple<int, int> h : hits) {
					for (int index = 0; index < unknowns.size(); index++) {
						if (std::get<0>(h) == std::get<0>(unknowns[index]) && std::get<1>(h) == std::get<1>(unknowns[index])) {
							unknowns.erase(unknowns.begin() + index, unknowns.begin() + index + 1);
							break;
						}
					}
				}

				// Create vector of hits + unknowns since both are possible boat start points
				std::vector<std::tuple<int, int>> notMiss;
				notMiss.reserve(unknowns.size() + hits.size());
				notMiss.insert(notMiss.end(), unknowns.begin(), unknowns.end());
				notMiss.insert(notMiss.end(), hits.begin(), hits.end());

				// Enumerate 
				std::vector<Boat> boats_list = {};

				// Boat 1 (carrier, l = 5)
				for (std::tuple<int, int> pos1 : notMiss) {
					boats_list = {};
					// Direction of boat 1 down or right (since up and left are duplicates)
					for (int dir1 = 1; dir1 < 3; dir1++) {
						// If position is valid
						Boat b1 = Boat(5, std::get<0>(pos1), std::get<1>(pos1), dir1);
						if (ai1.checkValidPlacementEnumeration(boats_list, b1, misses)) {
							boats_list.push_back(b1);
							// Boat 2 (battleship, l = 4)
							for (std::tuple<int, int> pos2 : notMiss) {
								for (int dir2 = 1; dir2 < 3; dir2++) {
									Boat b2 = Boat(4, std::get<0>(pos2), std::get<1>(pos2), dir2);
									if (ai1.checkValidPlacementEnumeration(boats_list, b2, misses)) {
										boats_list.push_back(b2);
										// Boat 3 (cruiser, l = 3)
										for (std::tuple<int, int> pos3 : notMiss) {
											for (int dir3 = 1; dir3 < 3; dir3++) {
												Boat b3 = Boat(3, std::get<0>(pos3), std::get<1>(pos3), dir2);
												if (ai1.checkValidPlacementEnumeration(boats_list, b3, misses)) {
													boats_list.push_back(b3);
													// Boat 4 (submarine, l = 3)
													for (std::tuple<int, int> pos4 : notMiss) {
														for (int dir4 = 1; dir4 < 3; dir4++) {
															Boat b4 = Boat(3, std::get<0>(pos4), std::get<1>(pos4), dir4);
															if (ai1.checkValidPlacementEnumeration(boats_list, b4, misses)) {
																boats_list.push_back(b4);
																// Boat 5 (Destroyer, l = 2)
																for (std::tuple<int, int> pos5 : notMiss) {
																	for (int dir5 = 1; dir5 < 3; dir5++) {
																		Boat b5 = Boat(2, std::get<0>(pos5), std::get<1>(pos5), dir5);
																		if (ai1.checkValidPlacementEnumeration(boats_list, b5, misses)) {
																			boats_list.push_back(b5);
																			
																			// Update permutations count
																			perms++;
																			
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
			}

			auto end = std::chrono::steady_clock::now();
			std::chrono::duration<double> elapsed = end - start;

			int average = perms / max_iters;

			// Output
			std::string output = "Average permutations = " + std::to_string(average) + " time = " + std::to_string(elapsed.count());
			const char* c = output.c_str();
			
			Logger::WriteMessage(c);

		}

		TEST_METHOD(CheckerboardInitialGuesses)
		{
			// Parameters
			int perms = 0;
			int max_iters = 1;
			int hit_percentage = 17;
			int num_guesses = 38;
			Ai ai1 = Ai();

			// Checkerboard Grids

			// Large checkerboard (A=13, B=12, C=13)
			std::vector<std::vector<std::tuple<int, int>>> checker_pos_1 = {
				{std::make_tuple(0,0), std::make_tuple(0,4), std::make_tuple(0,8), std::make_tuple(2,2), std::make_tuple(2,6),
				std::make_tuple(4,0), std::make_tuple(4,4), std::make_tuple(4,8), std::make_tuple(6,2), std::make_tuple(6,6),
				std::make_tuple(8,0), std::make_tuple(8,4), std::make_tuple(8,8) }, // A
				{std::make_tuple(0,2), std::make_tuple(0,6), std::make_tuple(2,0), std::make_tuple(2,4), std::make_tuple(2,8),
				std::make_tuple(4,2), std::make_tuple(4,6), std::make_tuple(6,0), std::make_tuple(6,4), std::make_tuple(6,8),
				std::make_tuple(8,2), std::make_tuple(8,6) }, // B
				{std::make_tuple(1,1), std::make_tuple(1,5), std::make_tuple(1,9), std::make_tuple(3,3),
				std::make_tuple(3,7), std::make_tuple(5,1), std::make_tuple(5,5), std::make_tuple(5,9),
				std::make_tuple(7,3), std::make_tuple(7,7), std::make_tuple(9,1), std::make_tuple(9,5), 
				std::make_tuple(9,9) } // C
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

			// Timing
			auto start = std::chrono::steady_clock::now();

			for (int iter = 0; iter < max_iters; iter++) {
				// Create unknowns list
				std::vector<std::tuple<int, int>> unknowns = {};
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 10; j++) {
						unknowns.push_back(std::make_tuple(i, j));
					}
				}


				// Get n guesses
				std::vector<std::tuple<int, int>> guesses;
				Assert::IsTrue(num_guesses <= 38);
				for (int i = 0; i < num_guesses; i++) {
					if (i < 13) {
						guesses.push_back(checker_pos_1[0][i]);
					}
					else if (i < 25) {
						guesses.push_back(checker_pos_1[1][i - 13]);
					}
					else {
						guesses.push_back(checker_pos_1[2][i - 25]);
					}
				}

				// Split into hits and misses
				std::vector<std::tuple<int, int>> misses = {};
				std::vector < std::tuple<int, int>> hits = {};
				for (int i = 0; i < guesses.size(); i++) {
					if (rand() % 100 > hit_percentage) {
						misses.push_back(guesses[i]);
					}
					else {
						hits.push_back(guesses[i]);
					}
				}

				// Update unknowns
				for (std::tuple<int, int> m : misses) {
					for (int index = 0; index < unknowns.size(); index++) {
						if (std::get<0>(m) == std::get<0>(unknowns[index]) && std::get<1>(m) == std::get<1>(unknowns[index])) {
							unknowns.erase(unknowns.begin() + index, unknowns.begin() + index + 1);
							break;
						}
					}
				}
				for (std::tuple<int, int> h : hits) {
					for (int index = 0; index < unknowns.size(); index++) {
						if (std::get<0>(h) == std::get<0>(unknowns[index]) && std::get<1>(h) == std::get<1>(unknowns[index])) {
							unknowns.erase(unknowns.begin() + index, unknowns.begin() + index + 1);
							break;
						}
					}
				}

				// Create vector of hits + unknowns since both are possible boat start points
				std::vector<std::tuple<int, int>> notMiss;
				notMiss.reserve(unknowns.size() + hits.size());
				notMiss.insert(notMiss.end(), unknowns.begin(), unknowns.end());
				notMiss.insert(notMiss.end(), hits.begin(), hits.end());

				// Enumerate 
				std::vector<Boat> boats_list = {};

				// Boat 1 (carrier, l = 5)
				for (std::tuple<int, int> pos1 : notMiss) {
					boats_list = {};
					// Direction of boat 1 down or right (since up and left are duplicates)
					for (int dir1 = 1; dir1 < 3; dir1++) {
						// If position is valid
						Boat b1 = Boat(5, std::get<0>(pos1), std::get<1>(pos1), dir1);
						if (ai1.checkValidPlacementEnumeration(boats_list, b1, misses)) {
							boats_list.push_back(b1);
							// Boat 2 (battleship, l = 4)
							for (std::tuple<int, int> pos2 : notMiss) {
								for (int dir2 = 1; dir2 < 3; dir2++) {
									Boat b2 = Boat(4, std::get<0>(pos2), std::get<1>(pos2), dir2);
									if (ai1.checkValidPlacementEnumeration(boats_list, b2, misses)) {
										boats_list.push_back(b2);
										// Boat 3 (cruiser, l = 3)
										for (std::tuple<int, int> pos3 : notMiss) {
											for (int dir3 = 1; dir3 < 3; dir3++) {
												Boat b3 = Boat(3, std::get<0>(pos3), std::get<1>(pos3), dir2);
												if (ai1.checkValidPlacementEnumeration(boats_list, b3, misses)) {
													boats_list.push_back(b3);
													// Boat 4 (submarine, l = 3)
													for (std::tuple<int, int> pos4 : notMiss) {
														for (int dir4 = 1; dir4 < 3; dir4++) {
															Boat b4 = Boat(3, std::get<0>(pos4), std::get<1>(pos4), dir4);
															if (ai1.checkValidPlacementEnumeration(boats_list, b4, misses)) {
																boats_list.push_back(b4);
																// Boat 5 (Destroyer, l = 2)
																for (std::tuple<int, int> pos5 : notMiss) {
																	for (int dir5 = 1; dir5 < 3; dir5++) {
																		Boat b5 = Boat(2, std::get<0>(pos5), std::get<1>(pos5), dir5);
																		if (ai1.checkValidPlacementEnumeration(boats_list, b5, misses)) {
																			boats_list.push_back(b5);

																			// Update permutations count
																			perms++;

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
			}

			auto end = std::chrono::steady_clock::now();
			std::chrono::duration<double> elapsed = end - start;

			int average = perms / max_iters;

			// Output
			std::string output = "Average permutations = " + std::to_string(average) + " time = " + std::to_string(elapsed.count());
			const char* c = output.c_str();

			Logger::WriteMessage(c);
		}
	};
	*/
}