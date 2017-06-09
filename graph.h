//	Header pentru structura de graf.
//	Aceasta contine:
//	int n - numarul de noduri din graf
//	int edges - numarul de muchii din graf
//	int **matrix - matricea de costuri a grafului.
//	int *v - vector in care se retin tipurile nodurilor
//	int **d - matrice de distante minime de la un nod la altul
//	int **N - matrice in care se retin nodurile NextHop pentru drumurile minime
//	int nrCTC - numarul de componente tare conexe, folosit la bonus
//	int *vectorComponente - vector in care se retine din ce componenta tare
//							conexa face parte fiecare nod din graf

#ifndef _GRAPH_H
#define _GRAPH_H
	typedef struct graph {
		int n;
		int edges;
		int **matrix;
		int *v;
		int **d;
		int **N;
		int nrCTC;
		int *vectorComponente;
	} graph;
#endif

