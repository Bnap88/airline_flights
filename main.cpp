//Brandon Knapp
//Airline Flights Graph - Shortest Path
#include <iostream>
#include <string>
#include <fstream>
#include "node.h"
#include "pq.h"
#include <map>
#include <vector>

using namespace std;

//Operator Overloads to Be used with cityNode's
bool operator>(cityNode A, cityNode B)
{
	return (A.totalDistance > B.totalDistance);
}
bool operator>=(cityNode A, cityNode B)
{
	return (A.totalDistance >= B.totalDistance);
}
bool operator<(cityNode A, cityNode B)
{
	return (A.totalDistance < B.totalDistance);
}
bool operator<=(cityNode A, cityNode B)
{
	return (A.totalDistance <= B.totalDistance);
}
bool operator!=(cityNode A, cityNode B)
{
	return (A.name != B.name);
}

void populateMap(map<string, int> & cityNum, int &count);
void getCities(string & start, string & ending, map<string, int> cityNum);
void declareMatrix(int count, int ** & cityConnection, int & rows, int & columns);
void populateMatrix(int ** & cityConnection, map<string, int> cityNum);
void populatePQwithNodes(map<string, int> cityNum, PQueue<cityNode> & myPQ, string start);
void deallocateMatrix(int ** & cityConnection, int rows);
void djikstra(PQueue<cityNode> & myPQ, map<string, int> cityNum, int ** cityConnection, vector<cityNode> & visitedList, string ending, bool & pathCheck, int columns);
void displayResult(PQueue<cityNode> & myPQ, vector<cityNode> & visitedList, map<string, int> cityNum, int ** cityConnection, bool & pathCheck);

//begin Main
int main()
{
	//Declare Variables and Structures
	map<string, int> cityNum;
	int count=0;
	PQueue<cityNode> myPQ;
	vector<cityNode> visitedList;
	bool pathCheck = false;
	string start, ending;
	int ** cityConnection;
	int rows, columns;

	//Populate Our Map For City Numbering
	populateMap(cityNum, count);

	//Get Starting and Ending Cities from User
	getCities(start, ending, cityNum);

	//Dynamically Allocate Adjacency Matrix & Set Non-Paths To 0
	declareMatrix(count, cityConnection, rows, columns);

	//Populate Adjacency Matrix From File 2
	populateMatrix(cityConnection, cityNum);
	
	//Create Nodes for Cities and Populate PQ
	populatePQwithNodes(cityNum, myPQ, start);

	//djikstra's algorithm
	djikstra(myPQ, cityNum, cityConnection, visitedList, ending, pathCheck, columns);

	//display results
	displayResult(myPQ, visitedList, cityNum, cityConnection, pathCheck);

	//Deallocate "cityConnection" dynamic mutlidimensional array
	deallocateMatrix(cityConnection, rows);

	return 0;
}

void populateMap(map<string, int> & cityNum, int &count)
{
	//Open File For List Of Cities
	ifstream cityFile;
	cityFile.open("File1.txt");

	//populate map with city names and their associative numbers
	while (!cityFile.eof())
	{
		string city;
		cityFile >> city;
		cityNum[city]=count;
		count++;
	}

	//close file 1
	cityFile.close();

}

void getCities(string & start, string & ending, map<string, int> cityNum)
{
	//get starting city check variable
	bool check = false;

	do
	{
		cout << "Please Enter Starting City (No Spaces): ";
		cin >> start;

		for (map<string,int>::iterator it=cityNum.begin(); it!=cityNum.end(); it++)
		{
			if ( it->first == start )
				check = true;
		}

	} while ( check != true );

	//reset check variable
	check = false;

	//get destination city
	do
	{
		cout << "Please Enter A Destination City (No Spaces): ";
		cin >> ending;

		for (map<string,int>::iterator it=cityNum.begin(); it!=cityNum.end(); it++)
		{
			if ( it->first == ending )
				check = true;
		}

	} while ( check != true );

	cout << endl;

}

void declareMatrix(int count, int ** & cityConnection, int & rows, int & columns)
{
	
	//Open File For City Flight Paths & Distances
	ifstream flightFile;
	flightFile.open("File2.txt");

	rows = count;
	columns = count;

	//Declare Adjacency Matrix
	cityConnection = new int*[rows];
	for(int i = 0; i < rows; i++)
		cityConnection[i] = new int[columns]; 

	//set each path to zero
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < columns; j++)
			cityConnection[i][j] = 0;
	}

	flightFile.close();

}

