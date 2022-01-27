# TpHeuristicas
- Bees.hpp: classe auxiliar bee que modela uma abelha, com sua comida e sua fitness
- BeeColony.hpp.cpp: Classe que modela o algoritmo bee colony com todos seus processos de scout,forage,inicialização,waggle dance, etc...

## Como usar
- digite make
- rode o comando main.exe hivesize foragerbees maxcycles maxiterations Flag
- Flag pode ser:
    - "a" -> imprime o resultado completo 
    - "t" -> imprime o resultado sem a cobertura
    - "g" -> resultado no formato para gerar tabela de resultados
    - "p" -> no formato para gerar graficos de convergencia
