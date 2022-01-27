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

	//receives number of bees,food sources, trials,flags and max iterations as parameters
	bee_colony(int nBees, int nFoodSources, int nTrials, int iterations,const char *flag);

	~bee_colony();
	
	//acha aleatóriamente soluções, usado para scout e inicialização
	vector<int> findFoodSource(int &cost);
	
	//inicializa n soluções
	void initialize(int nFoodSources);
	
	// imprime resultado formatado
	void printResult();
	
	// encontra o vetor de probabilidades de uma abelha onlooker ir para a food source k
	vector<double> waggle(long double totalFitness);
	
	//busca em vizinhança 1-flip
	void forage(int f);

	//busca em vizinhança troca de colunas
	void forage(int f,bee &b);

	// abandona as food sources que estagnaram
	void abandonFoodSources();
	
	// o algoritmo propriamente dito
	void beeColony();
	
	//função de busca de vizinhança. Se achar solução melhor lembrar de reduzir para 0 os ciclos da abelha que busca, caso contrário, incrementar.
	//void forage()(???) argumentos de função e etc a definir
private:

	void repair(vector<int> &solution,int &fitness);
	void flip2(int f, bee& b);

	vector<bee> forager, scout, onlooker;

	//parametros
	int trials,maxIter,nFoodSources,hivesize;;

	int nrows, ncolumns;
	int* costs;
	int minCost;
	const char* flag;

	//quais sets cada linha é coberta por (mapeia linha a sets que a cobrem)
	map<int,vector<int>> rows;

	//quais linhas cada set cobre
	map<int,vector<int>> sets;
	
	vector<int> solution;
};


#endif // !BEE_COLONY_H
