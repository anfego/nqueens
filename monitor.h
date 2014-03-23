// #include <iostream>       // std::cout
#include <vector>          // std::stack
#include <utility>
#include <pthread.h>

class monitor
{
	int job_number;
	int locked_threads;
	// pair <int,int> args;
	vector<pair<int,int> > *args;
	int total_proc;
	bool endjob_flag ;
	bool result
	vector<pair<int,int> > * pullPtr;
	vector<pair<int,int> > *result;
	//initialize mutex
	pthread_mutex_t buffer_mutex;
	//initialize cond mutex
	pthread_mutex_t condMutex;
	pthread_cond_t cond;
	//initialize threads
public:
	

	monitor(arguments){
		mon_init(int passed_proc, vector< pair<int,int> > * inputPull);
	};
	~monitor(){};
	
	void monitor_init(int passed_proc, std::vector < std::pair<int,int> > * inputPull);



	void monitor_wait_in_que();
	void mon_enter();
	void mon_exit();

	void mon_continue();
};


void monitor::monitor_wait_in_que()
{
	return locked_threads;
}
void monitor::mon_enter()
{
    pthread_mutex_lock(&buffer_mutex);//free lock
	// conditional mutex lock(queue_is_empty())
	if(result)
	{
		pullPtr.push(returnArgs);
		job_number++;
	}
	
	if(job > 0)
	{
		args = pullPtr.pop_back();
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
		args = pullPtr.pop_back();
		job_number--;
	}
}