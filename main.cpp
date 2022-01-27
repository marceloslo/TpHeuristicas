#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <string.h>
#include "bee_colony/bee_colony.hpp"

using namespace std;

int main(int argc,char* argv[])
{

	srand(time(NULL));
	const char* flag = (argc>5) ? argv[5] : "a";
    int hiveSize = (argc > 1) ? atoi(argv[1]) : 10;
    int nFoodSources = (argc > 2) ? atoi(argv[2]) : 5;
    int maxTrials = (argc > 3) ? atoi(argv[3]) : 10;
    int maxIters = (argc > 4) ? atoi(argv[4]) : 100;
    bee_colony b(hiveSize,nFoodSources,maxTrials,maxIters,flag);
    auto start_time = std::chrono::high_resolution_clock::now();
    b.beeColony();
    auto end_time = std::chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> time = end_time - start_time;
    b.printResult();
    if (strcmp(flag, "a") == 0 || strcmp(flag,"t") == 0)
    {
        cout << "Tempo de execucao(s): ";
        cout << time.count() / 1000 << endl;
    }
    else if (strcmp(flag, "g") == 0)
    {
        cout << time.count() / 1000 << endl;
    }
    return 0;
}
