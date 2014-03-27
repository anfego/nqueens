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

	void createWork(int gridSize, vector<pair<int,int> > * solutionVector);

	void printSolutionVector(vector< pair<int,int> > * solutionVector);

	void printALLSolutionVectors(vector< pair<int,int> > * solutionVector);
	
	bool isPosibleSolution(vector< pair<int,int> > * solutionVector);
	
	bool checkRow(vector<pair<int,int> > * solutionVector);
	
	bool checkDiagHigh(vector<pair<int,int> > * solutionVector);
	
	bool checkDiagLow(vector<pair<int,int> > * solutionVector);
	
	void createInitalWork(int gridSize, vector< pair<int,int> > * vectorPtr );
	
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
		void *ret;
		int numSolutions = 0;


		if(argc < 5)
		{
			cerr << "Number of Arguments incorrect\n";
			return 0;
		}
		if(!strcmp(argv[1],"-n")){
			gridSize = atoi(argv[2]);
			cout<<gridSize<<"\n";
		}
		if(!strcmp(argv[3],"-p"))
			procNum = atoi(argv[4]);
		if(argc == 6)
			if(!strcmp(argv[5],"-q"))
				quiet = true;
		
		vector< pair<int,int> > * vectorPtr;
		vectorPtr = new ( vector< pair<int,int> > [gridSize] );
		pair<int,int> pairPtr;
		pairPtr.first = -1;
		pairPtr.second = 0;

		createInitalWork(gridSize, vectorPtr);
	
		
		pthread_t threads[procNum-1];
		
		procNum = procNum;
		poolStack = vectorPtr;
		buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
		
		//initialize cond mutex
		condMutex = PTHREAD_MUTEX_INITIALIZER;
		cond = PTHREAD_COND_INITIALIZER;

		// // creates a vector "stack" for each row
		monitor mon(procNum, quiet, gridSize, vectorPtr, buffer_mutex, condMutex, cond);
		

		for (int i = 0; i < procNum-1; ++i)
		{	
			
			if (pthread_create(&threads[i], NULL, processFunc, &mon)) 
			{
				cerr << "ERROR";
				exit(1);
			}
		}

		numSolutions = (int)processFunc(&mon);
		cout<<"\tThread 0: +" <<numSolutions<< endl;
	//monitor * mon = new monitor(procNum,vectorPtr);
		// cout<<"here\n";
		for (int i = 0; i < procNum-1; ++i)
		{	  
			pthread_join(threads[i], &ret);
			numSolutions = numSolutions + (int)ret;
			cout<<"\tThread "<< i +1<<": +"<< (int)ret << endl;
		}
		cout<<"\t-----------------\n";
		cout<<"\t   Total:  " <<numSolutions<< endl;
		cout<<"EXIT\n";
		return 0;
	}


	void printSolutionVector(vector< pair<int,int> >  * solutionVector)
	{
	  	cout << "\n";
		for (vector<pair<int,int> >::iterator i = (*solutionVector).begin() ; i != (*solutionVector).end(); ++i)
	    	cout << ' ' << (*i).first << ' '<< (*i).second<<'|';

		cout<<endl;
	}

	void createInitalWork(int gridSize, vector< pair<int,int> > * vectorPtr )
	{
		
		// vector< pair<int,int> > * vectorPtr;
		

		for (int i = 0; i < gridSize; ++i)
		{
			pair<int,int> * pairPtr = new pair<int,int>;
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
	void createWork(int gridSize, vector<pair<int,int> > * solutionVector)
	{

		vector<pair<int,int> >::iterator i = (*solutionVector).begin();
		int xCurrent = (*solutionVector).back().first;
		int yCurrent = (*solutionVector).back().second;
		
		// cout<<"Create New work\n"; //<<xCurrent<<'|'<<yCurrent <<  endl;
		for (int i = 0; i < gridSize; ++i)
		{
			// check if solutionVector is a solution xCurrent = gridSize
			pair<int,int> newPair;
			newPair.first = xCurrent + 1;
			newPair.second = i;
			solutionVector[i].push_back( newPair );
			// printSolutionVector(&solutionVector[i]);
			

			
			// delete(pairPtr);
		}
	
	}
	bool isPosibleSolution(vector<pair<int,int> > * solutionVector)
	{
		// printSolutionVector(solutionVector);
		if(checkRow(solutionVector))
		{
			// cout<<"ROW: this is  solution"<<endl;
			if (checkDiagHigh(solutionVector))
			{
				// cout<<"DH: this is  solution"<<endl;
				if (checkDiagLow(solutionVector))
				{
					// cout<<"DL: this is  solution"<<endl;
					return true;
				}
			}
		}
		
		return false;
	}
	bool checkRow(vector<pair<int,int> > * solutionVector)
	{
		
		if((*solutionVector).size() > 1)
		{
			int xCurrent = (*solutionVector).back().first;
			int yCurrent = (*solutionVector).back().second;
			// pop laststd::pair to avoid to compare

			(*solutionVector).pop_back();
			
			for (vector<pair<int,int> >::iterator i = (*solutionVector).begin() ; i != (*solutionVector).end(); ++i)
		    {
				if((*i).second == yCurrent)
				{
					// cout<<"\tNOT SOLUTION!!!!\n";
					return false;
				}
			}
			// possible solution, addstd::pair back to the vector
			pair<int,int> temp(xCurrent,yCurrent);
			(*solutionVector).push_back(temp);
		}
		return true;
	}
	bool checkDiagHigh(vector<pair<int,int> > * solutionVector)
	{

		
		int xCurrent = (*solutionVector).back().first;
		int yCurrent = (*solutionVector).back().second;
		// pop laststd::pair to avoid to compare

		(*solutionVector).pop_back();
		
		for (vector<pair<int,int> >::iterator i = (*solutionVector).begin() ; i != (*solutionVector).end(); ++i)
	    {
			
			for (int pos = 0; (yCurrent-pos) >= 0 ; ++pos)
			{
				// cout<<" cheching DH: "<< (*i).first << " == " << (xCurrent-pos) << " && " << (*i).second << " == " <<(yCurrent-pos) << endl;
				if((*i).first == (xCurrent-pos) && (*i).second == (yCurrent-pos))
				{
					// cout<<"\tNOT SOLUTION!!!!\n";
					return false;
				}
			}
		}
		// possible solution, addstd::pair back to the vector
		pair<int,int> temp(xCurrent,yCurrent);
		(*solutionVector).push_back(temp);
		return true;
	}
	bool checkDiagLow(vector<pair<int,int> > * solutionVector)
	{
		
		int xCurrent = (*solutionVector).back().first;
		int yCurrent = (*solutionVector).back().second;
		// pop laststd::pair to avoid to compare

		(*solutionVector).pop_back();
		
		for (vector<pair<int,int> >::iterator i = (*solutionVector).begin() ; i != (*solutionVector).end(); ++i)
	    {
			for (int pos = 0; (xCurrent-pos) >= 0 ; ++pos)
			{	
				// cout<<" cheching DL: "<< (*i).first << " == " << (xCurrent-pos) << " && " << (*i).second << " == " <<(yCurrent+pos) << endl;
				if((*i).first == (xCurrent-pos) && (*i).second == (yCurrent+pos))
				{
					// cout<<"\tNOT SOLUTION!!!!\n";
					return false;
				}
			}
		}
		// possible solution, addstd::pair back to the vector
		pair<int,int> temp(xCurrent,yCurrent);
		(*solutionVector).push_back(temp);
		return true;
	}
	
	void * processFunc(void *p)
	{
		bool haveNewWork = false;
		monitor * mon =  (monitor *) p;
		int numSolutions = 0; 
		int numIterations = 0;
		vector< pair<int,int> > * solutionVector;

		solutionVector = new  vector< pair<int,int> > [mon->maxIndex] ;
		
		while(!mon->allWorkDone)
		{
			if ( mon->monitor_enter(solutionVector, haveNewWork) == 1 && !mon->allWorkDone)
			{
				// Evaluate if the vector can be a solution
				if(isPosibleSolution(&solutionVector[0]))
				{
					// If vector is the size of the grid is a solution
					if((*solutionVector).size() == mon->maxIndex)
					{
						if(!mon->quiet)
							printSolutionVector(&solutionVector[0]);
						numSolutions++;
						haveNewWork = false;
					}
					else
					{
						// if the vector size is less of the grid size continue checking
						for (int i = 1; i < mon->maxIndex; ++i)
						{
							*(solutionVector+i) = *(solutionVector) ;
							
						}
						createWork(mon->maxIndex, solutionVector);
						haveNewWork = true;
					}
				}
				else
				{
					// this vector cannot be a solution dont add more work  
					haveNewWork = false;
				}
				numIterations++;
			}
				
		}
		// cout<<"Number of pieces processed: "<<numIterations<<endl;
		return (void *)numSolutions;

	}


