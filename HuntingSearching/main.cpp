/* main.cpp
*  Author: Zongtai Luo
*  The main function of project1
*/
#include <iostream>
#include <cstdlib>
#include "HuntingMap.h"

using namespace std;

int main(int argc, char** argv)
{	
	// turn off the sync to speed up the process
	cout.sync_with_stdio(false);

	command_opt command = getCommandLineOption(argc,argv);

	char  	buffer;
	char 	input_Mode;
	int 	farm_level;
	int 	farm_size;
	
	cin>>input_Mode>>farm_size>>farm_level;
	cin.get(buffer);

	vector<vector<vector<char>>> Map(farm_level,vector<vector<char>>(farm_size, vector<char>(farm_size,'.')));
	
	HuntingMap myMap(Map,input_Mode,farm_level,farm_size,command);

	myMap.run(cin);
	
	return 0;
}
