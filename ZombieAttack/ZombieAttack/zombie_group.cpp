#include "zombie_group.h"

using namespace std;

ZombieGroup::ZombieGroup(command_opt_t& command):
rounds_(0),
current_round(0),
killing_index_(-1),
name_counter(1)
{
	// init command
	command_ = command;

	// init heap
    if (command.container_type == "BINARY") 
    {
		heap = new binary_heap<zombie*, comp>;
	} 
	else if (command.container_type == "POOR_MAN") 
	{
		heap = new poorman_priority_queue<zombie*, comp>;
	}
	else if (command.container_type == "SORTED") 
	{
		heap = new sorted_priority_queue<zombie*, comp>;
	}
	else if (command.container_type == "PAIRING") 
	{
		heap = new pairing_heap<zombie*, comp>;
	}

	// init GameFile
	GameFile_.open(command.file_name);
    
    // init Config
    string param_name;
	GameFile_>>param_name>>config_.quiver_capacity;
    GameFile_>>param_name>>config_.random_seed;
    GameFile_>>param_name>>config_.Max_rand_dist;
    GameFile_>>param_name>>config_.Max_rand_speed;
    GameFile_>>param_name>>config_.Max_rand_damage;
    GameFile_>>param_name>>config_.player_health;

    player_health_ = config_.player_health;

    // init rand
    srand(config_.random_seed);
}

ZombieGroup::~ZombieGroup(void)
{
	for (auto i:zombie_group)
	{
		delete i;
	}

	delete heap;

	if (command_.isStatsOutput)
	{
		delete heap_most_rounds;
		delete heap_least_rounds;
	}
}

void ZombieGroup::runTheGame(void)
{
	while(getContinueFlag()) 
	{
		rounds_++;

		// update quiver num
	    quiver_ = config_.quiver_capacity;

	    // verbose print
	    if (command_.isVerbose)
	    {
	    	printRoundsNum();
	    }

	    // update zombie dist at the starting of a round
	    if (!zombie_group.empty())
	    {
	    	updateActivateDist();
	    	checkDamage();
	    }

	    // create new zombies
	    loadNewZombie();

	    // check player health
	    if (player_health_<=0) 
	    {
	    	printKillingZombie();
	    	break;
	    }

	    // shoots zombies
	    while(quiver_ > 0 && !heap->empty()) 
	    {
	     	if (!heap->empty()){killZombie();}
	        quiver_--;
	    }
	}

	updateKilledRounds();

	if (player_health_>0)
	{
		printVictoryState();
	}

	if (command_.isStatsOutput)
	{
		printStatsN();
	}
}

void ZombieGroup::updateActivateDist(void)
{
	uint index = 0;
	while(index < zombie_group.size()) 
	{
	    if (zombie_group[index]->rounds_killed_ != 0)
	   	{
	   		index++;
	    }
	    else
	    {
	    	int random_offset = (rand() % zombie_group[index]->speed_)/2;
	    	int movement = zombie_group[index]->speed_ + random_offset;
	    	if (movement>zombie_group[index]->dist_)
	    	{
	    		zombie_group[index]->dist_ = 0;
	    		zombie_group[index]->ETA_ = 0;
	    	}
	    	else 
	    	{
	    		zombie_group[index]->dist_ -= movement;
	    		zombie_group[index]->ETA_ = zombie_group[index]->dist_/zombie_group[index]->speed_;
	    	}

	    	// check verbose print
	    	if (command_.isVerbose)
	    	{
	    		printUpdatedZombie(index, movement);
	    	}

	    	index++;
	    }
	}

	return;
}

void ZombieGroup::loadNewZombie(void)
{
	string char_buffer;
	int random_zombie;
	int explicit_zombie;

	if (!GameFile_.eof() && current_round == 0)
	{
		GameFile_>>char_buffer;
		GameFile_>>char_buffer>>current_round;
	}

	if (current_round == rounds_)
	{
		GameFile_>>char_buffer>>random_zombie;
		GameFile_>>char_buffer>>explicit_zombie;

		for (int i = 0; i<random_zombie; i++) 
		{
			int dist = rand() % config_.Max_rand_dist + 1;
			int speed = rand() % config_.Max_rand_speed + 1;
			int damage = rand() % config_.Max_rand_damage + 1;
			string name = "AI" + to_string(name_counter++);
			zombie* new_zombie = new zombie(name, speed, dist, damage, rounds_);
			zombie_group.push_back(new_zombie);
			heap->push(new_zombie);

			if (command_.isVerbose)
			{
				printCreatedZombie(name, dist);
			}
		}

		for (int i = 0; i<explicit_zombie; i++) 
		{
	    	int dist;
	    	int speed;
	    	int damage;
	    	string name;
	    	GameFile_ >> dist >> speed >> damage >> name;
	    	zombie* new_zombie = new zombie(name, speed, dist, damage, rounds_);
			zombie_group.push_back(new_zombie);
			heap->push(new_zombie);

			if (command_.isVerbose)
			{
				printCreatedZombie(name, dist);
			}
	    }

	    current_round = 0;
	}
	else {return;}
}

