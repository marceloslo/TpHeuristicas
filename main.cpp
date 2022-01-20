#include <iostream>
#include <cstdlib>
#include <ctime>
#include "bee_colony/bee_colony.hpp"

using namespace std;

int main(int argc,char* argv[])
{
	srand(time(NULL));
	int cost;
    bee_colony b;
    b.randomSolution(cost);
    b.printResult();
    return 0;
}
