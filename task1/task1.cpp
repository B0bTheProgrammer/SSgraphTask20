// Задача 20. Выполнил Решитько Михаил.
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

const string inputFileName = "input.txt"; // имя файла исходных данных 
const string outFileName = "output.txt"; // имя файла с решением

struct edge {
	int a,b;
	edge(int x=-1, int y=-1) { a = x; b = y; };
}; // структура описывает ребро графа
bool operator==(const edge& lhs, const edge& rhs)
{
	return ((lhs.a == rhs.a) && (lhs.b == rhs.b)) || ((lhs.a == rhs.b) && (lhs.b == rhs.a));
}

bool operator!=(const edge& lhs, const edge& rhs)
{
	return !(lhs == rhs);
}

vector<edge> edges; // список ребер графа ( содержит все ребра из исходного файла)
set<int> vertices; // множество вершин графа, еще не распределенных по частям разбиения


set<int> vertices_part_1; // эти 2 множества и переменная содержат ответ на задачу -
set<int> vertices_part_2; //- две части разбиения и число дуг между ними
int commEdgesNum = 0;

default_random_engine generator; // генератор случайных чисел

bool elemInSet(int element, set<int> Set)
{
	if (Set.count(element) == 0)
		return false;
	return true;
}

bool readGraph()
{
	/*функция считывает граф из файла и заполняет список ребер и множество вершин графа*/
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
		vertices.insert(x1);//контейнер set автоматически контролирует уникальность своих элементов,
		vertices.insert(x2);//лишние не будут добавлены
	}
	if (edges.size() == 0)
		return false;
	return true;
}

bool initStep()
{
	/*функция считывает делает первый шаг алгоритма - кладет в каждое множество по случайной вершине,
	которые не соединены дугой*/
	vector<edge> edgesFlipped;//вектор содержит все возможные дуги, которых нет в исходном графе
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
		return false;//граф полный
	}
	//теперь выбирается случайная дуга, ее вершины кладутся в разные множества и удаляются из списка нераспределенных вершин графа
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
	int n = (vertices.size() + 2)/2;//половина общего числа вершин

	/*Здесь оставшиеся вершины графа распределяются по частям разбиения согласно алгоритму из постановки задачи*/
	while (vertices.size() > 0)
	{
		//выбираем случайную вершину
		uniform_int_distribution<int> distribution(0, vertices.size() - 1);
		set<int>::iterator it(vertices.begin());
		advance(it, distribution(generator));
		int pt1Count = 0;//число дуг между выбраной вершиной и вершинами разбиения 1
		int pt2Count = 0;//число дуг между выбраной вершиной и вершинами разбиения 2
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
			//commEdgesNum += pt2Count;//когда вершина добавляется в множество, дуги, которые связывали вершину с другим разбиением, начаниют  связывать и разбиения 
		}
		else
		{
			vertices_part_2.insert(*it);
			//commEdgesNum += pt1Count;
		}
		vertices.erase(it);//удаляем распределенную вершину
		if (vertices_part_1.size() == n)
		{
			vertices_part_2.insert(vertices.begin(), vertices.end());
			break;
		}
		if (vertices_part_2.size() == n)
		{
			vertices_part_1.insert(vertices.begin(), vertices.end());
			break;
		}
	}

	for (vector<edge>::iterator it = edges.begin(); it != edges.end(); ++it)
	{
		if (elemInSet(it->a, vertices_part_1))
		{
			if (elemInSet(it->b, vertices_part_2))
			{
				commEdgesNum++;
			}
		}
		else
		{
			if (elemInSet(it->a, vertices_part_2))
			{
				if (elemInSet(it->b, vertices_part_1))
				{
					commEdgesNum++;
				}
			}
		}
	}
		

	return true;
}

bool saveResult()
{
	//запись в файл согласно формату из постановки задачи
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

