#ifndef _MEM_FUNC_H
#define _MEM_FUNC_H
	int **AllocIntMatrix(int n);
	graph *InitGraph(int nr);
	void FreeMatrix(int ***matrix, int n);
	void FreeGraph(graph **G);
#endif