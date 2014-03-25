
// #include <iostream>       // std::cout
#include <vector>          // std::stack
#include <utility>
#include <pthread.h>
#include <iostream>
#include <stack>

#include "monitor.h"

using namespace std;


void printSolutionVector1(std::vector< std::pair<int,int> >  * solutionVector);

monitor::monitor()
{
	cerr << "ERROR";
	return;
	//mon_init(int passed_proc, vector< pair<int,int> > * inputPull);
}

monitor::monitor(int passed_proc, bool quietFlag, int gridSize, vector< pair<int,int> > * inputPull, pthread_mutex_t buffer_mutex_passed, pthread_mutex_t condMutex_passed, pthread_cond_t cond_passed)
{
	cout << "Monitor constructor" << endl;
	monitor::mon_init(passed_proc, quietFlag, gridSize, inputPull, buffer_mutex_passed, condMutex_passed, cond_passed);
}

void monitor::mon_init(int passed_proc, bool quietFlag, int gridSize, std::vector < std::pair<int,int> > * inputPull,pthread_mutex_t buffer_mutex_passed, pthread_mutex_t condMutex_passed, pthread_cond_t cond_passed)
{
	//cout << "Monitor init" << endl;
	maxIndex = gridSize;
	for (int i = 0; i < maxIndex; ++i)
	{
	//	printSolutionVector1(inputPull+i);
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
	quiet = quietFlag;
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
		// mon_exit();
		
		mon_continue(inputPull);
		// for (int i = 0; i < maxIndex; ++i)
		// {
		// 	inputPull[i] = poolStack.top() ;
			
		// }
		// poolStack.pop() ;
		// // if(locked_threads > 0)
		mon_exit();
		// printSolutionVector1(inputPull);
	}
	else
	{
		// cout << "\t\tNO Data" << endl;
		//conditional mutex k()
		if(locked_threads >= (total_proc-1))
		{
			cout<<"\t FINISHED!!!!\n";
			endjob_flag = true;
			mon_exit();
			return;
		}
		else
		{
			// cout<<"\t Thread is Locked\n";
			locked_threads++;
			// cout<<"\t Thread is Locked: "<<locked_threads<<endl;
			pthread_mutex_unlock(&buffer_mutex);//free lock
			
			pthread_mutex_trylock(&condMutex);//free lock
        	
        	pthread_cond_wait(&cond, &condMutex); // unlock & sleep; wake up when signaled & lock again
			locked_threads--;
			// cout<<"\t\t Thread is UNLocked: "<<locked_threads<<endl;
        	mon_continue(inputPull);
        	mon_exit();
        	

        }
    }

}

void monitor::mon_exit()
{
	// if(endjob_flag && locked_threads >= (total_proc-1))
	// {
	// 	cout<<"BroadCast Signal";
	// 	pthread_cond_broadcast(&cond);
	// 	pthread_mutex_unlock(&condMutex);
	
	// }else 
	if( locked_threads > 0)
	{
		
		// cout<<"Single Signal\n";
		pthread_mutex_unlock(&condMutex);
		pthread_cond_signal(&cond);
		return;
		// pthread_mutex_unlock(&condMutex);
		// pthread_mutex_unlock(&buffer_mutex);//free lock
	}
	else if(locked_threads == 0)
	{
		// cout<<"No locked_threads\n";
		if(endjob_flag)
		{
			cout<<"Last ONE\n";
			/* code */
		}

		
	}
	pthread_mutex_unlock(&buffer_mutex);


	// result = foo(args, returnArgs);
}

void monitor::mon_continue(std::vector < std::pair<int,int> > * inputPull)
{
    // pthread_mutex_lock(&buffer_mutex);//free lock
	//    cout << "Monitor continue" << endl;
	if(!endjob_flag)
	{

		if(poolStack.size() > 0)
		{
			// cout << "\tGetting Data" << endl;
			for (int i = 0; i < maxIndex; ++i)
			{
				inputPull[i] = poolStack.top() ;
				
			}
			poolStack.pop() ;
		}
	}
}

void printSolutionVector1(std::vector< std::pair<int,int> >  * solutionVector)
{
	for (std::vector<std::pair<int,int> >::iterator i = (*solutionVector).begin() ; i != (*solutionVector).end(); ++i)
    	std::cout << ' ' << (*i).first << ' '<< (*i).second;
  	std::cout << '\n';

}
	
