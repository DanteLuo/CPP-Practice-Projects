#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include "zombie_group.h"
using namespace std;


void printHelp();

int main(int argc, char * argv[]) {

	// get option
	command_opt_t command;
	
	struct option longOpts[] = {
        {"help", 		no_argument, 		NULL, 'h'},
        {"container", 	required_argument, 	NULL, 'c'},
        {"statistics", 	required_argument,	NULL, 's'},
        {"verbose", 	no_argument,		NULL, 'v'},
        {NULL,			0,					NULL,  0 }
	};

	opterr = 1;
	int opt = 0, index = 0;
	while ((opt = getopt_long(argc, argv, "hc:s:v", longOpts, &index)) != -1){
		switch (opt) {
			case 'h':
				printHelp();
				exit(0);
			case 'c':
				command.container_type = optarg;
				break;
			case 's':
				command.isStatsOutput = true;
				command.statsN = atoi(optarg);
				break;
			case 'v':
				command.isVerbose = true;
				break;
			case '?':
			default:
				printHelp();
				exit(1);
		}
	}

	/* Print any remaining command line arguments */
    if (optind < argc) {
       command.file_name = argv[optind];
    } else {
    	cerr << "No mandatory gamefile option!\n";
    	exit(1);
    }

    ZombieGroup zombies(command);

    zombies.runTheGame();

	return 0;
}

const static char helpMessage[] =
"NAME\n\
\trotld - a command line \'Revenge of the Living Deadline\' game \n\
\tsimulation program.\n\
SYNOPSIS\n\
\trotld (-c | --container) CONTAINER [-s | --statistics ] N GAMEFILE \n\
\trotld (-h | --help)\n\
DESCRIPTION\n\
\tROTLD (Revenge of the Living Deadline) game simulator with both zombie \n\
\tand player AI. This program acts as the player in a ROTLD game and shoots \n\
\tzombies in the optimal ordering to achieve the best possible result \n\
\t(stay alive for as long as possible).\n\
OPTIONS\n\
-h, --help\n\
\tPrint this help screen and exit.\n\
-c, --container\n\
\tRequired option that states the type of priority queue to use. Must be one \
of\n\
\tBINARY, POOR_MAN, SORTED, or PAIRING.\n\
-s, --statistics N\n\
\tAn optional option that indicates the program should print additional \
output \n\tstatistics.\n\
-v, --verbose\n\
\tAn optional option that indicates the program should print some debugging\n\
\toutput.\n";

void printHelp() {
	cout << helpMessage << flush;
	return;
}

