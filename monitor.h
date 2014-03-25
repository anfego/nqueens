#ifndef MONITOR_H
#define MONITOR_H 


// #include <iostream>       // std::cout
#include <vector>          // std::stack
#include <utility>
#include <pthread.h>
#include <iostream>
#include <stack>

using namespace std;

bool foo(vector<pair<int,int> > args, stack< vector <pair<int,int> > > returnArgs);

class monitor
{

public:
	int job_number;
	int maxIndex;
	int locked_threads;
	// pair <int,int> args;
	vector<pair<int,int> > args;
	
	int total_proc;
	bool endjob_flag;
	bool result;
	stack< vector <pair<int,int> > > poolStack;
	stack< vector <pair<int,int> > > returnArgs;
	
	//initialize mutex
	pthread_mutex_t buffer_mutex;
	
	//initialize cond mutex
	pthread_mutex_t condMutex;
	pthread_cond_t cond;
	
	//initialize threads

	monitor(int passed_proc,  int gridSize, vector< pair<int,int> > * inputPull,pthread_mutex_t buffer_mutex_passed, pthread_mutex_t condMutex_passed, pthread_cond_t cond_passed);
	monitor();
	~monitor(){};
	void mon_init(int passed_proc, int gridSize, std::vector < std::pair<int,int> > * inputPull,pthread_mutex_t buffer_mutex, pthread_mutex_t condMutex, pthread_cond_t cond);
	int monitor_wait_in_que();
	void mon_enter(std::vector < std::pair<int,int> > * inputPull, bool haveWork);
	void mon_exit();
	void mon_continue(std::vector < std::pair<int,int> > * inputPull);

};

#endif