#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
#include <set>
#include "bee_colony/bee_colony.hpp"

using namespace std;

int main(int argc,char* argv[])
{
    bee_colony b;
    b.randomSolution();
    b.printResult();
    return 0;
}
