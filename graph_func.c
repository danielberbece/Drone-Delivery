//	File with useful graph functions
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "mem_func.h"

//	Structura de nod, folosit la gasirea componentelor tare conexe
typedef struct nod {
	int val;
	struct nod *next;
} nod;

void Add_edge(graph **g, int i, int j, int c){
//	Functie de adaugare a unei muchii ponderate de la nodul i la nodul j
	(*g) -> matrix[i][j] = c;
}

void Floyd(graph *G){
//	Algoritmul lui Floyd, pentru a calcula distanta minima intre oricare
//	doua noduri si a matricei NextHop pentru graful G:

	int i,j,k;
	for(k = 0; k < G -> n; k++){
		for(i = 0; i < G -> n; i++){
			for(j = 0; j < G -> n; j++){
				if( G -> d[i][j] > G -> d[i][k] + G -> d[k][j]){
					G -> d[i][j] = G -> d[i][k] + G -> d[k][j];
					G -> N[i][j] = G -> N[i][k];
				}
			}
		}
	}
}

void CreateDistanceMatrix(graph *G){
//	Functia initializeaza matricea de distante minime si matricea NextHop
//	si apoi apeleaza algoritmul lui Floyd pentru a le completa:
	int i,j;
	for(i = 0; i < G -> n; i++){
		for(j = 0; j < G -> n; j++){
			if(G -> matrix[i][j] != 0 ){
				G -> d[i][j] = G -> matrix[i][j];
			} else {
				G -> d[i][j] = 999999999;
			}
			if(  i == j ){
				G -> d[i][j] = 0;
			}
		}
	}

	for(i = 0; i < G -> n; i++){
		for(j = 0; j < G -> n; j++){
			if(G -> matrix[i][j] != 0 ){
				G -> N[i][j] = j;
			} else {
				G -> N[i][j] = -1;
			}
			if( i == j ){
				G -> N[i][j] = i;
			}
		}
	}

	Floyd(G);
}

void ShowIntermediarNodes(int **N, int i, int j, FILE *outfile){
//	Functia afiseaza in fisierul de iesire nodurile intermediare
//	pentru distanta minima dintre nodurile i si j:

	//	Daca nodurile sunt diferite si exista drum intre ele:
	if( i != j && N[i][j] != -1 ){
		int nexthop = N[i][j];
		
		//	Cat timp nexthopul este diferit de nodul final j, afisam nexthop
		while(nexthop != j){
			fprintf(outfile, "%d ", nexthop);
			nexthop = N[nexthop][j];
		}
	}
}

nod *PushStack(nod *S, int n){
//	Functie de adaugare la stiva

	nod *tmp = calloc(1, sizeof(nod));
	tmp -> val = n;
	if( S == NULL ) return tmp;
	else {
		tmp -> next = S;
		return tmp;
	}
}

int PopStack(nod **S){
//	Functia de eliminare din stiva, cu returnarea variabilei din top
	int aux = (*S) -> val;
	nod *tmp = (*S);
	(*S) = (*S) -> next;
	free(tmp);
	return aux;
}

void dfs(graph *g, int nod, int **visited, struct nod **S){
//	Parcurgerea grafului, incepand dintr-un nod dat
	int i;
	(*visited)[nod] = 1;
	for(i = 0; i < g -> n; i++){
		if( g -> matrix[nod][i] != 0 && (*visited)[i] != 1 ){
			dfs(g, i, visited, S);
		}
	}
	*S = PushStack(*S, nod);
}

void dfsT(graph *g, int nod, int **visited, int k){
//	Parcurgerea unui graf in sens invers(Transpus). Folosit la
//	determinarea componentelor tare conexe.
	int i;
	(*visited)[nod] = 1;
	g -> vectorComponente[nod] = k;
	for(i = 0; i < g -> n; i++){
		if( g -> matrix[i][nod] != 0 && (*visited)[i] != 1 ){
			dfsT(g, i, visited, k);
		}
	}
}

void ComponenteTareConexe(graph *G){
//	Determinarea componentelor tare conexe din graful G cu algoritmul
//	lui Kosaraju:
	int i;
	nod *stiva = NULL;
	int *visited = calloc(G -> n, sizeof(int));

	for(i = 0; i < G -> n; i++){
		if( visited[i] == 0 ){
			dfs(G, i, &visited, &stiva);
		}
	}

	free(visited);
	visited = calloc(G -> n, sizeof(int));
	i = 1;
	while( stiva != NULL ){
		int k = PopStack(&stiva);
		if( visited[k] == 0 ){
			dfsT(G, k, &visited, i);
			i++;
		}
	}

	G -> nrCTC = i - 1;
	free(visited);
}

graph *CreateGraph(FILE *infile){
//	Initializarea grafului, crearea muchiilor 
//	si a matricei de distante.
//	Primeste ca parametru fisierul de date de intrare
	
	int i, n, x, y, cost;
	//	Citim numarul de noduri pentru a sti cata memorie sa alocam
	//	structurii de graf.
	fscanf(infile, "%d", &n);
	graph *G = InitGraph(n);

	//	Citim tipul fiecarui nod si il salvam in graful creat:
	for(i = 0; i < G -> n; i++){
		fscanf(infile, "%d", &(G -> v[i]));
	}

	//	Citim numarul de drumuri si le adaugam pe fiecare,
	//	impreuna cu costul lor.
	fscanf(infile, "%d", &(G -> edges));
	for(i = 0; i < G -> edges; i++){
		fscanf(infile, "%d %d %d", &x, &y, &cost);
		Add_edge(&G, x, y, cost);
	}

	//	Odata creata matricea de drumuri directe,
	//	cream matricea de distante minime de la orice nod
	//	la orice alt nod: 
	CreateDistanceMatrix(G);

	//	Intorcem graful creat:
	return G;
}