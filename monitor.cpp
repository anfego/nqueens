
// #include <iostream>       // std::cout
#include <vector>          // std::stack
#include <utility>
#include <pthread.h>
#include <iostream>
#include <stack>

#include "monitor.h"

using namespace std;

bool foo(vector<pair<int,int> > args, stack< vector <pair<int,int> > > returnArgs);

void printSolutionVector1(std::vector< std::pair<int,int> >  * solutionVector);

monitor::monitor()
{
	cerr << "ERROR";
	return;
	//mon_init(int passed_proc, vector< pair<int,int> > * inputPull);
}

monitor::monitor(int passed_proc,  int gridSize, vector< pair<int,int> > * inputPull, pthread_mutex_t buffer_mutex_passed, pthread_mutex_t condMutex_passed, pthread_cond_t cond_passed)
{
	cout << "Monitor constructor" << endl;
	monitor::mon_init(passed_proc, gridSize, inputPull, buffer_mutex_passed, condMutex_passed, cond_passed);
}

void monitor::mon_init(int passed_proc, int gridSize, std::vector < std::pair<int,int> > * inputPull,pthread_mutex_t buffer_mutex_passed, pthread_mutex_t condMutex_passed, pthread_cond_t cond_passed)
{
	cout << "Monitor init" << endl;
	maxIndex = gridSize;
	for (int i = 0; i < maxIndex; ++i)
	{
		printSolutionVector1(inputPull+i);
		poolStack.push(*(inputPull+i));
		
		// for ( std::vector<std::pair<int,int> >::iterator i = (*inputPull).begin() ; i != (*inputPull[i]).end(); ++i)
		// {
		// 	std::vector< pair<int,int> > tempVector = *inputPull;
	
	}
		  	
	// poolStack.push(*inputPull);
	job_number = poolStack.size();
	cout << "INITIAL WORK: "<<job_number <<endl;
	locked_threads = 0;
	
	// pair <int,int> args;
	//args = new vector< pair<int,int> >;
	total_proc = passed_proc;
	endjob_flag = false;
	result = false;
	// returnArgs = new vector<pair<int,int> >;
	//initialize mutex
	buffer_mutex = buffer_mutex_passed;
	//initialize cond mutex
	condMutex = condMutex_passed;
	cond = cond_passed;
	//initialize threads
}
int monitor::monitor_wait_in_que()
{
	return locked_threads;
}

void monitor::mon_enter(std::vector < std::pair<int,int> > * inputPull, bool haveWork)
{
    pthread_mutex_lock(&buffer_mutex);//free lock
	// cout << "Monitor enter" << endl;
	// conditional mutex lock(queue_is_empty())
	if(haveWork)
	{
		for (int i = 0; i < maxIndex; ++i)
		{
			// cout << "Stacking Data" << endl;
			poolStack.push(*(inputPull+i));
		}
			
	}

	
	if(poolStack.size() > 0)
	{
		// cout << "\tGetting Data" << endl;
		for (int i = 0; i < maxIndex; ++i)
		{
			inputPull[i] = poolStack.top() ;
			
		}
		poolStack.pop() ;
		// printSolutionVector1(inputPull);
	}
	else
	{
		cout << "\t\tNO Data" << endl;
		//conditional mutex k()
		locked_threads++;
		if(locked_threads == total_proc)
			endjob_flag = true;
		else
		{
			cout<<"\t Thread is Locked\n";
	        pthread_mutex_unlock(&buffer_mutex);//free lock
        	pthread_cond_wait(&cond, &condMutex); // unlock & sleep; wake up when signaled & lock again
        	mon_continue();
        }
    }

}

void monitor::mon_exit()
{
	// cout << "Monitor EXIT" << endl;
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

	// result = foo(args, returnArgs);
}

void monitor::mon_continue()
{
    // pthread_mutex_lock(&buffer_mutex);//free lock
    cout << "Monitor continue" << endl;
    if(job_number > 0)
	{
		args = poolStack.top() ;
		poolStack.pop() ;
		job_number--;
	}
}

bool foo(vector<pair<int,int> > args, stack< vector <pair<int,int> > > returnArgs)
{
	cout << "FOO func" << endl;
	//returnArgs = NULL;
	return true;
}
void printSolutionVector1(std::vector< std::pair<int,int> >  * solutionVector)
{
	for (std::vector<std::pair<int,int> >::iterator i = (*solutionVector).begin() ; i != (*solutionVector).end(); ++i)
    	std::cout << ' ' << (*i).first << ' '<< (*i).second;
  	std::cout << '\n';

}
	