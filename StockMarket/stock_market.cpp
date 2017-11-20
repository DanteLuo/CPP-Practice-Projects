#include "stock_market.h"

using namespace std;

StockMarket::StockMarket():
command_(command),
pseudo_random_input_(pseudo_random_input),
order_num_(0),
current_timestamp_(0)
{
	for (auto index:command_.insiderEquity)
	{
	    addInsider(index);
	}
}

void StockMarket::addInsider(string& insider_equity)
{
	insider_equity = "INSIDER_"+insider_equity;
	client_table.insert(make_pair(insiderName, new client_t(insiderName));
}

void StockMarket::runStockMarket(void)
{
	if (command_.inputMode=="PR")
	{
		runPRStockMarket();
	}
	else
	{
		runTLStockMarket();
	}

	if (command_.isMedian)
	{
		printMedian();
	}

	cout<<"---End of Day---\n";

	if (command_.isSummary)
	{
		printSummary();
	}

	if (command_.isTransfers)
	{
		printTransfers();
	}

	if (command_.isTimeTransferTrade)
	{
		printTimeTransferTrade();
	}
}

void StockMarket::runPRStockMarket(void)
{
	mt19937 gen(mode.random_seed);
	uniform_int_distribution<char> clients('a', mode.last_client);
	uniform_int_distribution<char> equities('A', mode.last_equity);
	exponential_distribution<> arrivals(mode.arrival_rate);
	bernoulli_distribution action;
	uniform_int_distribution<> price(2, 11);
	uniform_int_distribution<> quantity(1, 30);
	BigInt generator_timestamp = 0;
	BigInt timestamp;
	string client_name;
	string action;
	string equity_symbol;
	BigInt price_;
	BigInt quantity_;
	string buffer;

	for (int i = 0; i<mode.number_of_orders; i++) 
	{
		timestamp = generator_timestamp;
		generator_timestamp += floor(arrivals(gen)+.5);
		buffer = clients(gen);
		client_name = "C_" + tmpString;
		action = (action(gen) ? "BUY":"SELL");
		buffer = equities(gen);
		equity_symbol = "E_" + tmpString;
		price_ = 5 * price(gen);
		quantity_ = quantity(gen);

		processingOrder(timestamp,client_name,action,equity_symbol,price_,quantity_);
	}
}

void StockMarket::runTLStockMarket(void)
{
	BigInt timestamp;
	string name;
	string action;
	string equity;
	BigInt price;
	BigInt quantity;
	string priceRaw;
	string quantityRaw;
	BigInt lastTimestamp = -1;

	while (cin>>timestamp) 
	{
		//Error checking & data reading
		cin >> name >> action;
			
		if (action!="BUY" && action!="SELL") 
		{
			invalidInput();
		}

		cin >> equity >> priceRaw;
		try 
		{
			if (priceRaw[0] != '$') 
			{
				invalidInput();
			}
			priceRaw[0] = ' ';
			price = stoll(priceRaw);
		}
		catch (invalid_argument v) 
		{
		invalidInput();
		}

		cin>> quantityRaw;

		try 
		{
			if (quantityRaw[0] != '#') 
			{
				invalidInput();
			}
			quantityRaw[0] = ' ';
			quantity = stoll(quantityRaw);
		}

		catch (invalid_argument v) 
		{
			invalidInput();
		}

		if (timestamp<0 || price<=0 || lastTimestamp>timestamp || equity.size()>5 ||
			!all_of(name.begin(), name.end(), is_alnum_underscore) ||
			!all_of(equity.begin(), equity.end(), is_alnum_underscore_dot)) 
		{
			invalidInput();
		}

		lastTimestamp = timestamp;
		//End of error checking & data reading
		processOrder(timestamp, name, action, equity, price, quantity);
	}
}

void StockMarket::invalidInput(void) 
{
	cout<<"Invalid input! Please check your input data.\n";
	exit(1);
}

bool StockMarket::is_alnum_underscore(char c) 
{
  return isalnum(c) || c == '_';
}

bool StockMarket::is_alnum_underscore_dot(char c) 
{
  return isalnum(c) || c == '_' || c == '.';
}

void StockMarket::processOrder(BigInt timestamp, 
								  string name, 
								  string action, 
								  string equity, 
								  BigInt price, 
								  BigInt quantity)
{
	if (current_timestamp_!=timestamp)
	{
		if (command_.isMedian)	{printMedian();}
		current_timestamp_ = timestamp;
	}

	if (equity_table.find(equity) == equity_table.end())
	{
		equity_table.insert(equity,new equity_t(order_num, equity, name, action, price, quantity));
		order_num++;
	}

	if (client_table.find(name) == client_table.end())	
	{
		client_table.insert(name,new client_t(name));
	}

	processTrade(equity);

	processInsider(equity);
}

void StockMarket::processTrade(string equity)
{
	while(!equity_table[equity]->sell.empty() && !equity_table[equity]->buy.empty() && equity_table[equity]->buy.top()->price >= equity_table[equity]->sell.top()->price) 
	{
	    BigInt trade_price = (equity_table[equity]->buy.top()->id > equity_table[equity]->sell.top()->id) ? equity_table[equity]->sell.top()->price : equity_table[equity]->buy.top()->price;

	    BigInt quantity = (equity_table[equity]->buy.top()->share > equity_table[equity]->sell.top()->share) ? equity_table[equity]->sell.top()->share : equity_table[equity]->buy.top()->share;

	    equity_table[equity]->buy.top()->share -= quantity;
	    equity_table[equity]->sell.top()->share -= quantity;

	    // update summary data
	    summary.commission_ += (trade_price*quantity)*2/100;
	    summary.moneyTransfered_ += trade_price*quantity;
	    summary.complete_trades_++;
	    summary.complete_shares_trades_ += quantity;

	    // update client data
	    client_table[equity_table[equity]->buy.top()->name]->Buy += quantity;
	    client_table[equity_table[equity]->buy.top()->name]->Transfer += trade_price*quantity;
	    client_table[equity_table[equity]->sell.top()->name]->Sell += quantity;
	    client_table[equity_table[equity]->sell.top()->name]->Transfer += trade_price*quantity;

	    updateMedian(equity, trade_price);

	    // update trade price
	    equity_table[equity]->equity_price_record.push_back(trade_price);
	    equity_table[equity]->order_timestamp_record.push_back(current_timestamp_);

	    // verbose output
	    if (command_.isVerbose)
	    {
	    	cout<<equity_table[equity]->buy.top()->client_name<<" purchased "<<quantity<<" shares of "<<equity<<" from "<<equity_table[equity]->sell.top()->client_name<<" for $" <<trade_price<<"/share.\n";
	    }

	    if (equity_table[equity]->buy.top()->quantity <= 0)
	    {
	    	order_t victim = equity_table[equity]->buy.top();
	    	equity_table[equity]->buy.pop();
	    	delete victim;
	    }

	    if (equity_table[equity]->sell.top()->quantity <= 0)
	    {
	    	order_t victim = equity_table[equity]->sell.top();
	    	equity_table[equity]->sell.pop();
	    	delete victim;
	    }
	}
}

void StockMarket::updateMedian(string equity, BigInt trade_price)
{
	if (equity_table[equity]->median == -1)
	{
		equity_table[equity]->left.push(trade_price);
		equity_table[equity]->median = trade_price;
	}
	else if (equity_table[equity]->left.size() > equity_table[equity]->right.size())
	{
		if (trade_price > equity_table[equity]->median){equity_table[equity]->right.push(trade_price);}
		else 
		{
			BigInt buffer = equity_table[equity]->left.top();
			equity_table[equity]->right.push(buffer);
			equity_table[equity]->left.pop();
			equity_table[equity]->left.push(trade_price);
		}

		equity_table[equity]->median = (equity_table[equity]->left.top() + equity_table[equity]->right.top())/2;
	}
	else if (equity_table[equity]->left.size() == equity_table[equity]->right.size())
	{
		if (trade_price<median)
		{
			equity_table[equity]->left.push(trade_price);
			equity_table[equity]->median = equity_table[equity]->left.top();
		}
		else
		{
			equity_table[equity]->right.push(trade_price);
			equity_table[equity]->median = equity_table[equity]->right.top();
		}
	}
	else
	{
		if (trade_price>median)
		{
			BigInt buffer = equity_table[equity]->right.top();
			equity_table[equity]->left.push(buffer);
			equity_table[equity]->right.pop();
			equity_table[equity]->right.push(trade_price);
		}
		else { equity_table[equity]->left.push(trade_price); }

		equity_table[equity]->median = (equity_table[equity]->left.top() + equity_table[equity]->right.top())/2;
	}
}

void StockMarket::processInsider(string equity)
{
	if (client_table.find("INSIDER_"+equity) != client_table.end())
	{
		if (equity_table[equity]->median!=-1 && !equity_table[equity]->sell.empty() && 9*equity_table[equity]->median > 10*equity_table[equity]->sell.top()->price) 
		{
			equity_table[equity]->buy.push(new order_t(order_num, "INSIDER_"+equity, equity_table[equity]->sell.top()->price, equity_table[equity]->sell.top()->share);
			processTrade(equity);
			order_num++;
		}
		if (equity_table[equity]->median!=-1 && !equity_table[equity]->buy.empty() && 10*equity_table[equity]->buy.top()->price>11*equity_table[equity]->median) 
		{
			equity_table[equity]->sell.push(new order_t(order_num, "INSIDER_"+equity, equity_table[equity]->buy.top()->price, equity_table[equity]->buy.top()->share);
			processTrade(equity);
			order_num++;
		}
	}
}

void StockMarket::printMedian(void)
{
	for (auto index:equity_table)
	{
		if (index->median != -1)
		{
			cout<<"Median match price of "<<index.first<<" at time "<<currTime<<" is $"<<index.second->median << "\n";
		}
	}
}

void StockMarket::printSummary(void)
{
	cout << "Commission Earnings: $" << summary_.commission_ << "\n";
	cout << "Total Amount of Money Transferred: $" << summary_.moneyTransfered_ << "\n";
	cout << "Number of Completed Trades: " << summary_.complete_trades_ << "\n";
	cout << "Number of Shares Traded: " << summary_.complete_shares_trades_ << "\n";
}

void StockMarket::printTransfers(void)
{
	for (auto index:client_table) 
	{
		cout << index.first << " bought " << index.second->Buy << " and sold " << index.second->Sell << " for a net transfer of $" << index.second->Transfer << "\n";
	}
}

void StockMarket::printTimeTransferTrade(void)
{
	for (auto index:command_.timeTravlTradingEquity)
	{
		if (equity_table.find(index) != equity_table.end())
		{
			processTTT(index);
		}
	}
}

void StockMarket::processTTT(string equity)
{
	vector<BigInt> price_record = equity_table[equity]->equity_price_record;
	vector<BigInt> timestamp_record = equity_table[equity]->order_timestamp_record;

	BigInt minPrice = Integer.MAX_VALUE;
	BigInt maxProfit = 0;
	int buy_index = -1;
	int buy_index_buffer = -1;
	int sell_index = -1;
	int index = 0;

	while(index < price_record.size()) 
	{
		if (price_record[index]<minPrice)
		{
			minPrice = price_record[index];
			buy_index_buffer = index;
		}
		else if (price_record[index]-minPrice>maxProfit)
		{
			maxProfit = price_record[index]-minPrice;
			sell_index = index;
			buy_index = buy_index_buffer;
		}

	    index++;
	}

	cout << "Time travelers would buy " << i << " at time: " << order_timestamp_record[buy_index] << " and sell it at time: " << order_timestamp_record[sell_index]<<"\n";	
}
