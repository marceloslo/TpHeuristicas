#ifndef BEES_H
#include <vector>
#include <climits>
#define BEES_H
#define FORAGER 0
#define SCOUT 1
#define ONLOOKER 2

using std::vector;

class bee
{
public:
	bee(int r)
	{
		role = r;
		fitness = INT_MAX;
		cycles = 0;
	};
	bee(int r,vector<int> food,int solution)
	{
		role = 0;
		fitness = solution;
		foodSource = food;
		cycles = 0;
	};

	//0=employed,1=scout,2=onlooker
	int role;
	//number of times bestSolution hasn't changed
	int cycles;
	//best solution found
	int fitness;
	//associated foodSource, irrelevant if not employed
	vector<int> foodSource;
};


#endif //BEES_H
