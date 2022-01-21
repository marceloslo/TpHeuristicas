#ifndef BEES_H
#define BEES_H
#define FORAGER 0
#define SCOUT 1
#define ONLOOKER 2

class bee
{
public:
	bee(int r)
	{
		role = r;
		fitness = 0;
		foodSource = -1;
		cycles = 0;
	};
	bee(int food,int solution)
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
	int foodSource;
};


#endif //BEES_H
