#include <iostream>
#include <cstdlib>
#include <ctime>
#include "bee_colony/bee_colony.hpp"

using namespace std;

int main(int argc,char* argv[])
{
	srand(time(NULL));
	int cost;
    bee_colony b(100,50,10,1000);
    b.findFoodSource(cost);
    b.printResult();
    return 0;
}
