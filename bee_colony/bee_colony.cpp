#include "bee_colony.hpp"
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <climits>
#include <cstdlib>
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

//gets covers for each set(dependendo do que agnt for implementar, pode ser necessário mudar isso pra pegar quais sets cobrem cada linha no lugar de quais linhas um set cobre)
vector<set<int>> getCovers(int n, int m)
{
    vector<set<int>> covers(n);
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
                    covers[setId - 1].insert(elementId);
                }
            }
        }
        if (cnt)
        {
            //numero de sets que cobrem o elemento
            s >> numberOfSets;
            cnt = false;
            counter = 0;
        }
    }
    return covers;
}

//quais sets cobrem quais linhas
map<int,vector<int>> getRowCovers(int n, int m)
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
    int argmin=0,mincost=INT_MAX;
    double cost;
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
    this->rows = getRowCovers(ncolumns, nrows);
    this->trials = nTrials;
    this->hivesize = nBees;
    this->maxIter = iterations;
    this->nFoodSources = sources;
}

bee_colony::~bee_colony()
{
    delete[] costs;
}

//algoritmo greedy básico O(mn^2)
void bee_colony::greedyCover()
{
    vector<int> cover;
    set<int> U;
    int argmin;
    for (int i = 0; i < nrows; i++)
    {
        U.insert(i);
    }
    while (!U.empty())
    {
        //find set with minimum insertion cost(optimally covers all sets)
        argmin = minCostSet(S, U, costs);
        //add it to cover
        cover.push_back(argmin);
        minCost += costs[argmin];

        set<int> update;
        //U = U - S[i]
        set_difference(U.begin(), U.end(), S[argmin].begin(), S[argmin].end(), inserter(update, update.begin()));
        U = update;
    }
    solution = cover;
}

//encontra solução aleatória, O(nlogn)
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

        //remove ("cobre") todas linhas que contem esse conjunto
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
        }
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

void bee_colony::printResult()
{
    cout << "Cover: { ";
    for (const auto& it : solution)
    {
        cout << it + 1 <<" ";
    }
    cout << "}" << endl;
    cout << "Total cost: " << minCost << endl;
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
    initialize(nFoodSources);
    int k = 0, totalFitness=0, foodSourceFitness;
    vector<double> probabilities(nFoodSources);
    unsigned int f,o,s;
    while (k < this->maxIter)
    {
        totalFitness = 0;
        // fase das abelhas trabalhadoras
        for (f = 0; f < forager.size(); f++)
        {
            //busca de vizinhança
            forage(f);
            //soma das fitness achadas(importante para distrib de probabilidade)
            totalFitness += forager[f].fitness;
            if (forager[f].fitness < minCost)
            {
                minCost = forager[f].fitness;
                solution = forager[f].foodSource;
            }
        }

        //fase das abelhas onlooker 
        //probabilidades cumulativas "waggle dance"
        probabilities[0] = ((double)forager[0].fitness) / totalFitness;
        for (f = 1; f < probabilities.size(); f++)
        {
            probabilities[f] = (((double)forager[f].fitness) / totalFitness) + probabilities[f - 1];
        }
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
                    forage(f, o);
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
