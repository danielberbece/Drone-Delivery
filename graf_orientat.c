#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef struct graf {
	int n;
	int **mat;
} graf;

typedef struct nod {
	int val;
	struct nod *next;
} nod;

nod *pushstack(nod *S, int n){
	nod *tmp = calloc(1, sizeof(nod));
	tmp -> val = n;
	if( S == NULL ) return tmp;
	else {
		tmp -> next = S;
		return tmp;
	}
}

int pop(nod **S){
	int aux = (*S) -> val;
	(*S) = (*S) -> next;
	return aux;
}

graf *init( int n ){

	int i;
	graf *G = (graf*)calloc(1, sizeof(graf));
	G -> n = n;
	G -> mat = calloc(n, sizeof(int *));
	for(i = 0; i < n; i++){
		G -> mat[i] = calloc(n,sizeof(int));
	}
	return G;
}

void add_edge(graf **g, int i, int j, int c){

	(*g) -> mat[i][j] = c;
}

void del_edge(graf **g, int i, int j){

	(*g) -> mat[i][j] = 0;
}

void dfs(graf *g, int nod, int **visited, struct nod **S){

	int i;
	(*visited)[nod] = 1;
	for(i = 0; i < g -> n; i++){
		if( g -> mat[nod][i] != 0 && (*visited)[i] != 1 ){
			dfs(g, i, visited, S);
		}
	}
	*S = pushstack(*S, nod);
}

void dfsT(graf *g, int nod, int **visited){
	int i;
	(*visited)[nod] = 1;
	printf("%d ", nod);
	for(i = 0; i < g -> n; i++){
		if( g -> mat[i][nod] != 0 && (*visited)[i] != 1 ){
			dfsT(g, i, visited);
		}
	}
}

void print(graf *g){

	int i,j;
	for(i = 0; i < g -> n; i++){
		for(j = 0; j < g -> n; j++)
			printf("%d ", g -> mat[i][j]);
		printf("\n");
	}
	printf("\n");
}

void floyd(graf *G, int ***d, int **N){

	int i,j,k;
	for(k = 0; k < G -> n; k++){
		for(i = 0; i < G -> n; i++){
			for(j = 0; j < G -> n; j++){
				if( (*d)[i][j] > (*d)[i][k] + (*d)[k][j]){
					(*d)[i][j] = (*d)[i][k] + (*d)[k][j];
					N[i][j] = N[i][k];
				}
			}
		}
	}
}

void printMat(int **d, int n){
	int i,j;
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			if( d[i][j] != 999999)
				printf("%d ", d[i][j]);
			else printf("%d ", 0 );
		}
		printf("\n");
	}
	printf("\n");
}

void ctc(graf *G){

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
		int k = pop(&stiva);
		if( visited[k] == 0 ){
			printf("%s %d%c\n", "Componenta", i++, ':');
			dfsT(G, k, &visited);
			printf("\n");
		}
	}
}

void bonus(int **N, int n, int i, int j){

	// int i,j;
	// for(i = 0; i < n; i ++){
	// 	for(j = 0; j < n; j++){
	// 		if( i != j && N[i][j] != -1 ){
	// 			printf("%s %d %d %s\n", "Drumul minim dintre", i, j, "este format din:");
	// 			printf("%d ", i);
	// 			int nexthop = N[i][j];
	// 			printf("%d ", nexthop);
	// 			while(nexthop != j){
	// 				nexthop = N[nexthop][j];
	// 				printf("%d ", nexthop);
	// 			}
	// 			printf("\n");
	// 		}
	// 		if( N[i][j] == -1 ) printf("%s %d %s %d\n", "Nu se poate ajunge din", i, "in", j);
	// 	}
	// }
	if( i != j && N[i][j] != -1 ){
				printf("%s %d %d %s\n", "Drumul minim dintre", i, j, "este format din:");
				printf("%d ", i);
				int nexthop = N[i][j];
				printf("%d ", nexthop);
				while(nexthop != j){
					nexthop = N[nexthop][j];
					printf("%d ", nexthop);
				}
				printf("\n");
			}
	else if( N[i][j] == -1 ) printf("%s %d %s %d\n", "Nu se poate ajunge din", i, "in", j);
	else printf("%s\n", "Nodurile date sunt unul si acelasi" );
}

int main(){
	
	graf *G = init(7);
	add_edge(&G, 0, 1, 2);
	add_edge(&G, 0, 5, 4);
	add_edge(&G, 1, 2, 1);
	add_edge(&G, 2, 0, 3);
	add_edge(&G, 0, 4, 8);
	add_edge(&G, 4, 2, 2);
	add_edge(&G, 2, 3, 3);
	add_edge(&G, 3, 4, 5);
	add_edge(&G, 4, 5, 5);
	add_edge(&G, 5, 6, 6);
	//add_edge(&G, 6, 5, 3);
	printf("%s\n", "Graful:");
	print(G);
	int **d = calloc(G-> n, sizeof(int*));
	int i,j;
	for(i = 0; i < G -> n; i++){
		d[i] = calloc(G -> n, sizeof(int));
	}
	for(i = 0; i < G -> n; i++){
		for(j = 0; j < G -> n; j++){
			if(G -> mat[i][j] != 0 ){
				d[i][j] = G -> mat[i][j];
			} else {
				d[i][j] = 999999;
			}
			if(  i == j ){
				d[i][j] = 0;
			}
		}
	}

	int **N = calloc(G-> n, sizeof(int*));
	for(i = 0; i < G -> n; i++){
		N[i] = calloc(G -> n, sizeof(int));
	} 

	for(i = 0; i < G -> n; i++){
		for(j = 0; j < G -> n; j++){
			if(G -> mat[i][j] != 0 ){
				N[i][j] = j;
			} else {
				N[i][j] = -1;
			}
			if(  i == j ){
				N[i][j] = i;
			}
		}
	}

	floyd(G, &d, N);
	printf("%s\n", "Distanta minima:");
	printMat(d, G -> n);
	printf("%s\n", "Next hop:");
	printMat(N, G -> n);

	printf("%s\n", "Componente tare conexe:");
	ctc(G);
	printf("\n");
	bonus(N, G -> n, 0, 3);

	return 0;
}