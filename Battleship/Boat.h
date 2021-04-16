#pragma once
#include <vector>
#include<algorithm>
#include <string>

/*
* Boat class to represent the different types of boats
* that can be placed. Can technically accept boats of
* any length.
*/
class Boat
{
private:
	// Variables
	std::string name;
	int length;
	std::vector<int> pos_r;
	std::vector<int> pos_c;
	std::vector<int> status;

public:
	// Constructor
	Boat(int length, int start_r, int start_c, int direction);

	// Methods
	void update(int shot_r, int shot_c);
	bool checkHit(int shot_r, int shot_c);
	bool checkDestroyed();
	void setpos(int r, int c);

	// Getter Methods
	int getLength();
	std::vector<int> getRPos();
	std::vector<int> getCPos();
	std::vector<int> getStatus();

	// Setter methods
	void setName(std::string name);

	// Destructor
	virtual ~Boat();

};

