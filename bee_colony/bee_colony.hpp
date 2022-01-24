#ifndef BEE_COLONY_H
#define BEE_COLONY_H
#include <vector>
#include <set>
#include <map>
#include "Bees.hpp"

using namespace std;

class bee_colony
{
public:
	//receives number of bees,food sources, trials and max iterations as parameters
	bee_colony(int nBees, int nFoodSources, int nTrials,int iterations);
	~bee_colony();
	
	//inutil só deixei pq ja tava feito
	void greedyCover();
	
	//acha aleatóriamente soluções, usado para scout e inicialização
	vector<int> findFoodSource(int &cost);
	
	//inicializa n soluções
	void initialize(int nFoodSources);
	
	// imprime resultado formatado
	void printResult();
	
	// encontra o vetor de probabilidades de uma abelha onlooker ir para a food source k
	vector<double> waggle(int totalFitness);
	
	// abandona as food sources que estagnaram
	void abandonFoodSources();
	
	// o algoritmo propriamente dito
	void beeColony();
	
	//função de busca de vizinhança. Se achar solução melhor lembrar de reduzir para 0 os ciclos da abelha que busca, caso contrário, incrementar.
	//void forage()(???) argumentos de função e etc a definir
private:
	int hivesize;
	vector<bee> forager, scout, onlooker;
	int trials,maxIter,nFoodSources;

	int nrows, ncolumns;
	int* costs;
	int minCost;
	//quais sets cada linha é coberta por (mapeia linha a sets que a cobrem)
	map<int,vector<int>> rows;
	//as fontes de alimento
	//vector<vector<int>> foodSources;
	//quais linhas cada set cobre(não está sendo usado)
	vector<set<int>> S;
	vector<int> solution;
};


#endif // !BEE_COLONY_H
