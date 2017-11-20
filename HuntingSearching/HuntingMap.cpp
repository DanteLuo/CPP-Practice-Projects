// Map.cpp
// Aurthur: Zongtai Luo
// Implementation of Map class.

#include <iostream>
#include "HuntingMap.h"

using namespace std;

HuntingMap::HuntingMap(vector<vector<vector<char>>>& Map, const char& input_Mode, const int level, const int size, const command_opt& option):
TerminateFlag(false),
input_Mode_(input_Mode),
level_(level),
size_(size),
option_(option),
Map_(Map),
findPath(false)
{
	vector<vector<vector<bool> > > Bool_Map(level,vector<vector<bool> >(size, vector<bool>(size, false)));
	Bool_Map_ = Bool_Map;
}


HuntingMap::~HuntingMap()
{}


void HuntingMap::run(istream& is)
{
	initMap(is);
	searchBuck();
	retrievePath();
	printResult();
}


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
						Map_[level][row][col] = MapInputCharacterCheck(line_buffer[col]); // adding start position
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
			getline(is,line_buffer);
			cout<<line_buffer<<"\n";
		
			if(line_buffer[0] == '/' && line_buffer[1] == '/')
			{
				continue;
			}
			else
			{
				// parseCoords(line_buffer); // problem with istream input
				int level;
				int row;
				int col;
				char map_unit;
				char bracket,comma;
				istringstream line(line_buffer);

				line>>bracket>>row>>comma>>col>>comma>>level>>comma>>map_unit>>bracket;

				cout<<row<<col<<level<<map_unit<<"\n";

				point ptr_buffer(level,row,col);

				if (CoordsCheck(ptr_buffer))
				{
					Map_[level][row][col] = MapInputCharacterCheck(map_unit);
					if (map_unit == 'S')
					{
						push(ptr_buffer);
					}
				}
				else
				{
					cerr<<"Please check your coordinates." <<endl;
					exit(1);
				}
			}
		}
	}

	Path_Map_ = Map_;
}


void HuntingMap::searchBuck(void)
{
	if (next_move.empty())
	{
		cerr<<"Please specify starting point as 'S'. \n";
		exit(1);
	}

	while(!TerminateFlag) 
	{
	    updateMove();
	    printResult(true); // for debug
	}
	
}


void HuntingMap::printResult(bool isDebug)
{
	ostringstream os;
	os.sync_with_stdio(false);

	os<<size_<<"\n"<<level_<<"\n"<<"//path taken\n";

	if (option_.outputMode == "M" && !isDebug)
	{
		printMap(os,true);
	}
	else if(option_.outputMode == "L" && !isDebug)
	{
		printCoordsPath(os);
	}
	else
	{
		printMap(os);
	}

	cout<<os.str();
}


void HuntingMap::printMap(ostringstream& os, bool isOutputMap)
{
	int level(0);
	
	if (!findPath)
	{
		os<<"//path not found.\n";
		cout<<os.str();
		return;
	}

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
		            os<<Map_[level][row][col];
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
			os<<"// farm "<<level<<"\n";

			int row(0);
		    while(row<size_) 
		    {
		    	int col(0);
		        while(col<size_) 
		        {
		            os<<Path_Map_[level][row][col];
		            col++;
		        }

		        os<<"\n";
		        row++;
		    }

		    level++;
		}
	}
}


void HuntingMap::printCoordsPath(ostringstream& os)
{
	while(path.empty()) 
	{
	    point current_point = path.front();
	    path.pop_front();
	    printPoint(current_point, os);
	}
}


void HuntingMap::retrievePath(void)
{
	if (!findPath)
	{
		return;
	}

	point current_point = endpoint;
	push(current_point,true);

	while(!getStartingFlag(current_point)) 
	{
		char cmd = readingMap(current_point,2);
	 	char reverse_cmd = reverseCmd(cmd);
	 	moveFollowCmd(current_point, reverse_cmd);
	 	updateMap(current_point,cmd);
	 	push(current_point,true);
	}
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

	cout<<row<<col<<level<<map_unit<<"\n";

	point ptr_buffer(level,row,col);

	if (CoordsCheck(ptr_buffer))
	{
		Map_[level][row][col] = MapInputCharacterCheck(map_unit);
		if (map_unit == 'S')
		{
			push(ptr_buffer);
		}

		return;
	}

	cerr<<"Please check your coordinates." <<endl;
	exit(1);
}


void HuntingMap::push(point& target, bool isPath)
{
	if (!isPath)
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
	else
	{
		if (option_.isStack)
		{
			path.push_front(target);
		}
		else
		{
			path.push_back(target);
		}
	}
	
}


