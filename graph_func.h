//	Header pentru functiile de baza in lucrul cu structura de graf,
//	pentru functiile explicate in graph_func.c :
#ifndef _GRAPH_FUNC_H
#define _GRAPH_FUNC_H
	void ShowIntermediarNodes(int **N, int i, int j, FILE *outfile);
	void ComponenteTareConexe(graph *G);
	graph *CreateGraph(FILE *infile);
#endif
