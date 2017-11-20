/* utils.cpp
*  Author: Zongtai Luo
*  utils function implementation
*/

#include <getopt.h>
#include <iostream>
#include "utils.h"

using namespace std;

command_opt getCommandLineOption(int argc, char** argv)
{
	int isDuplicate = 0;
	bool OutputValidation = false;
	command_opt command;

	static struct option long_options[]
		{
			{"stack", no_argument, NULL, 's'},
			{"queue", no_argument, NULL, 'q'},
			{"output", required_argument, NULL, 'o'},
			{"help", no_argument, NULL, 'h'}

		};

	int option = 0;
	int index = 0;
	opterr = 0;

	while((option = getopt_long(argc,argv,"sqo:h",long_options,&index)) != -1)
	{
		switch(option)
		{
		    case 's':
		    	command.isStack = true;
		    	isDuplicate++;
		    	break;

		    case 'q':
		    	command.isStack = false;
		    	isDuplicate++;
		    	break;
		    case 'o':
		    	command.outputMode = *optarg;
		    	// if (command.outputMode == "M" || command.outputMode == "L"){OutputValidation = true;}
		    	break;
		    case 'h':
		    	print_help_msg();
		    	break;
		    default:
		    	cout<<"Please type valid input, for help please type -h.\n";
		}
	}

	if ((isDuplicate == 0)||(isDuplicate > 1))
	{
		cerr<<"Please select at least and only one method from queue or stack." <<endl;
		exit(1);
	}

	// if (!OutputValidation)
	// {
	// 	cerr<<"Please select correct ouput mode M for Map and L for coordinates output." <<endl;
	// 	exit(1);
	// }

	return command;
}


void print_help_msg()
{
	cout<<"This is program for finding the correct way to the buck for the hunter and can be run in several options listed below.\n";
	cout<<"		-s or --stack select stack method as the container\n";
	cout<<"		-q or --queue select queue method as the container\n";
	cout<<"		-o for selecting output mode M for map output and L for corrdinates output\n";
	cout<<"		-h for help\n";
}

