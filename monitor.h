// #include <iostream>       // std::cout
#include <stack>          // std::stack
/*
 monitor_t m;
    mon_init(&m,num_ranks_participating);
    mon_enter(&m);
    mon_exit(&m);
    mon_continue(&m);
    mon_wait_in_q(&m);
    mon_num_in_q(&m);

Note that these could be used to implement higher level monitors, e.g.
    Simple lock and unlock:
        mon_enter(&m);
        mon_exit(&m);
    Barrier:
        mon_enter(&m);
        if (mon_num_in_q(&m) < (num_ranks_participating-1))
            mon_wait_in_q(&m);
        mon_continue(&m);
*/
class monitor
{
	//private
	pthread_t *threadArray;
	numthreads; 
	pthread_mutex_t p_mutex_1;
	int mutex_resp;
	std::stack<int> my_stack;//store index of thread
	bool jobDone;

public:
	monitor(arguments){mon_init(&m,num_ranks_participating);};
	~monitor();

	/* data */
	//initializate monitors
	void mon_init(num_ranks_participating);
	// enter a process to the "dungeon"
	void mon_enter();
	//exit a process from the "dungeon"
	void mon_exit();
	// 
	void mon_continue();
	// add a process to a waiting queue
	void mon_wait_in_q();
	// returns the number of proccess in queue
	int mon_num_in_q();
};

