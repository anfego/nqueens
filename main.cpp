/*This program is a pthreaded solution for nqueens problem
Author:	Andres F. Gomez
Date: 	03/22/2014
Class: 	Dr. Buttler 6430 Paralell Processing
Arguments:
	-n  size of board (N)
	-q  quiet (prints number of solutions found but *not* the solutions)
	-p  number of processes or threads to do the work (may be 1 - 16)

*/


	#include <iostream>
	#include <utility>
	#include <string.h>
	#include <stdlib.h>
	#include <vector>
	#include <pthread.h>


	#include "monitor.h"

	using namespace std;

	void createWork(int gridSize, std::vector<std::pair<int,int> > * solutionVector);

	void printSolutionVector(std::vector< std::pair<int,int> > * solutionVector);

	void printALLSolutionVectors(std::vector< std::pair<int,int> > * solutionVector);
	
	bool isPosibleSolution(std::vector< std::pair<int,int> > * solutionVector);
	
	bool checkRow(std::vector<std::pair<int,int> > * solutionVector);
	
	bool checkDiagHigh(std::vector<std::pair<int,int> > * solutionVector);
	
	bool checkDiagLow(std::vector<std::pair<int,int> > * solutionVector);
	
	void createInitalWork(int gridSize, std::vector< std::pair<int,int> > * vectorPtr );
	
	void * processFunc(void *p);

	
	
	int procNum;
	
	vector <pair<int,int> > *poolStack;


	pthread_mutex_t buffer_mutex;
	
	//initialize cond mutex
	pthread_mutex_t condMutex;
	pthread_cond_t cond;
	

	int main(int argc, char const *argv[])
	{
		int gridSize = 4;
		int procNum = 1;
		bool quiet = false;
		int val;
		void *ret = &val;


		if(argc < 5)
		{
			cerr << "Number of Arguments incorrect";
			return 0;
		}
		if(!strcmp(argv[1],"-n")){
			gridSize = atoi(argv[2]);
			cout<<gridSize<<"\n";
		}
		if(!strcmp(argv[3],"-p"))
			procNum = atoi(argv[4]);

		if(!strcmp(argv[5],"-q"))
			quiet = true;
		
		std::vector< std::pair<int,int> > * vectorPtr;
		vectorPtr = new ( std::vector< std::pair<int,int> > [gridSize] );
		pair<int,int> pairPtr;
		pairPtr.first = -1;
		pairPtr.second = 0;

		// vectorPtr[0].push_back( pairPtr );
		createInitalWork(gridSize, vectorPtr);
		// createWork(gridSize, vectorPtr);
		
		// for (int i = 0; i < gridSize; ++i)
		// {
		// 	printSolutionVector(&vectorPtr[i]);
		// }
		
		pthread_t threads[procNum];
		
		procNum = procNum;
		poolStack = vectorPtr;
		buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
		
		//initialize cond mutex
		condMutex = PTHREAD_MUTEX_INITIALIZER;
		cond = PTHREAD_COND_INITIALIZER;

		// // creates a vector "stack" for each row
		monitor mon(procNum, gridSize, vectorPtr, buffer_mutex, condMutex, cond);
		

		for (int i = 0; i < procNum-1; ++i)
		{
			if (pthread_create(&threads[i], NULL, processFunc, &mon)) 
			{
				perror("pthread_create: ");
				exit(1);
			}
		}

		processFunc(&mon);
	//monitor * mon = new monitor(procNum,vectorPtr);
		  
		for (int i = 0; i < procNum; ++i)
		{	  
			pthread_join(threads[i], &ret);
		}




		return 0;
	}


	void printSolutionVector(std::vector< std::pair<int,int> >  * solutionVector)
	{
	  	std::cout << "\n";
		for (std::vector<std::pair<int,int> >::iterator i = (*solutionVector).begin() ; i != (*solutionVector).end(); ++i)
	    	std::cout << ' ' << (*i).first << ' '<< (*i).second<<'|';

	}
	
	void printALLSolutionVectors(std::vector< std::pair<int,int> >  * solutionVector)
	{


	}
	


	void createInitalWork(int gridSize, std::vector< std::pair<int,int> > * vectorPtr )
	{
		
		// std::vector< std::pair<int,int> > * vectorPtr;
		

		for (int i = 0; i < gridSize; ++i)
		{
			pair<int,int> * pairPtr = new std::pair<int,int>;
			(*pairPtr).first = 0;
			(*pairPtr).second = i;

			vectorPtr[i].push_back( *pairPtr );
			// printSolutionVector(&vectorPtr[i]);
			delete(pairPtr);
		}
	

	}
		// this funtion create work stack 
	// arguments:
	// 		size of the grid
	// 		location 
	void createWork(int gridSize, std::vector<std::pair<int,int> > * solutionVector)
	{

		std::vector<std::pair<int,int> >::iterator i = (*solutionVector).begin();
		int xCurrent = (*i).first;
		int yCurrent = (*i).second;
		
		cout<<"Create New work\n"; //<<xCurrent<<'|'<<yCurrent <<  endl;
		for (int i = 0; i < gridSize; ++i)
		{
			// check if solutionVector is a solution xCurrent = gridSize
			if(xCurrent == (gridSize-1))
			{
				cout<<"\t\t\tsolution found!!!!"<< endl;
				return;
			}
			pair<int,int> newPair;
			newPair.first = xCurrent + 1;
			newPair.second = i;
			// pair<int,int> * pairPtr = new std::pair<int,int>;
			// (*pairPtr).first = xCurrent+1;
			// (*pairPtr).second = i;
			solutionVector[i].push_back( newPair );
			printSolutionVector(&solutionVector[i]);
			

			
			// delete(pairPtr);
		}
	
	}
	bool isPosibleSolution(std::vector<std::pair<int,int> > * solutionVector)
	{
		printSolutionVector(solutionVector);
		if (checkRow(solutionVector) && checkDiagHigh(solutionVector) && checkDiagLow(solutionVector))
			return true;
		return false;
	}
	bool checkRow(std::vector<std::pair<int,int> > * solutionVector)
	{
		
		
		int xCurrent = (*solutionVector).back().first;
		int yCurrent = (*solutionVector).back().second;
		// pop laststd::pair to avoid to compare

		(*solutionVector).pop_back();
		
		for (std::vector<std::pair<int,int> >::iterator i = (*solutionVector).begin() ; i != (*solutionVector).end(); ++i)
	    {
			if((*i).first == xCurrent)
				return false;
		}
		// possible solution, addstd::pair back to the vector
		std::pair<int,int> temp(xCurrent,yCurrent);
		(*solutionVector).push_back(temp);
		return true;
	}
	bool checkDiagHigh(std::vector<std::pair<int,int> > * solutionVector)
	{

		
		int xCurrent = (*solutionVector).back().first;
		int yCurrent = (*solutionVector).back().second;
		// pop laststd::pair to avoid to compare

		(*solutionVector).pop_back();
		
		for (std::vector<std::pair<int,int> >::iterator i = (*solutionVector).begin() ; i != (*solutionVector).end(); ++i)
	    {

			for (int pos = 0; (yCurrent-pos) <= 0 ; ++pos)
			{
				if((*i).first == (xCurrent-pos) && (*i).second == (yCurrent-pos))
					return false;
			
			}
		}
		// possible solution, addstd::pair back to the vector
		std::pair<int,int> temp(xCurrent,yCurrent);
		(*solutionVector).push_back(temp);
		return true;
	}
	bool checkDiagLow(std::vector<std::pair<int,int> > * solutionVector)
	{
		
		int xCurrent = (*solutionVector).back().first;
		int yCurrent = (*solutionVector).back().second;
		// pop laststd::pair to avoid to compare

		(*solutionVector).pop_back();
		
		for (std::vector<std::pair<int,int> >::iterator i = (*solutionVector).begin() ; i != (*solutionVector).end(); ++i)
	    {

			for (int pos = 0; (xCurrent-pos) <= 0 ; ++pos)
			{
				if((*i).first == (xCurrent-pos) && (*i).second == (yCurrent+pos))
					return false;
			
			}
		}
		// possible solution, addstd::pair back to the vector
		std::pair<int,int> temp(xCurrent,yCurrent);
		(*solutionVector).push_back(temp);
		return true;
	}
	
	void * processFunc(void *p)
	{
		bool haveNewWork = false;
		monitor * mon =  (monitor *) p;
		int test = 4; 
		std::vector< std::pair<int,int> > * solutionVector;

		solutionVector = new  std::vector< std::pair<int,int> > [mon->maxIndex] ;
		
		while(!mon->endjob_flag)
		{
			mon->mon_enter(solutionVector, haveNewWork);
			// solutionVector[0] = mon->args;
			mon->mon_exit();
			// printSolutionVector(&solutionVector[0]);
			// if(!isPosibleSolution(&solutionVector[0]))
			if(test>=0)
			{
				cout<<"it's posible generate more now"<< endl;
				// std::vector<std::pair<int,int> >::iterator i = (*solutionVector).begin();
				// cout << '*' << (*i).first << ' '<< (*i).second<<'*';
				// // 	  	std::cout << "\n";
				// for ( i ; i != (*solutionVector).end(); ++i)
				// 	cout << '=' << (*i).first << ' '<< (*i).second<<'=';

				printSolutionVector(&solutionVector[0]);

				createWork(mon->maxIndex, solutionVector);
				haveNewWork = true;
			}
		}
		return NULL;
	}


