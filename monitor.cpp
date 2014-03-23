
// #include <iostream>       // std::cout
#include <vector>          // std::stack
#include <utility>
#include <pthread.h>
#include <iostream>
#include <stack>

#include "monitor.h"

using namespace std;

bool foo(vector<pair<int,int> > args, stack< vector <pair<int,int> > > returnArgs);

monitor::monitor()
{
	cerr << "ERROR";
	return;
	//mon_init(int passed_proc, vector< pair<int,int> > * inputPull);
}

monitor::monitor(int passed_proc, vector< pair<int,int> > * inputPull)
{
	monitor::mon_init(passed_proc, inputPull);
}

void monitor::mon_init(int passed_proc, std::vector < std::pair<int,int> > * inputPull)
{
	job_number = 0;
	locked_threads = 0;
	
	// pair <int,int> args;
	//args = new vector< pair<int,int> >;
	total_proc = 0;
	endjob_flag = false;
	result = false;
	// returnArgs = new vector<pair<int,int> >;
	//initialize mutex
	buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
	//initialize cond mutex
	condMutex = PTHREAD_MUTEX_INITIALIZER;
	cond = PTHREAD_COND_INITIALIZER;
	//initialize threads
}
int monitor::monitor_wait_in_que()
{
	return locked_threads;
}

void monitor::mon_enter()
{
    pthread_mutex_lock(&buffer_mutex);//free lock
	// conditional mutex lock(queue_is_empty())
	if(result)
	{
		while(returnArgs.size() > 0)
		{			
			poolStack.push(returnArgs.top());
			returnArgs.pop();
			job_number++;
		}
	}
	
	if(job_number > 0)
	{
		args = poolStack.top() ;
		poolStack.pop() ;
		job_number--;
	}
	else
	{
		//conditional mutex k()
		locked_threads++;
		
		if(locked_threads == total_proc)
			endjob_flag = true;
		else
		{
	        pthread_mutex_unlock(&buffer_mutex);//free lock
        	pthread_cond_wait(&cond, &condMutex); // unlock & sleep; wake up when signaled & lock again
        }
    }
}

void monitor::mon_exit()
{
	if((locked_threads > 0 && job_number > 0 )|| endjob_flag)
	{
		pthread_cond_signal(&cond);
		locked_threads--;
        // pthread_mutex_unlock(&buffer_mutex);//free lock
	}
	else
	{
        pthread_mutex_unlock(&buffer_mutex);//free lock
	}

	result = foo(args, returnArgs);
}

void monitor::mon_continue()
{
    // pthread_mutex_lock(&buffer_mutex);//free lock
    if(job_number > 0)
	{
		args = poolStack.top() ;
		poolStack.pop() ;
		job_number--;
	}
}

bool foo(vector<pair<int,int> > args, stack< vector <pair<int,int> > > returnArgs)
{
	//returnArgs = NULL;
	return true;
}
