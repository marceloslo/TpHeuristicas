#ifndef BEE_COLONY_H
#define BEE_COLONY_H
#include <vector>
#include <set>
#include <map>

using std::vector;
using std::set;
using std::map;

class bee_colony
{
public:
	bee_colony();
	~bee_colony();
	void greedyCover();
	void randomSolution();
	void printResult();

private:
	int nrows, ncolumns;
	int* costs;
	int totalCost;
	//quais sets cada linha é coberta por (mapeia linha a sets que a cobrem)
	map<int,vector<int>> rows;

	//quais linhas cada set cobre
	vector<set<int>> S;
	vector<int> solution;
};


#endif // !BEE_COLONY_H