void ZombieGroup::checkDamage(void)
{
	uint index = 0;
	while(index < zombie_group.size()) 
	{
		if (zombie_group[index]->dist_ == 0 && zombie_group[index]->rounds_killed_ != 0)
		{
			player_health_ -= zombie_group[index]->damage_;
			if (player_health_<=0)
			{
				killing_index_ = index;
				return;
			}
		}
		index++;
	}
}

void ZombieGroup::killZombie(void)
{
	cout<<heap->size()<<"\n";
	zombie* killed = heap->top();
	// cout<<"Killed Zombie "<<killed->name_<<"\n";
	killed->rounds_killed_ = rounds_;
	killed->rounds_active_ = killed->rounds_killed_-killed->rounds_created_+1;
	kill_indexes.push_back(killed);
	heap->pop();

	return;
}

void ZombieGroup::updateKilledRounds(void)
{
	uint index = 0;
	while(index < zombie_group.size()) 
	{
	    if (zombie_group[index]->rounds_killed_ == 0)
	    {
	    	zombie_group[index]->rounds_killed_ = rounds_;
	    	zombie_group[index]->rounds_active_ = rounds_-zombie_group[index]->rounds_created_+1;
	    }
	    index++;
	}
}

void ZombieGroup::printRoundsNum(void)
{
	cout<<"Rounds: "<<rounds_<<"\n";
}

void ZombieGroup::printUpdatedZombie(int index, int movement)
{
	cout<<"Moved: "<<zombie_group[index]->name_<<" "<<movement<<"\n";
}

void ZombieGroup::printCreatedZombie(string name, int dist)
{
	cout<<"Created: "<<name<<" "<<dist<<"\n";
}

void ZombieGroup::printKillingZombie(void)
{
	cout<<"DEFEAT IN ROUND "<<rounds_<<"! "<<zombie_group[killing_index_]->name_<<" ate your brains!\n";
}

void ZombieGroup::printVictoryState(void)
{
	cout<<"VICTORY IN ROUND "<<rounds_<<"! "<<kill_indexes.back()->name_<<" was the last zombie. You survived with "<<player_health_<<" health left.\n";
}

void ZombieGroup::printStatsN(void)
{
	printZombiesActive();

	printKilledZombies();

	printActiveZombiesRank();
}

void ZombieGroup::printZombiesActive(void)
{
	cout<<"Zombies still active: "<<zombie_group.size()-kill_indexes.size()<<"\n";
}

void ZombieGroup::printKilledZombies(void)
{
	int index = 0;
	int kill_indexes_size = kill_indexes.size();
	cout<<"First zombies killed:\n";

	while(index < min(command_.statsN, kill_indexes_size)) 
	{
	    cout<<kill_indexes[index]->name_<<" "<<index+1<<"\n";
	    index++;
	}

	int counter = min(command_.statsN, kill_indexes_size);
	index = kill_indexes.size()-1;
	cout<<"Last zombies killed:\n";

	while(index > max(0, kill_indexes_size-command_.statsN-1)) 
	{
	    cout<<kill_indexes[index]->name_<<" "<<counter<<"\n";
	    index--;
	    counter--;
	}
}

void ZombieGroup::printActiveZombiesRank(void)
{
	if (command_.container_type == "BINARY") 
	{
		heap_most_rounds = new binary_heap<zombie*, comp_active_most_rounds>;
		heap_least_rounds = new binary_heap<zombie*, comp_active_least_rounds>;
	} 
	else if (command_.container_type == "POOR_MAN") 
	{
		heap_most_rounds = new poorman_priority_queue<zombie*, comp_active_most_rounds>;
		heap_least_rounds = new poorman_priority_queue<zombie*, comp_active_least_rounds>;
	}
	else if (command_.container_type == "SORTED") 
	{
		heap_most_rounds = new sorted_priority_queue<zombie*, comp_active_most_rounds>;
		heap_least_rounds = new sorted_priority_queue<zombie*, comp_active_least_rounds>;
	}
	else if (command_.container_type == "PAIRING") 
	{
		heap_most_rounds = new pairing_heap<zombie*, comp_active_most_rounds>;
		heap_least_rounds = new pairing_heap<zombie*, comp_active_least_rounds>;
	}
	
	uint index = 0;
	int kill_indexes_size = zombie_group.size();
	while(index < zombie_group.size()) 
	{
	    heap_most_rounds->push(zombie_group[index]);
	    heap_least_rounds->push(zombie_group[index]);
	    index++;
	}

	int print_index = 0;
	cout<<"Most active zombies:\n";
	while(print_index < min(command_.statsN,kill_indexes_size)) 
	{
	 	cout<<heap_most_rounds->top()->name_<<" "<<heap_most_rounds->top()->rounds_active_<<"\n";
	 	heap_most_rounds->pop();
	 	print_index++;
	}

	print_index = 0;
	cout<<"Least active zombies:\n";
	while(print_index < min(command_.statsN,kill_indexes_size)) 
	{
	 	cout<<heap_least_rounds->top()->name_<<" "<<heap_least_rounds->top()->rounds_active_<<"\n";
	 	heap_least_rounds->pop();
	 	print_index++;
	}
}

