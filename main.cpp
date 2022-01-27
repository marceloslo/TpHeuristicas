#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "bee_colony/bee_colony.hpp"

using namespace std;

int main(int argc,char* argv[])
{

	srand(time(NULL));
	const char* flag = (argc>1) ? argv[1] : "a";
    int hiveSize = (argc > 2) ? atoi(argv[2]) : 10;
    int nFoodSources = (argc > 3) ? atoi(argv[3]) : 5;
    int maxTrials = (argc > 4) ? atoi(argv[4]) : 10;
    int maxIters = (argc > 5) ? atoi(argv[5]) : 100;
    bee_colony b(hiveSize,nFoodSources,maxTrials,maxIters);
    auto start_time = std::chrono::high_resolution_clock::now();
    b.beeColony();
    auto end_time = std::chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> time = end_time - start_time;
    b.printResult(flag);
    cout << "Tempo de execucao(s): ";
    cout << time.count()/1000 << endl;
    return 0;
}
