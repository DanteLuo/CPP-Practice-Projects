// Map.cpp
// Aurthur: Zongtai Luo
// Implementation of Map class.

#include <iostream>
#include "HuntingMap.h"

using namespace std;

HuntingMap::HuntingMap(vector<vector<vector<char>>>& Map, const char& input_Mode, const int level, const int size, const command_opt& option):
input_Mode_(input_Mode),
level_(level),
size_(size),
option_(option),
Map_(Map)
{}


HuntingMap::~HuntingMap()
{}


void HuntingMap::initMap(istream& is)
{
	string line_buffer;
	
	if (input_Mode_ == 'M')
	{
		int level = 0;
		while(level<level_)
		{
			int row = 0;
			while(row<size_)
			{
				getline(is,line_buffer);
				
				if(line_buffer[0] == '/' && line_buffer[1] == '/')
				{
					continue;
				}
				else
				{
					int col = 0;
					while(col<size_)
					{
						Map_[level][row][col] = line_buffer[col]; // adding start position
					    if (line_buffer[col] == 'S')
						{
							point starting_point(level,row,col);
							push(starting_point);
						}
					    col++;
					}
				}

				row++;
			}

				level++;
		}
	}
	else if(input_Mode_ == 'L')
	{
		while(!is.eof())
		{
			if(line_buffer[0] == '/' && line_buffer[1] == '/')
			{
				continue;
			}
			else
			{
				getline(is,line_buffer);
				parseCoords(line_buffer);
			}
		}
	}

	Output_Map_ = Map_;

}


void HuntingMap::printMap(bool isOutputMap)
{
	ostringstream os;
	os.sync_with_stdio(false);

	int level(0);
	
	os<<size_<<"\n"<<level_<<"\n";

	if (isOutputMap)
	{
		while(level<level_)
		{
			os<<"//farm "<<level<<"\n";

			int row(0);
			while(row<size_) 
		    {
		    	int col(0);
		        while(col<size_) 
		        {
		            os<<Output_Map_[level][row][col];
		            col++;
		        }

		        os<<"\n";
		        row++;
		    }

		    level++;
		}
	}
	else
	{
		while(level<level_)
		{
			os<<"//farm "<<level<<"/n";

			int row(0);
		    while(row<size_) 
		    {
		    	int col(0);
		        while(col<size_) 
		        {
		            os<<Map_[level][row][col];
		            col++;
		        }

		        os<<"\n";
		        row++;
		    }

		    level++;
		}
	}

	cout<<os.str();
}


void HuntingMap::parseCoords(string& line)
{
	istringstream line_buffer(line);
	int level;
	int row;
	int col;
	char map_unit;
	char bracket,comma;

	line_buffer>>bracket>>row>>comma>>col>>comma>>level>>comma>>map_unit>>bracket;
	Map_[level][row][col] = map_unit;
	if (map_unit == 'S')
	{
		point starting_point(level,row,col);
		push(starting_point);
	}
}


void HuntingMap::push(point& target)
{
	if (option_.isStack)
	{
		next_move.push_front(target);
	}
	else
	{
		next_move.push_back(target);
	}
}