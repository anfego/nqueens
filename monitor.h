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

	monitor(int passed_proc, vector< pair<int,int> > * inputPull);
	monitor();
	~monitor(){};
	void mon_init(int passed_proc, std::vector < std::pair<int,int> > * inputPull);
	int monitor_wait_in_que();
	void mon_enter();
	void mon_exit();
	void mon_continue();
};

#endif