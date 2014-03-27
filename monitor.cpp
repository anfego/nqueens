
// #include <iostream>       // cout
#include <vector>          // stack
#include <utility>
#include <pthread.h>
#include <iostream>
#include <stack>

#include "monitor.h"

using namespace std;




monitor::monitor()
{
	
}

monitor::monitor(int passed_proc, bool quietFlag, int gridSize, vector< pair<int,int> > * inWork, pthread_mutex_t buffer_mutex_passed, pthread_mutex_t condMutex_passed, pthread_cond_t cond_passed)
{
	
	monitor::monitor_init(passed_proc, quietFlag, gridSize, inWork, buffer_mutex_passed, condMutex_passed, cond_passed);
}

void monitor::monitor_init(int passed_proc, bool quietFlag, int gridSize, vector < pair<int,int> > * inWork,pthread_mutex_t buffer_mutex_passed, pthread_mutex_t condMutex_passed, pthread_cond_t cond_passed)
{
	
	maxIndex = gridSize;
	for (int i = 0; i < maxIndex; ++i)
	{
		poolStack.push(*(inWork+i));
	
	}
	
	locked_threads = 0;
	
	total_proc = passed_proc;
	allWorkDone = false;

	quiet = quietFlag;
	
	//initialize mutex
	backDoorMutex = PTHREAD_MUTEX_INITIALIZER;
	jobPoolMutex = PTHREAD_MUTEX_INITIALIZER;
	frontDoorMutex = buffer_mutex_passed;
	
	//initialize cond mutex
	condMutex = condMutex_passed;
	cond = cond_passed;

}
void monitor::monitor_wait_in_que()
{
	locked_threads++;
	pthread_mutex_trylock(&condMutex);

	pthread_cond_wait(&cond, &condMutex); 
	locked_threads--;
	
	pthread_mutex_lock(&backDoorMutex);
}

int monitor::monitor_enter(vector < pair<int,int> > * inWork, bool haveWork)
{
    pthread_mutex_lock(&frontDoorMutex);
	int gotWork = 0;
	if(haveWork)
	{
		for (int i = 0; i < maxIndex; ++i)
		{
			
			poolStack.push(*(inWork+i));
		}
			
	}

	
	if(poolStack.size() > 0)
	{
		
		
		monitor_continue(inWork);
		monitor_exit();
		gotWork = 1;
		
	}
	else
	{
		if(locked_threads >= (total_proc-1))
		{
			
			allWorkDone = true;
			monitor_exit();
			gotWork = 0;
			
		}
		else
		{
			pthread_mutex_unlock(&frontDoorMutex);
			
			monitor_wait_in_que();
        	
        	gotWork = monitor_continue(inWork);
        	
        	monitor_exit();

        	

        }
    }
    return gotWork;

}

void monitor::monitor_exit()
{
	if( locked_threads > 0)
	{
		
		
		pthread_mutex_unlock(&condMutex);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&backDoorMutex);
		return;
	}
	else if(locked_threads == 0)
	{

		pthread_mutex_unlock(&backDoorMutex);
	}
	pthread_mutex_unlock(&frontDoorMutex);


	
}

int monitor::monitor_continue(vector < pair<int,int> > * inWork)
{
    

    pthread_mutex_lock(&jobPoolMutex);

    int gotWork = 0;
    if(!allWorkDone)
	{
		if(poolStack.size() > 0)
		{
			inWork[0] = poolStack.top() ;
			poolStack.pop() ;
			gotWork = 1;
		}
	}
    pthread_mutex_unlock(&jobPoolMutex);
	return gotWork;
}

monitor::~monitor()
{
	pthread_mutex_destroy(&backDoorMutex);
	pthread_mutex_destroy(&frontDoorMutex);
	pthread_mutex_destroy(&jobPoolMutex);
}

