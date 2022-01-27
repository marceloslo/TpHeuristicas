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
    this->flag = "a";
    nrows = 0;
    ncolumns = 0;
    minCost = INT_MAX;
    getDimension(nrows, ncolumns);
    costs = new int[ncolumns];
    getCosts(ncolumns,costs);;
    this->rows = getRowCovers(ncolumns, nrows,sets);
    this->trials = nTrials;
    this->hivesize = nBees;
    this->maxIter = iterations;
    this->nFoodSources = sources;
}

bee_colony::bee_colony(int nBees, int sources, int nTrials, int iterations, const char* flag)
{
    this->flag = flag;
    nrows = 0;
    ncolumns = 0;
    minCost = INT_MAX;
    getDimension(nrows, ncolumns);
    costs = new int[ncolumns];
    getCosts(ncolumns, costs);;
    this->rows = getRowCovers(ncolumns, nrows, sets);
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
    srand(rand());
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
	//minCost = cost;
	//this->solution=sol;
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
        if (minCost > k)
        {
            minCost = k;
            solution = aux;
        }
    }
    //inicializa o resto como onlooker
    for (i = nFoodSources; i < hivesize; i++)
    {
        onlooker.push_back(bee(ONLOOKER));
    }
}

void bee_colony::printResult()
{
	if(strcmp(flag,"a")==0)
	{
		cout << "Cover: { ";
		for (const auto& it : solution)
		{
			cout << it + 1 <<" ";
		}
		cout << "}" << endl;
	}
    if (strcmp(flag, "a") == 0||strcmp(flag,"t")==0)
    {
        cout << "Custo minimo: "<<minCost << endl;
    }
    else if (strcmp(flag, "g") == 0)
    {
        cout << minCost << " ";
    }
}

