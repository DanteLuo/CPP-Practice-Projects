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
};


class HuntingMap
{

public:
	HuntingMap(vector<vector<vector<char>>>& Map, const char& input_Mode, const int level, const int size, const command_opt& option);

	void initMap(istream& is);

	void printMap(bool isOutputMap);

	~HuntingMap();

private:
	void parseCoords(string& line);
	void push(point& target);

	char input_Mode_;
	int  level_;
	int  size_;

	command_opt option_;

	vector<vector<vector<char>>> Map_;
	vector<vector<vector<char>>> Output_Map_;
	deque<point> next_move;

};

#endif
