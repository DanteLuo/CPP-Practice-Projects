/* utils.h
*  Author: Zongtai Luo
*  utils function decleration
*/

#include <sstream>

using namespace std;

struct command_opt
{
	// mode params
	bool isStack;
	string outputMode;
};

// give main function commandline and return information about method and output mode
command_opt getCommandLineOption(int argc, char** argv);

void print_help_msg();
