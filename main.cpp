#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

void getDimension(int &m,int &n)
{
    string aux;
    getline(cin,aux);
    stringstream s(aux);
    s>>m;
    s>>n;
}

int *getCosts(int n)
{
    int counter=0;
    int *costs = new int[n];
    string aux;
    while(counter<n)
    {
        getline(cin,aux);
        stringstream s(aux);
        while(s >> costs[counter])
        {
            counter++;
        }
    }
    return costs;
}

vector<unordered_set<int>> getCovers(int n,int m)
{
    vector<unordered_set<int>> covers(n);
    string aux;
    bool cnt=true;
    int numberOfSets,counter,elementId=0,setId;
    while(getline(cin,aux))
    {
        stringstream s(aux);
        if(!cnt)
        {
            if(counter >= numberOfSets)
            {
                cnt=true;
                elementId++;
            }
            else
            {
                while(s >> setId)
                {
                    counter++;
                    //conjunto cobre o elemento i
                    covers[setId-1].insert(elementId);
                }
            }
        }
        if(cnt)
        {
            //numero de sets que cobrem o elemento
            s >>  numberOfSets;
            cnt=false;
            counter=0;
        }
    }
    return covers;
}

int main(int argc,char* argv[])
{
    int *costs;
    int n,m;
    getDimension(m,n);
    costs=getCosts(n);
    vector<unordered_set<int>> covers=getCovers(n,m);

    return 0;
}
