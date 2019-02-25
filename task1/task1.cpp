// task1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <set>

using namespace std;

const string inputFileName = "input.txt";

struct edge {
	int a,b;
	edge(int x, int y) { a = x; b = y; };
};

vector<edge> edges;
set<int> vertices;


bool readGraph()
{
	ifstream file(inputFileName);
	string line;
	while (getline(file, line))
	{
		std::stringstream iss(line);
		int x1;
		iss >> x1;
		int x2;
		iss >> x2;
		edges.push_back(edge(x1, x2));
		vertices.insert(x1);
		vertices.insert(x2);
	}
	if (edges.size() == 0)
		return false;
	return true;
}

int main()
{
	if (!readGraph())
	{

	}


    return 0;
}

