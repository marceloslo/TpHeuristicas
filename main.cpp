#include <iostream>
#include <cstdlib>
#include <ctime>
#include "bee_colony/bee_colony.hpp"

using namespace std;

int main(int argc,char* argv[])
{
	srand(time(NULL));
	int cost;
	const char *flag = (argc>1) ? argv[1] : "a";
    int hiveSize = (argc > 2) ? atoi(argv[2]) : 1;
    int nFoodSources = (argc > 3) ? atoi(argv[3]) : 1;
    int maxTrials = (argc > 4) ? atoi(argv[4]) : 1;
    int maxIters = (argc > 5) ? atoi(argv[5]) : 1;
    bee_colony b(hiveSize,nFoodSources,maxTrials,maxIters);
    b.findFoodSource(cost);
    b.printResult(flag);
    return 0;
}
