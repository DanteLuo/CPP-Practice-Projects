#ifndef STOCK_MARKET_H
#define STOCK_MARKET_H

#include <unordered_map>
#include <map>
#include <queue>

using namespace std;

typedef long long BigInt;

struct command_option_t
{
	bool isSummary;
	bool isVerbose;
	bool isMedian;
	bool isTransfers;
	bool isInsider;
	bool isTimeTransferTrade;
	string inputMode;
	vector<string> insiderEquity;
	vector<string> timeTravlTradingEquity;

	command_option_t(void){}
};

struct pseudo_random_input_t
{
	int randomSeed;
	int numOrders;
	char lastClient;
	char lastEquity;
	double arrivalRate;

	pseudo_random_input_t(void){}
};

struct client_t
{
	string name;
	BigInt Sell;
	BigInt Buy;
	BigInt Transfer;

	client_t(string name_):
	name(name_)
	{}
};  

struct order_t
{
	int id;
	string client_name;
	BigInt price;
	BigInt share;

	order_t(int id_, string client_name_, BigInt price_, BigInt share_):
	id(id_),
	client_name(client_name_),
	price(price_),
	share(share_)
	{}
};

struct comp_sell
{
	bool operator()(const Order *order_one, const Order *order_two) const{
		if (order_one->price == order_two->price) {
			return order_one->id>order_two->id;
		} 
		else return order_one->price>order_two->price;
	}
};

struct comp_buy
{
	bool operator()(const Order *order_one, const Order *order_two) const{
		if (order_one->price == order_two->price) {
			return order_one->id>order_two->id;
		} 
		else return order_one->price<order_two->price;
	}
};

struct equity_t
{
	string equity_name;
	BigInt median;
	priority_queue<order_t, vector<order_t*>, comp_sell> sell;
	priority_queue<order_t, vector<order_t*>, comp_buy> buy;
	priority_queue<BigInt, vector<BigInt>> left_of_median;
	priority_queue<BigInt, vector<BigInt>, greater<BigInt> > right_of_median;
	vector<BigInt> equity_price_record;
	vector<BigInt> order_timestamp_record;

	equity_t(int order_num, string equity_name_, string client_name_, string action, BigInt price, BigInt quantity):
	equity_name(equity_name_),
	median(-1)
	{
		if (action == "BUY")
		{
			buy.insert(new order_t(order_num, client_name_, price, quantity));
		}
		else if (action == "SELL")
		{
			sell.insert(new order_t(order_num, client_name_, price, quantity));
		}
	}
};

struct summary_data_t
{
	BigInt commission_;
	BigInt moneyTransfered_;
	int complete_trades_;
	BigInt complete_shares_trades_;

	summary_data_t(void):
	commission_(0),
	moneyTransfered_(0),
	complete_trades_(0),
	complete_shares_trades_(0)
	{}
};


class StockMarket
{
public:
	StockMarket(command_option_t& command, pseudo_random_input_t& pseudo_random_input);

	void runStockMarket(void);

	~StockMarket();
private:
	void runPRStockMarket(void);
	void runTLStockMarket(void);
	void processOrder(BigInt timestamp, 
						 string name, 
						 string action, 
						 string equity, 
						 BigInt price, 
						 BigInt quantity);
	void invalidInput(void);
	bool is_alnum_underscore(char c);
	bool is_alnum_underscore_dot(char c);
	void processTrade(string equity);
	void updateMedian(string equity, BigInt trade_price);
	void processInsider(string equity);
	void printMedian(void);
	void printSummary(void);
	void printTransfers(void);
	void printTimeTransferTrade(void);
	void processTTT(string equity);

	command_option_t command_;
	pseudo_random_input_t pseudo_random_input_;

	BigInt current_timestamp_;
	int order_num_;

	summary_data_t summary_;
	map<string,equity_t*> equity_table;
	map<string,client_t*> client_table;
	vector<string> 		  time_travelers;
};

#endif // STOCK_MARKET_H
