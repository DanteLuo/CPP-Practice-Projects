#include <iostream>
#include <getopt.h>
#include "stock_market.h"
using namespace std;

void printHelp();

int main(int argc, char * argv[]) {

	struct option longOpts[] = {
		{"help", 		no_argument, 		NULL, 'h'},
		{"summary", 	no_argument, 		NULL, 's'},
		{"verbose", 	no_argument, 		NULL, 'v'},
		{"median", 		no_argument, 		NULL, 'm'},
		{"transfers", 	no_argument, 		NULL, 't'},
		{"insider", 	required_argument,	NULL, 'i'},
		{"ttt", 		required_argument,	NULL, 'g'},
		{NULL,			0,					NULL,  0 }
	};

	command_option_t command;
	pseudo_random_input_t pseudo_random_input;
	opterr = 1;
	int opt = 0, index = 0;
	while ((opt = getopt_long(argc,argv,"hsvmti:g:",longOpts,&index)) != -1) {
		switch (opt) {
			case 'h':
				printHelp();
				return 0;
			case 's':
				command.isSummary = true;
				break;
			case 'v':
				command.isVerbose = true;
				break;
			case 'm':
				command.isMedian = true;
				break;
			case 't':
				command.isTransfers = true;
				break;
			case 'i':
				command.isInsider = true;
				command.insiderEquity.push_back(optarg);
				break;
			case 'g':
				command.isTimeTransferTrade = true;
				command.timeTravlTradingEquity.push_back(optarg);
				break;
			case '?':
			default:
				printHelp();
				exit(1);
		}
	}

	cin>>command.inputMode;
	char buffer;
	string buffer_string;
	cin.getline(buffer);

	if (command.inputMode == "PR")
	{
		cin>>buffer_string>>pseudo_random_input.randomSeed;
		cin>>buffer_string>>pseudo_random_input.numOrders;
		cin>>buffer_string>>pseudo_random_input.lastClient;
		cin>>buffer_string>>pseudo_random_input.lastEquity;
		cin>>buffer_string>>pseudo_random_input.arrivalRate;
	}

	StockMarket stock_market(command, pseudo_random_input);

	stock_market.runStockMarket();

	return 0;
}

static const char helpText[] = "NAME\n\
\tmarket - an electronic stock exchange market\n\n\
SYNOPSIS\n\
\tmarket [OPTION]\n\n\
DESCRIPTION\n\
\tMarket is an electronic stock exchange market simulator. The market offers\n\
\ta variety of equities. Any market client can place a buy or sell order on\n\
\tan equity to request that a transaction be executed when matching sellers\n\
\tor buyers become available. The simulator takes in buy and sell orders for\n\
\ta variety of equities as they arrive and matches buyers with sellers to\n\
\texecute trades as quickly as possible. In addition, it has the ability to\n\
\tcreate an insider trader, allowing for instantaneous trades and for the\n\
\tstock exchange to make even more money on the side.\n\n\
\t-s, --summary\n\
\t\tAn optional flag that indicates the program should print a variety of\n\
\t\tsummary information describing the day\'s trades.\n\n\
\t-v, --verbose\n\
\t\tAn optional flag that indicates the program should print additional\n\
\t\toutput information while trades are being executed.\n\n\
\t-m, --median\n\
\t\tAn optional flag that indicates the program should print the current\n\
\t\tmedian match price for each equity.\n\n\
\t-t, --transfers\n\
\t\tAn optional flag that indicates the program should print additional\n\
\t\toutput information at the end of the day to show the net amount of\n\
\t\tfunds transferred by all clients.\n\n\
\t-i, --insider EQUITY_SYMBOL\n\
\t\tAn optional flag that may appear more than once with different equity\n\
\t\tsymbols as arguments. The insider option allows for the program itself\n\
\t\tto have an advantage over other buyers and sellers by being able to\n\
\t\tinstantaneously insert its own buys and sells into the input queue.\n\n\
\t-g, --ttt EQUITY_SYMBOL\n\
\t\tAn optional flag that may appear more than once with different equity\n\
\t\tsymbols as arguments. The Time-Travel Trading option requests that, at\n\
\t\tthe end of the day the program determines what was the best time to\n\
\t\tbuy (once) and then subsequently sell (once) a particular equity during\n\
\t\tthe day to maximize profit.\n\n\
\t-h, --help\n\
\t\tAn optional flag that prints this help message and then exits.\n";

void printHelp() {
	cout << helpText << flush;
}
