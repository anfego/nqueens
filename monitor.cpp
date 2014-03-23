monitor_init(int passed_proc)
{
	//create stack
	stack <(<int>stack)> check = new <(<int>stack)>stack;
	int job_number = 0;
	int locked_threads = 0;
	stack <int> args;
	total_proc = passed_proc;
	bool endjob_flag = false;
	//initialize mutex
	//initialize cond mutex
	//initialize threads
}

monitor_wait_in_que()
{
	return locked_threads;
}
mon_enter()
{
    pthread_mutex_lock(&buffer_mutex);//free lock
	// conditional mutex lock(queue_is_empty())
	if(result)
	{
		check.push(result);
		job_number++;
	}
	
	if(job > 0)
	{
		args = check.pop();
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
        	pthread_cond_wait(&cond, &buffer_mutex); // unlock & sleep; wake up when signaled & lock again
        }
    }
}

mon_exit()
{
	if((locked_threads > 0 && job>0 )|| endjob_flag)
	{
		pthread_cond_signal(&cond);
		locked_threads--;
        // pthread_mutex_unlock(&buffer_mutex);//free lock
	}
	else
	{
        pthread_mutex_unlock(&buffer_mutex);//free lock
	}

	result = foo(args);
}

mon_continue()
{
    // pthread_mutex_lock(&buffer_mutex);//free lock
    if(job_number > 0)
	{
		args = list.pop();
		job_number--;
	}
}