#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int **AllocIntMatrix(int n){
//	Functia aloca memorie pentru o matrice de int-uri de marime n
//	si o intoarce:	
	int i;
	int **tmp = calloc(n, sizeof(int*));
	for(i = 0; i < n; i++){
		tmp[i] = calloc(n, sizeof(int));
	}
	return tmp;
}

graph *InitGraph(int nr){
//	Functie de initializare a memoriei grafului:
	graph *G = calloc(1, sizeof(graph));
	G -> n = nr;
	G -> v = (int *)calloc(G -> n, sizeof(int));
	G -> matrix = AllocIntMatrix(G -> n);
	G -> d = AllocIntMatrix(G -> n);
	G -> N = AllocIntMatrix(G -> n);
	G -> vectorComponente = calloc(G -> n, sizeof(int));
	return G;
}

void FreeMatrix(int ***matrix, int n){
//	Functia elibereaza memoria unei matrici de marime n:
	int i;
	for(i = 0; i < n; i++){
		free((*matrix)[i]);
	}
	free(*matrix);
}

void FreeGraph(graph **G){
//	Functia elibereaza memoria folosita de structura graph:
	FreeMatrix(&((*G) -> matrix), (*G) -> n);
	FreeMatrix(&((*G) -> d), (*G) -> n);
	FreeMatrix(&((*G) -> N), (*G) -> n);
	free((*G) -> v);
	free((*G) -> vectorComponente);
	free(*G);
}