vector<double> bee_colony::waggle(long double totalFitness)
{
    vector<double> probabilities(forager.size());
    unsigned int f;
    probabilities[0] = ((long double)1/forager[0].fitness) / totalFitness;
    for (f = 1; f < probabilities.size(); f++)
    {
        probabilities[f] = ((((long double)1/forager[f].fitness)) / totalFitness) + probabilities[f - 1];
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

void bee_colony::forage(int f)
{
    forage(f, forager[f]);
}

//pior dos casos O(tamanho de solução^2)
void bee_colony::forage(int f, bee& b)
{
    int randomBee = randomNumber(forager.size());
    while (randomBee==f)
    {
        randomBee = randomNumber(forager.size());
    }
    sort(forager[f].foodSource.begin(), forager[f].foodSource.end());
    sort(forager[randomBee].foodSource.begin(), forager[randomBee].foodSource.end());
    bee otherBee = forager[randomBee];
    if (otherBee.foodSource == forager[f].foodSource)
    {
        //forager.erase(forager.begin() + f);
        forager[f].cycles = INT_MAX;
        //scout.push_back(bee(SCOUT));
        return;
    }
    vector<int> currentSolution = forager[f].foodSource;
    int newFitness=b.fitness;
    int addcols = randomNumber((int)forager[randomBee].foodSource.size());
    int rmcols = randomNumber((int)forager[f].foodSource.size());
    int i,col;

    //adds random columns from other solution pior dos casos:c*n (onde n proporcional ao tamanho de current solution)
    for (i = 0; i < addcols; i++)
    {
        col = forager[randomBee].foodSource[randomNumber((int)forager[randomBee].foodSource.size())];
        auto it = lower_bound(currentSolution.begin(), currentSolution.end(), col);
        //se já não estiver no vector, adicione
        if ((it-currentSolution.begin())>=(int)currentSolution.size() || (*it)!=col)
        {
            currentSolution.insert(it,col);
            newFitness += costs[col];
        }
    }

    //removes random columns from solution pior dos casos:r*n (onde n proporcional ao tamanho de current solution)
    for (i = 0; i < rmcols; i++)
    {
        col = randomNumber(currentSolution.size());
        newFitness -= costs[currentSolution[col]];
        currentSolution.erase(currentSolution.begin() + col);
    }

    repair(currentSolution, newFitness);

    if (newFitness < b.fitness)
    {
        b.foodSource = currentSolution;
        b.fitness = newFitness;
        b.cycles = 0;
    }
    else
    {
        b.cycles++;
    }
    return;
}

/*void bee_colony::forage(int f, bee& b)
{
    flip2(f, b);
}*/
void bee_colony::beeColony()
{
    int k = 0, foodSourceFitness;
    long double totalFitness;
    unsigned int f, o, s;
    vector<double> probabilities;
    initialize(nFoodSources);
    while (k < this->maxIter)
    {
        if (strcmp(flag, "p") == 0)
        {
            cout << k << " " << minCost << endl;
        }
        totalFitness = 0;
        // fase das abelhas trabalhadoras
        for (f = 0; f < forager.size(); f++)
        {
            //busca de vizinhança (incrementa numero de ciclos da abelha)
            forage(f);
            //soma das fitness achadas(importante para distrib de probabilidade)
            totalFitness += ((long double)1)/forager[f].fitness;
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
                    onlooker[o].fitness = forager[f].fitness;
                    forage(f,onlooker[o]);
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

//repara solução
void bee_colony::repair(vector<int>& solution,int &fitness)
{
    unsigned int i;
    map<int, vector<int>> remainingRows = rows;
    for (i = 0; i < solution.size(); i++)
    {
        for (const auto& it : sets[solution[i]])
        {
            remainingRows.erase(it);
        }
    }
    int argmin,randomrow,count=0;
    while (!remainingRows.empty())
    {
        randomrow = randomNumber((int)remainingRows.size());
        argmin = remainingRows.begin()->second[0];
        //seleciona aleatóriamente uma linha a ser coberta greedy
        for (map<int, vector<int>>::iterator k = remainingRows.begin(); k != remainingRows.end(); k++)
        {
            if (count == randomrow)
            {
                for (const auto& it : k->second)
                {
                    if (costs[it] < costs[argmin])
                    {
                        argmin = it;
                    }
                }
            }
            count++;
        }
        /*for (const auto& it : remainingRows.begin()->second)
        {
            if (costs[it] < costs[argmin])
            {
                argmin = it;
            }
        }*/
        //i = randomNumber(remainingRows.begin()->second.size());
        //argmin = remainingRows.begin()->second[i];
        solution.push_back(argmin);
        fitness += costs[argmin];

        //remove ("cobre") todas linhas que esse conjunto cobre
        for (const auto& it : sets[argmin])
        {
            remainingRows.erase(it);
        }
    }
}

void bee_colony::flip2(int f, bee& b)
{
    int rc1 = randomNumber(forager[f].foodSource.size()), rc2= randomNumber(forager[f].foodSource.size());
    while (rc1 == rc2)
    {
        rc2 = randomNumber(forager[f].foodSource.size());
    }
    vector<int> newSolution = forager[f].foodSource;
    
    //remove 2 columns randomly
    int newFitness = forager[f].fitness- costs[newSolution[rc1]]- costs[newSolution[rc2]];
    if (rc1 > rc2)
    {
        newSolution.erase(newSolution.begin() + rc1);
        newSolution.erase(newSolution.begin() + rc2);
    }
    else
    {
        newSolution.erase(newSolution.begin() + rc2);
        newSolution.erase(newSolution.begin() + rc1);
    }

    //determine remaining rows
    map<int, vector<int>> remainingRows = rows;
    unsigned int i;
    for (i = 0; i < newSolution.size(); i++)
    {
        for (const auto& it : sets[newSolution[i]])
        {
            remainingRows.erase(it);
        }
    }

    //add up to 2 new columns greedly
    int argmin, randomrow, count = 0;
    i = 0;
    while(!remainingRows.empty() && i<2)
    {
        randomrow = randomNumber((int)remainingRows.size());
        argmin = remainingRows.begin()->second[0];
        //seleciona aleatóriamente uma linha a ser coberta greedy
        for (map<int, vector<int>>::iterator k = remainingRows.begin(); k != remainingRows.end(); k++)
        {
            if (count == randomrow)
            {
                for (const auto& it : k->second)
                {
                    if (costs[it] < costs[argmin])
                    {
                        argmin = it;
                    }
                }
            }
            count++;
        }
        newSolution.push_back(argmin);
        newFitness += costs[argmin];

        //remove ("cobre") todas linhas que esse conjunto cobre
        for (const auto& it : sets[argmin])
        {
            remainingRows.erase(it);
        }
        ++i;
    }
    //checa se houve melhora
    if ((!remainingRows.empty()) || (newFitness >= b.fitness))
    {
        b.cycles++;
    }
    else
    {
        b.cycles = 0;
        b.fitness = newFitness;
        b.foodSource = newSolution;
    }
}
