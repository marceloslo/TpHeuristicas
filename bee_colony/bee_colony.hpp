#ifndef BEE_COLONY_H
#define BEE_COLONY_H
#include <vector>
#include <set>

using std::vector;
using std::set;

class bee_colony
{
public:
	bee_colony();
	~bee_colony();
	void greedyCover();

	void printResult();

private:
	int nrows, ncolumns;
	int* costs;
	int totalCost;
	vector<set<int>> S;
	vector<int> solution;
};


#endif // !BEE_COLONY_H