void populateMatrix(int ** & cityConnection, map<string, int> cityNum)
{
	//Open File For City Flight Paths & Distances
	ifstream flightFile;
	flightFile.open("File2.txt");

	//reset file 
	flightFile.clear();
	flightFile.seekg(0, ios::beg);

	//declare temporary variables to populate matrix
	string city1, city2;
	int dist;

	//Poplulate Adjacency Matrix from Flight Paths File
	while (!flightFile.eof())
	{
		flightFile >> city1 >> city2 >> dist;

		cityConnection[cityNum[city1]][cityNum[city2]] = dist;
	}

	flightFile.close();
}

void populatePQwithNodes(map<string, int> cityNum, PQueue<cityNode> & myPQ, string start)
{
	//Create City Nodes & Populate both PQ's
	for (map<string,int>::iterator it=cityNum.begin(); it!=cityNum.end(); it++)
	{
		if ( it->first == start )
		{
			cityNode * temp;
			temp = new cityNode;

			temp->name = it->first;
			temp->number = it->second;
			temp->totalDistance=0;
			temp->previous=-1;

			myPQ.enqueue(*temp);
			delete temp;
		}
		else
		{
			cityNode * temp;
			temp = new cityNode;

			temp->name = it->first;
			temp->number = it->second;
			temp->totalDistance=INT_MAX;
			temp->previous=-1;

			myPQ.enqueue(*temp);
			delete temp;
		}
	}
}

void deallocateMatrix(int ** & cityConnection, int rows)
{
	for(int i = 0; i < rows; i++)
		delete [] cityConnection[i];
	delete [] cityConnection;
}

void djikstra(PQueue<cityNode> & myPQ, map<string, int> cityNum, int ** cityConnection, vector<cityNode> & visitedList, string ending, bool & pathCheck, int columns)
{
	//take starting city node out of PQ and put into Visited List
	visitedList.push_back(myPQ.top());
	myPQ.dequeue();

	while (myPQ.isEmpty() == false && visitedList.back().totalDistance != INT_MAX)
	{
		
		if (visitedList.back().name == ending)
		{
			pathCheck = true;
			break;
		}
		else 
		{
			cityNode myTempNode;
			myTempNode = visitedList.back();

			for (int i=0; i<columns; i++)
			{
				if (cityConnection[cityNum[myTempNode.name]][i] != 0)
				{
					int tempDistance = myTempNode.totalDistance + cityConnection[cityNum[myTempNode.name]][i];

					if ( tempDistance < myPQ.findDistance(i) )
					{
						myPQ.reducePriority(i, tempDistance, visitedList.back().number);
					}
				}
			}
			
			visitedList.push_back(myPQ.top());
			myPQ.dequeue();

		}
	}
}

void displayResult(PQueue<cityNode> & myPQ, vector<cityNode> & visitedList, map<string, int> cityNum, int ** cityConnection, bool & pathCheck)
{
	if (pathCheck == true)
	{

	cout << "\n";
	cout << "TOTAL DISTANCE: " << visitedList.back().totalDistance << " Miles " << endl << endl;
	
	bool finalCheck = false;

		while (finalCheck == false)
		{
			cityNode tempNode;
			tempNode = visitedList.back();

			if (tempNode.previous == -1)
			{
				finalCheck = true;
				break;
			}

			cout << tempNode.name << " from ";	

			int tempFind = tempNode.previous;

			for (map<string,int>::iterator it=cityNum.begin(); it!=cityNum.end(); it++)
			{
				if ( it->second == tempFind )
				{
					cout << it->first;
				}
			}

			cout << " - ";  
			
			int to;

			for (map<string,int>::iterator it=cityNum.begin(); it!=cityNum.end(); it++)
			{
				if ( it->first == tempNode.name )
					to = it->second;
			}

			cout << cityConnection[tempNode.previous][to];	
				
			cout << " Miles " << endl;

			if (finalCheck == false)
			{
				for (vector<cityNode>::iterator it = visitedList.begin(); it != visitedList.end(); ++it)
				{
					cityNode tempNode2;
					tempNode2 = *it;

					if (tempNode2.number == tempFind)
					{
						visitedList.pop_back();
						visitedList.push_back(tempNode2);
						visitedList.erase(it);
						break;
					}
				} // end for loop
			}
		}
	}
	else
	{
		cout << "No Path Available!\n";
	}
	cout << endl;
}
