#ifndef BEE_COLONY_H
#define BEE_COLONY_H
#include <vector>
#include <set>
#include <map>
#include "Bees.hpp"

using namespace std;

class bee_colony
{
public:
	//receives number of bees,food sources, trials and max iterations as parameters
	bee_colony(int nBees, int nFoodSources, int nTrials,int iterations);
	~bee_colony();
	void greedyCover();
	vector<int> findFoodSource(int &cost);
	void initialize(int nFoodSources);
	void printResult();
	vector<double> waggle(int totalFitness);
	void abandonFoodSources();
	void beeColony();
private:
	int hivesize;
	vector<bee> forager, scout, onlooker;
	int trials,maxIter,nFoodSources;

	int nrows, ncolumns;
	int* costs;
	int minCost;
	//quais sets cada linha é coberta por (mapeia linha a sets que a cobrem)
	map<int,vector<int>> rows;
	//as fontes de alimento
	//vector<vector<int>> foodSources;
	//quais linhas cada set cobre
	vector<set<int>> S;
	vector<int> solution;
};


#endif // !BEE_COLONY_H