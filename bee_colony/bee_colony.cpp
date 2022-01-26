#include "bee_colony.hpp"
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <climits>
#include <cstdlib>
#include <string.h>
#include "Bees.hpp"

using namespace std;

// gets number of lines and columns
void getDimension(int& m, int& n)
{
    string aux;
    getline(cin, aux);
    stringstream s(aux);
    s >> m;
    s >> n;
}

//get cost of each set (column)
void getCosts(int n,int costs[])
{
    int counter = 0;
    string aux;
    while (counter < n)
    {
        getline(cin, aux);
        stringstream s(aux);
        while (s >> costs[counter])
        {
            counter++;
        }
    }
}

//quais sets cobrem quais linhas
map<int,vector<int>> getRowCovers(int n, int m,map<int,vector<int>> &sets)
{
    map<int,vector<int>> covers;
    string aux;
    bool cnt = true;
    int numberOfSets, counter, elementId = 0, setId;
    while (getline(cin, aux))
    {
        stringstream s(aux);
        if (!cnt)
        {
            if (counter >= numberOfSets)
            {
                cnt = true;
                elementId++;
            }
            else
            {
                while (s >> setId)
                {
                    counter++;
                    //conjunto cobre o elemento i
                    covers[elementId].push_back(setId-1);
					sets[setId-1].push_back(elementId);
                }
            }
        }
        if (cnt)
        {
            //numero de sets que cobrem o elemento
            s >> numberOfSets;
            cnt = false;
            counter = 0;
            covers[elementId] = vector<int>();
        }
    }
    return covers;
}

//custo de adição por elemento novo de um set
double addCost(set<int> cover,set<int> U,int cost)
{
    set<int> intersection;
    set_intersection(cover.begin(),cover.end(),U.begin(),U.end(),inserter(intersection, intersection.begin()));
    if(intersection.size()==0)
    {
        return INT_MAX;
    }
    return ((double)cost)/intersection.size();
}

//encontra o set que tem menor custo para ser adicionado
int minCostSet(vector<set<int>> covers,set<int> U,int *costs)
{
    int argmin = 0;
    double cost,mincost = INT_MAX;
    for(unsigned int i=0;i<covers.size();i++)
    {
        cost = addCost(covers[i],U,costs[i]);
        if(cost<mincost)
        {
            mincost=cost;
            argmin=i;
        }
    }
    return argmin;
}

//gera número entre 0 e upperBound
int randomNumber(int upperBound)
{
    return rand() % upperBound;
}

bee_colony::bee_colony(int nBees,int sources,int nTrials,int iterations)
{
    nrows = 0;
    ncolumns = 0;
    minCost = 0;
    getDimension(nrows, ncolumns);
    costs = new int[ncolumns];
    getCosts(ncolumns,costs);;
    this->rows = getRowCovers(ncolumns, nrows,sets);
    this->trials = nTrials;
    this->hivesize = nBees;
    this->maxIter = iterations;
    this->nFoodSources = sources;
}

bee_colony::~bee_colony()
{
    delete[] costs;
}

//encontra solução aleatória, O(nlogn) -> n sets removidos e cada remoção custa log n
//pode ser reduzida criando um map em que cada set é mapeia para as linhas que ele cobre(tradeoff espaço x função)
vector<int> bee_colony::findFoodSource(int &cost)
{
    map<int,vector<int>> remainingRows = this->rows;
	vector<int> sol;
    int i,set;
	cost=0;
    while (!remainingRows.empty())
    {
        //seleciona aleatóriamente um conjunto que cobre a linha atual
        i = randomNumber(remainingRows.begin()->second.size());
        set = remainingRows.begin()->second[i];
        sol.push_back(set);
        cost += costs[set];


        //remove ("cobre") todas linhas que esse conjunto cobre
        for (const auto& it : sets[set])
        {
            remainingRows.erase(it);
        }
        /*
        for (auto it = remainingRows.begin();it!=remainingRows.end();)
        {
            bool found = binary_search(it->second.begin(), it->second.end(), set);
            if (found)
            {
                it = remainingRows.erase(it);
            }
            else
            {
                ++it;
            }
        }*/
    }
	//remover as duas linhas abaixo na versão final
	minCost = cost;
	this->solution=sol;
	return sol;
}

