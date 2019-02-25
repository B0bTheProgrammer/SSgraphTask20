// task1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

const string inputFileName = "input.txt";

struct edge {
	int a,b;
	edge(int x, int y) { a = x; b = y; };
};

vector<edge> edges;

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
	}
	return true;
}

int main()
{
	readGraph();

    return 0;
}

