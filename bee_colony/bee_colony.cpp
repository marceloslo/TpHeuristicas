#include "bee_colony.hpp"
#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <climits>

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
vector<set<int>> getRowCovers(int n, int m)
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
                    covers[elementId].insert(setId-1);
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
    int argmin=0,mincost=INT_MAX,cost;
    for(int i=0;i<covers.size();i++)
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


bee_colony::bee_colony()
{
    nrows = 0;
    ncolumns = 0;
    totalCost = 0;
    getDimension(nrows, ncolumns);
    costs = new int[ncolumns];
    getCosts(ncolumns,costs);;
    this->S = getCovers(ncolumns, nrows);
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
        totalCost += costs[argmin];

        set<int> update;
        //U = U - S[i]
        set_difference(U.begin(), U.end(), S[argmin].begin(), S[argmin].end(), inserter(update, update.begin()));
        U = update;
    }
    solution = cover;
}

void bee_colony::printResult()
{
    cout << "Cover: { ";
    for (const auto& it : solution)
    {
        cout << it<<" ";
    }
    cout << "}" << endl;
    cout << "Total cost: " << totalCost << endl;
}
