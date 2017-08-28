// Map.h
// Aurthur: Zongtai Luo
// Declearation of Map class.

#ifndef PROJECT1_MAP_H
#define PROJECT1_MAP_H

#include <vector>
#include <deque>
#include "utils.h"

using namespace std;

struct point
{
	int level;
	int row;
	int col;

	point(int level_,int row_,int col_):
	level(level_),
	row(row_),
	col(col_)
	{}

	point(void):
	level(0),
	row(0),
	col(0)
	{}
};


class HuntingMap
{

public:
	HuntingMap(vector<vector<vector<char>>>& Map, const char& input_Mode, const int level, const int size, const command_opt& option);

	void run(istream& is);
	
	~HuntingMap();

private:
	void initMap(istream& is);
	void searchBuck(void);
	void retrievePath(void); 
	void printResult(bool isDebug = false);
	void parseCoords(string& line);
	void push(point& target, bool isPath = false);
	bool CoordsCheck(point ptr);
	char MapInputCharacterCheck(char map_unit);
	char readingMap(point& ptr, int map_id = 0);
	void getTerminateFlag(point checkpoint);
	void updateMove(void);
	bool checkNextStep(point current_point, char next);
	void moveFollowCmd(point& current_point, char cmd);
	void updatePathMap(point ptr, char cmd);
	void updateBoolMap(point ptr);
	void stepUpdate(point ptr, char cmd);
	void printCoordsPath(ostringstream& os);
	bool getStartingFlag(point ptr);
	char reverseCmd(char cmd);
	void updateMap(point ptr, char cmd);
	void printPoint(point ptr, ostringstream& os);
	void printMap(ostringstream& os, bool isOutputMap = false);
	


	bool TerminateFlag;

	char input_Mode_;
	int  level_;
	int  size_;

	command_opt option_;

	vector<vector<vector<char>>> Map_;
	vector<vector<vector<char>>> Path_Map_;
	vector<vector<vector<bool>>> Bool_Map_;
	deque<point> next_move;
	deque<point> path;
	point endpoint;

	bool findPath;

};

#endif
