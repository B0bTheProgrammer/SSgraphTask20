// task1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <set>
#include <chrono>
#include <random>

using namespace std;

const string inputFileName = "input.txt";
const string outFileName = "output.txt";

struct edge {
	int a,b;
	edge(int x=-1, int y=-1) { a = x; b = y; };
};
bool operator==(const edge& lhs, const edge& rhs)
{
	return ((lhs.a == rhs.a) && (lhs.b == rhs.b)) || ((lhs.a == rhs.b) && (lhs.b == rhs.a));
}

bool operator!=(const edge& lhs, const edge& rhs)
{
	return !(lhs == rhs);
}

vector<edge> edges;
set<int> vertices;


set<int> vertices_part_1;
set<int> vertices_part_2;
int commEdgesNum = 0;

default_random_engine generator;

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

bool initStep()
{
	vector<edge> edgesFlipped;
	edgesFlipped.reserve(vertices.size()*vertices.size());
	for (set<int>::iterator i = vertices.begin(); i != vertices.end(); ++i)
	{
		set<int>::iterator j = i;
		for (++j ; j != vertices.end(); ++j)
		{
			edge edgeNew(*i, *j);
			bool flag = true;
			for (vector<edge>::iterator l = edges.begin(); l != edges.end(); ++l)
			{
				if (edgeNew == *l)
				{
					flag = false;
					break; 
				}
			}
			if (flag)
			{
				edgesFlipped.push_back(edgeNew);
			}
		}
	}
	if (edgesFlipped.size() == 0)
	{
		return false;
	}
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	generator.seed(seed);
	uniform_int_distribution<int> distribution(0, edgesFlipped.size()-1);
	edge edgeNew = edgesFlipped[distribution(generator)];
	vertices_part_1.insert(edgeNew.a);
	vertices_part_2.insert(edgeNew.b);
	vertices.erase(edgeNew.a);
	vertices.erase(edgeNew.b);

	return true;
}

bool splitGraph()
{

	while (vertices.size() > 0)
	{
		uniform_int_distribution<int> distribution(0, vertices.size() - 1);
		set<int>::iterator it(vertices.begin());
		advance(it, distribution(generator));
		int pt1Count = 0;
		int pt2Count = 0;
		for (set<int>::iterator pt1 = vertices_part_1.begin(); pt1 != vertices_part_1.end(); ++pt1)
		{
			edge edgeNew(*it, *pt1);
			for (vector<edge>::iterator l = edges.begin(); l != edges.end(); ++l)
			{
				if (edgeNew == *l)
				{
					pt1Count++;
					break;
				}		
			}
		}
		for (set<int>::iterator pt2 = vertices_part_2.begin(); pt2 != vertices_part_2.end(); ++pt2)
		{
			edge edgeNew(*it, *pt2);
			for (vector<edge>::iterator l = edges.begin(); l != edges.end(); ++l)
			{
				if (edgeNew == *l)
				{
					pt2Count++;
					break;
				}
			}
		}

		if (pt1Count > pt2Count)
		{
			vertices_part_1.insert(*it);
			commEdgesNum += pt2Count;
		}
		else
		{
			vertices_part_2.insert(*it);
			commEdgesNum += pt1Count;
		}
		vertices.erase(it);
	}

	return true;
}

bool saveResult()
{
	ofstream file(outFileName);
	file << commEdgesNum<< '\n';
	for (set<int>::iterator pt1 = vertices_part_1.begin(); pt1 != vertices_part_1.end(); ++pt1)
	{
		file << *pt1 << ' ';
	}
	file << '\n';
	for (set<int>::iterator pt2 = vertices_part_2.begin(); pt2 != vertices_part_2.end(); ++pt2)
	{
		file << *pt2 << ' ';
	}

	return true;
}

int main()
{
	if (!readGraph())
	{
		//множество дуг пусто
	}
	if (!initStep())
	{
		//граф полный
	}
	splitGraph();
	saveResult();
    return 0;
}

