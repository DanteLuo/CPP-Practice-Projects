#ifndef PROJECT_TWO_ZOMBIE_GROUP_H
#define PROJECT_TWO_ZOMBIE_GROUP_H

#include <iostream>
#include <fstream>
#include "binary_heap.h"
#include "poorman_priority_queue.h"
#include "pairing_heap.h"
#include "sorted_priority_queue.h"

using namespace std;

struct command_opt_t
{
	// mode params
	string 	container_type;
	bool 	isStatsOutput;
	int 	statsN;
	bool	isVerbose;
	string 	file_name;

	command_opt_t(void):
	isStatsOutput(false),
	isVerbose(false)
	{}
};

struct configuration_t
{
	int quiver_capacity;
	int random_seed;
	int Max_rand_dist;
	int Max_rand_speed;
	int Max_rand_damage;
	int player_health;
};

struct zombie
{
	string name_;
	int speed_;
	int dist_;
	int damage_;
	int ETA_;
	int rounds_created_;
	int rounds_killed_;
	int rounds_active_;

	zombie(void){}

	zombie(string name, int speed, int dist, int damage, int rounds):
	name_(name),
	speed_(speed),
	dist_(dist),
	damage_(damage),
	rounds_created_(rounds),
	rounds_killed_(0)
	{
		ETA_ = dist/speed;
	}
};

struct comp 
{
	bool operator()(const zombie *z1, const zombie *z2) const
	{
		if (z1->ETA_ == z2->ETA_) 
		{
			return z1->name_>z2->name_;
		} 
		else return z1->ETA_>z2->ETA_;
	}
};

struct comp_active_most_rounds
{
	bool operator()(const zombie *z1, const zombie *z2) const
	{
		if (z1->rounds_active_ == z2->rounds_active_) 
		{
			return z1->name_>z2->name_;
		} 
		else return z1->rounds_active_ < z2->rounds_active_;
	}
};

struct comp_active_least_rounds
{
	bool operator()(const zombie *z1, const zombie *z2) const
	{
		if (z1->rounds_active_ == z2->rounds_active_) 
		{
			return z1->name_>z2->name_;
		} 
		else return z1->rounds_active_ > z2->rounds_active_;
	}
};

class ZombieGroup
{
public:
	ZombieGroup(command_opt_t& command);

	void runTheGame(void);

	~ZombieGroup();

private:
	void updateActivateDist(void);

	void loadNewZombie(void);

	void checkDamage(void);

	void killZombie(void);

	void updateKilledRounds(void);

	void printRoundsNum(void);

	void printUpdatedZombie(int index, int movement);

	void printCreatedZombie(string name, int dist);

	void printKillingZombie(void);

	void printVictoryState(void);

	void printStatsN(void);

	void printZombiesActive(void);

	void printKilledZombies(void);

	void printActiveZombiesRank(void);

	bool getKillFlag(void){return (player_health_ <= 0 ? true:false);}

	bool getContinueFlag(void){return player_health_ > 0 && (!heap->empty() || !GameFile_.eof());}
	
	int rounds_;
	int current_round;
	int quiver_;
	int player_health_;
	int killing_index_; // recording which zombie killed the player -1 for not been killed.
	int name_counter;
	
	configuration_t config_;
	command_opt_t command_;
	ifstream GameFile_;

	vector<zombie*>	kill_indexes;
	vector<zombie*> zombie_group;
	eecs281priority_queue<zombie*, comp> *heap;
	eecs281priority_queue<zombie*, comp_active_most_rounds> *heap_most_rounds;
	eecs281priority_queue<zombie*, comp_active_least_rounds> *heap_least_rounds;
};

#endif // PROJECT_TWO_ZOMBIE_GROUP_H
