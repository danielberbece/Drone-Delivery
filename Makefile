build: livrare

livrare: livrare.o graph_func.o mem_func.o
	gcc -Wall livrare.o graph_func.o mem_func.o -o livrare

livrare.o: livrare.c
	gcc -Wall -c livrare.c 

graph_func.o: graph_func.c
	gcc -Wall -c graph_func.c

mem_func.o: mem_func.c
	gcc -Wall -c mem_func.c

clean:
	rm -rf mem_func.o livrare.o livrare graph_func.o