bool HuntingMap::CoordsCheck(point ptr)
{
	if (ptr.level>=0 && ptr.level<level_ && ptr.row>=0 && ptr.row<size_ && ptr.col>=0 && ptr.col<size_)
	{
		return true;
	}

	return false;
}


char HuntingMap::MapInputCharacterCheck(char map_unit)
{
	if (map_unit == '.' || map_unit == '#' || map_unit == '>' || map_unit == '<' || map_unit == 'B' || map_unit == 'S')
	{
		return map_unit;
	}
	else
	{
		cerr<<"Invalid map characters." <<endl;
		exit(1);
	}

	return map_unit;
}


char HuntingMap::readingMap(point& ptr, int map_id)
{
	if (map_id == 0)
	{
		return Map_[ptr.level][ptr.row][ptr.col];
	}
	else if (map_id == 1)
	{
		return !Bool_Map_[ptr.level][ptr.row][ptr.col];
	}
	else
	{
		return Path_Map_[ptr.level][ptr.row][ptr.col];
	}
}


bool HuntingMap::getStartingFlag(point ptr)
{
	if (readingMap(ptr,2) == 'S')
	{
		return true;
	}

	return false;
}


void HuntingMap::getTerminateFlag(point checkpoint)
{
	if (readingMap(checkpoint) == 'B')
	{
		findPath = true;
		TerminateFlag = true;
		endpoint = checkpoint;
	}

	if (next_move.empty())
	{
		TerminateFlag = true;
	}
}


void HuntingMap::updateMove(void)
{
	point current_point = next_move.front();
	next_move.pop_front();
	
	char current_unit = readingMap(current_point);

	if (current_unit == '>' && checkNextStep(current_point, 'u'))
	{
		stepUpdate(current_point, 'u');
	}
	else if (current_unit == '<' && checkNextStep(current_point, 'd'))
	{
		stepUpdate(current_point, 'd');
	}
	else 
	{
		if (checkNextStep(current_point, 'n'))
		{
			stepUpdate(current_point, 'n');
			if(findPath){return;}
		}
		if (checkNextStep(current_point, 'e'))
		{
			stepUpdate(current_point, 'e');
			if(findPath){return;}
		}
		if (checkNextStep(current_point, 's'))
		{
			stepUpdate(current_point, 's');
			if(findPath){return;}
		}
		if (checkNextStep(current_point, 'w'))
		{
			stepUpdate(current_point, 'w');
			if(findPath){return;}
		}
	}
}


bool HuntingMap::checkNextStep(point current_point, char next)
{
	moveFollowCmd(current_point, next);

	if (CoordsCheck(current_point) && readingMap(current_point,1) && readingMap(current_point) != '#')
	{
		return true;
	}
	
	return false;
}


char HuntingMap::reverseCmd(char cmd)
{
	switch(cmd)
	{
		case 'u':
		cmd = 'd';
		break;

		case 'd':
		cmd = 'u';
		break;

		case 'n':
		cmd = 's';
		break;

		case 'e':
		cmd = 'w';
		break;

		case 's':
		cmd = 'n';
		break;

		case 'w':
		cmd = 'e';
		break;
	}

	return cmd;
}


void HuntingMap::moveFollowCmd(point& current_point, char cmd)
{
	switch(cmd)
	{
		case 'u':
		current_point.level++;
		break;

		case 'd':
		current_point.level--;
		break;

		case 'n':
		current_point.row--;
		break;

		case 'e':
		current_point.col++;
		break;

		case 's':
		current_point.row++;
		break;

		case 'w':
		current_point.col--;
		break;
	}
}


void HuntingMap::updatePathMap(point ptr, char cmd)
{
	Path_Map_[ptr.level][ptr.row][ptr.col] = cmd;
}


void HuntingMap::updateBoolMap(point ptr)
{
	Bool_Map_[ptr.level][ptr.row][ptr.col] = true;
}


void HuntingMap::updateMap(point ptr, char cmd)
{
	Map_[ptr.level][ptr.row][ptr.col] = cmd;
}


void HuntingMap::stepUpdate(point current_point, char cmd)
{
	updateBoolMap(current_point);
	moveFollowCmd(current_point, cmd);
	push(current_point);
	updatePathMap(current_point,cmd);
	getTerminateFlag(current_point);
}


void HuntingMap::printPoint(point ptr, ostringstream& os)
{
	char cmd = readingMap(ptr);

	os<<"("<<ptr.row<<","<<ptr.col<<","<<ptr.level<<","<<cmd<<")\n";
}