void bee_colony::initialize(int nFoodSources)
{
    int k;
    int i;
    //inicializa fontes de alimentos e trabalhadoras
    for (i = 0; i < nFoodSources; i++)
    {
        vector<int>aux=findFoodSource(k);
        forager.push_back(bee(FORAGER,aux, k));
    }
    //inicializa o resto como onlooker
    for (i = nFoodSources; i < hivesize; i++)
    {
        onlooker.push_back(bee(ONLOOKER));
    }
}

void bee_colony::printResult(const char* flag)
{
	if(strcmp(flag,"t")!=0)
	{
		cout << "Cover: { ";
		for (const auto& it : solution)
		{
			cout << it + 1 <<" ";
		}
		cout << "}" << endl;
		cout << "Total cost: ";
	}
	cout << minCost << endl;
}

vector<double> bee_colony::waggle(int totalFitness)
{
    vector<double> probabilities(nFoodSources);
    unsigned int f;
    probabilities[0] = ((double)1/forager[0].fitness) / totalFitness;
    for (f = 1; f < probabilities.size(); f++)
    {
        probabilities[f] = ((((double)1/forager[f].fitness)) / totalFitness) + probabilities[f - 1];
    }
    return probabilities;
}

void bee_colony::abandonFoodSources()
{
    for (vector<bee>::iterator it = onlooker.begin(); it != onlooker.end();)
    {
        if (it->cycles >= trials)
        {
            scout.push_back(bee(SCOUT));
            it = onlooker.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void bee_colony::beeColony()
{
    int k = 0, totalFitness = 0, foodSourceFitness;
    unsigned int f, o, s;
    vector<double> probabilities;
    initialize(nFoodSources);
    while (k < this->maxIter)
    {
        totalFitness = 0;
        // fase das abelhas trabalhadoras
        for (f = 0; f < forager.size(); f++)
        {
            //busca de vizinhança (incrementa numero de ciclos da abelha)
            //forage(f);
            //soma das fitness achadas(importante para distrib de probabilidade)
            totalFitness += (1/forager[f].fitness);
            if (forager[f].fitness < minCost)
            {
                minCost = forager[f].fitness;
                solution = forager[f].foodSource;
            }
        }

        //fase das abelhas onlooker
        //probabilidades cumulativas "waggle dance"
        probabilities = waggle(totalFitness);
        //abelhas onlooker exploram vizinhança das foodSources
        for (o = 0; o < onlooker.size(); o++)
        {
            //number between 0 and 1 (faixa de probabilidade escolhida)
            double choice = (float)rand() / RAND_MAX;
            for (f = 0; f < probabilities.size(); f++)
            {
                //escolheu essa food source
                if (choice <= probabilities[f])
                {
                    //busca em vizinhança onlooker (overloaded function)
                    //forage(f, o);
                    if (onlooker[o].fitness < minCost)
                    {
                        minCost = onlooker[o].fitness;
                        solution = onlooker[o].foodSource;
                    }
                    break;
                }
            }
        }

        //fase das abelhas scout
        abandonFoodSources();
        for (s = 0; s < scout.size(); s++)
        {
            //scout acha nova fonte de comida e passa a trabalhar nela.
            scout[s].foodSource = findFoodSource(foodSourceFitness);
            scout[s].fitness = foodSourceFitness;
            scout[s].role = FORAGER;
            scout[s].cycles = 0;
            if (scout[s].fitness < minCost)
            {
                minCost = scout[s].fitness;
                solution = scout[s].foodSource;
            }
            forager.push_back(scout[s]);
        }
        scout.clear();
        k++;
    }
}
