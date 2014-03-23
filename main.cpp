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

	// #include "monitor.h"

	using namespace std;

	void createWork(int gridSize);

	void printSolution(std::vector< std::pair<int,int> > solution);

	bool checkSolution(std::std::vector< pair<int,int> * solutionVector);
	bool checkRow(std::std::vector< pair<int,int> * solutionVector);
	bool checkDiagHigh(std::std::vector< pair<int,int> * solutionVector);
	bool checkDiagLow(std::std::vector< pair<int,int> * solutionVector);


	std::std::vector< pair<int,int> > * createInitalWork(int gridSize);
	
	int main(int argc, char const *argv[])
	{
		int gridSize = 4;
		int pNum = 1;
		bool quiet = false;

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
			pNum = atoi(argv[4]);

		if(!strcmp(argv[5],"-q"))
			quiet = true;
		

		// creates a vector "stack" for each row
		std::vector< std::pair<int,int> > * vectorPtr;
		createInitalWork(gridSize, vectorPtr);
		// vectorPtr = new ( std::vector< std::pair<int,int> > [gridSize] );

		// for (int i = 0; i < gridSize; ++i)
		// {
		// 	pair<int,int> * pairPtr = new std::pair<int,int>;
		// 	(*pairPtr).first = 0;
		// 	(*pairPtr).second = i;

		// 	vectorPtr[i].push_back( *pairPtr );
		// 	printSolution(vectorPtr[i]);
		// 	delete(pairPtr);
		// }
	
		// create work
		
		// stack work
		// get work

		for (int i = 0; i < gridSize; ++i)
		{
			delete(vectorPtr+i);
		}

		return 0;
	}

	// this funtion create work stack 
	// arguments:
	// 		size of the grid
	// 		location 
	void createWork(int gridSize, std::vector< pair<int,int> > v)
	{

	}
	void printSolution(std::vector< std::pair<int,int> > solution)
	{
		for (std::vector< pair<int,int> >::iterator i = solution.begin() ; i != solution.end(); ++i)
	    	std::cout << ' ' << (*i).first << ' '<< (*i).second;
	  	std::cout << '\n';

	}
	void createInitalWork(int gridSize, std::vector< std::pair<int,int> > * vectorPtr )
	{
		
		
		vectorPtr = new ( std::vector< std::pair<int,int> > [gridSize] );

		for (int i = 0; i < gridSize; ++i)
		{
			pair<int,int> * pairPtr = new std::pair<int,int>;
			(*pairPtr).first = 0;
			(*pairPtr).second = i;

			vectorPtr[i].push_back( *pairPtr );
			printSolution(vectorPtr[i]);
			delete(pairPtr);
		}

	}
	bool checkSolution(std::std::vector< pair<int,int> * solutionVector)
	{

		if (checkRow(solutionVector) && checkDiagHigh(solutionVector) & checkDiagLow(solutionVector))
			return true;
		return false;
	}
	bool checkRow(std::std::vector< pair<int,int> * solutionVector)
	{
		
		
		int xCurrent = *(solutionVector).push_back().first;
		int yCurrent = *(solutionVector).push_back().second;
		// pop last pair to avoid to compare

		(*solutionVector).pop_back();
		
		for (std::vector< pair<int,int> >::iterator i = solutionVector.begin() ; i != solutionVector.end(); ++i)
	    {
			if((*i).first == xCurrent)
				return false;
		}
		// possible solution, add pair back to the vector
		(*solutionVector).push_back(new pair<int,int>(xCurrent,yCurrent));
		return true;
	}
	bool checkDiagHigh(std::std::vector< pair<int,int> * solutionVector)
	{

		
		int xCurrent = *(solutionVector).push_back().first;
		int yCurrent = *(solutionVector).push_back().second;
		// pop last pair to avoid to compare

		(*solutionVector).pop_back();
		
		for (std::vector< pair<int,int> >::iterator i = solutionVector.begin() ; i != solutionVector.end(); ++i)
	    {

			for (int pos = 0; (yCurrent-pos) <= 0 ; ++pos)
			{
				if((*i).first == (xCurrent-pos) && (*i).second == (yCurrent-pos))
					return false;
			
			}
		}
		// possible solution, add pair back to the vector
		(*solutionVector).push_back(new pair<int,int>(xCurrent,yCurrent));
		return true;
	}
	bool checkDiagLow(std::std::vector< pair<int,int> * solutionVector)
	{

		
		int xCurrent = *(solutionVector).push_back().first;
		int yCurrent = *(solutionVector).push_back().second;
		// pop last pair to avoid to compare

		(*solutionVector).pop_back();
		
		for (std::vector< pair<int,int> >::iterator i = solutionVector.begin() ; i != solutionVector.end(); ++i)
	    {

			for (int pos = 0; (xCurrent-pos) <= 0 ; ++pos)
			{
				if((*i).first == (xCurrent-pos) && (*i).second == (yCurrent+pos))
					return false;
			
			}
		}
		// possible solution, add pair back to the vector
		(*solutionVector).push_back(new pair<int,int>(xCurrent,yCurrent));
		return true;
	}
	

