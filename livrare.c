//	Tema 3 - Structuri de Date
//	Livrare de produse folosind drone
//
//	Autor: Berbece Daniel
//	Grupa: 311CC

//	Tabsize recommended: 4 spaces.

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "graph_func.h"
#include "mem_func.h"

#define INFINIT 999999999

int GetClosestFarm(int currentNode, int destNode, int farmType, graph *G){
//	Functia intoarce indicele nodului ferma pentru care se obtine distanta
//	minima intre nodul curent si urmatorul nod client.
//	Paramtrii primiti sunt: nodul curent, nodul client urmator, tipul fermei
//	pe care trebuie sa il intoarca si structura de graf.

	int k;
	int distSum = INFINIT;
	int farmNode;

	//	Parcurgem fiecare nod din graf si verificam daca nodul este ferma, de tipul
	//	dat ca parametru si daca distanta pana la el si de la el pana la urmatorul 
	//	client este mai mica decat distanta curenta. Daca e necesar, actualizam 
	//	distanta si nodul ferma
	for(k = 0; k < G -> n; k++){
		if( G -> v[k] == farmType && G -> d[currentNode][k] + G -> d[k][destNode] < distSum ){
			distSum = G -> d[currentNode][k] + G -> d[k][destNode];
			farmNode = k;
		}
	}

	//	Intoarcem indicele fermei:
	return farmNode;
}

int GetClosestFinalFarm(int currentNode, graph *G){
//	Functia intoarce ferma cea mai apropiata de nodul primit ca paramtru,
//	indiferent de tipul fermei.

	int k;
	int distSum = INFINIT;
	int farmNode;

	//	Parcurgem fiecare nod din graf. Daca este ferma si distanta pana la
	//	el este mai mica decat distanta curenta, updatam distanta si nodul
	for(k = 0; k < G -> n; k++){
		if( G -> v[k] != 0 && G -> d[currentNode][k] < distSum ){
			distSum = G -> d[currentNode][k];
			farmNode = k;
		}
	}

	//	Intoarcem indicele fermei:
	return farmNode;
}

void ShowFullPathAndDistance(int *nodes, int deliveries, FILE *outfile, int *farmTypes, graph *G){
//	Functia afiseaza nodurile prin care se trece pentru ca drona sa faca 
//	livrarile pe un drum minim si distanta parcursa de drona in total pana
//	la intoarcerea la cea mai apropiata ferma.
//	Parametrii sunt: un vector de noduri clienti, la care trebuie drona sa ajunga
//	in ordinea din vector. Primul element din vector este nodul de plecare.
//	deliveries - numarul de noduri clienti, farmTypes - vector care retine
//	tipul comenzilor, fisierul de output, necesar afisarii si graful G.

	int i;
	//	Afisam nodul de plecare:
	fprintf(outfile, "%d ", nodes[0]);
	int farmNode;
	//	Initializam distanta parcursa:
	int distance = 0;

	//	Primul caz este mai special, deoarece drona poate pleca
	//	dintr-o ferma de acelasi tip precum comanda sau nu:
	farmNode = GetClosestFarm(nodes[0], nodes[1], farmTypes[1], G);
	//	Daca ferma cea mai apropiata nu este nodul de plecare, afisam
	//	drumul pana la urmatorul nod cu escala prin ferma. Altfel
	//	afisam drumul direct catre urmatorul nod client.
	if( farmNode != nodes[0] ){
		ShowIntermediarNodes(G -> N, nodes[0], farmNode, outfile);
		distance += G -> d[nodes[0]][farmNode];
		fprintf(outfile, "%d ", farmNode);
		ShowIntermediarNodes(G -> N, farmNode, nodes[1], outfile);
		distance += G -> d[farmNode][nodes[1]];
		fprintf(outfile, "%d ", nodes[1]);
	} else {
		ShowIntermediarNodes(G -> N, nodes[0], nodes[1], outfile);
		distance += G -> d[nodes[0]][nodes[1]];
		fprintf(outfile, "%d ", nodes[1]);
	}

	//	Pentru restul comenzilor, algoritmul este identic. Se trece ajunge la
	//	urmatorul client cu escala la o ferma anume, pe care o cautam in
	//	functia GetClosestFarm. De fiecare data adaugam distanta parcursa 
	//	la variabila distance
	for(i = 1; i < deliveries; i++){
		
		farmNode = GetClosestFarm(nodes[i], nodes[i + 1], farmTypes[i + 1], G);
		ShowIntermediarNodes(G -> N, nodes[i], farmNode, outfile);
		distance += G -> d[nodes[i]][farmNode];
		fprintf(outfile, "%d ", farmNode);
		ShowIntermediarNodes(G -> N, farmNode, nodes[i + 1], outfile);
		fprintf(outfile, "%d ", nodes[i + 1]);
		distance += G -> d[farmNode][nodes[i + 1]];
	}

	//	Dupa ce am terminat vectorul de clienti, drona trebuie sa
	//	ajunga la ferma cea mai apropiata de ultimul client.
	//	Gasim ferma si afisam drumul pana la ea. Afisam la sfarsit
	//	distanta parcursa de drona de la plecare pana la final.
	farmNode = GetClosestFinalFarm(nodes[i], G);
	ShowIntermediarNodes(G -> N, nodes[i], farmNode, outfile);
	distance += G -> d[nodes[i]][farmNode];
	fprintf(outfile, "%d\n%d\n", farmNode, distance);
}

int IsBestDistance(int *perm, int *bestdist, int *types, int n, graph *G){
//	Functia intoarce TRUE daca drumul intre nodurile primite ca
//	parametru in vectorul perm si intoarcerea la cea mai apropiata ferma dupa
//	terminarea clientilor este mai mic decat distanta minima curenta, primita
//	ca parametru prin adresa acesteia, pentru modificarea ei totodata.
	int i;
	//	Initializam distanta pentru traseul primit:
	int currdist = 0;
	int farmNode;

	//	O calculcam:
	for(i = 0; i < n; i++){
		farmNode = GetClosestFarm(perm[i], perm[i + 1], types[i + 1], G);
		currdist += G -> d[perm[i]][farmNode] + G -> d[farmNode][perm[i + 1]];
	}
	farmNode = GetClosestFinalFarm(perm[i], G);
	currdist += G -> d[perm[i]][farmNode];

	//	Daca e mai mica, modificam *bestdist si intoarcem 1:
	if( currdist < *bestdist ){
		*bestdist = currdist;
		return 1; 
	} else {
		return 0;
	}
}

void SwapNumbers(int *a, int *b){
//	Functia schimba doua numere intre ele. Primeste ca parametru
//	pointeri la cele doua numere de tip integer.
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

void back(int k, int n, int *permutation, int *tmpdest, 
		  int *permTypes, int *tmptype, int *bestdist, graph *G){
//	Functia back genereaza toate permutarile comenzilor primite in vectorul tmpdest
//	impreuna cu tipurile comenzilor si salveaza in vectorul permutation secventa de
//	comenzi pentru care drona parcurge cea mai mica distanta pentru a livra comenzile
	
	int i,j;
	//	permutam pozitia k cu toate care urmeaza:
	for(i = k; i <= n; i++){

		SwapNumbers(&tmpdest[k], &tmpdest[i]);
		SwapNumbers(&tmptype[k], &tmptype[i]);
		//	Daca ajungem la sfarsit si descoperim o distanta mai buna,
		//	salvam permuatarea:
		if( k == n ){
			if( IsBestDistance(tmpdest, bestdist, tmptype, n, G) ){
				for(j = 0; j <= n; j++){
					permutation[j] = tmpdest[j];
					permTypes[j] = tmptype[j];
				}
			}
		} else {
			//	Altfel, daca nu am terminat de generat permutarile, continuam recursiv
			back(k + 1, n, permutation, tmpdest, permTypes, tmptype, bestdist, G);
		}

		//	Schimbam inapoi cele doua numere: tmpdest si tmptype.
		SwapNumbers(&tmpdest[k], &tmpdest[i]);
		SwapNumbers(&tmptype[k], &tmptype[i]);
	}
}

void Task1(graph *G, FILE *infile, FILE *outfile){

	//	Initializam variabilele:
	int i;
	int deliveries;
	int destNode;
	int tmp;
	int farmNode;
	int distance = 0;

	//	Vedem care este singurul nod ferma, pentru a sti nodul
	//	de plecare al dronei
	for(i = 0; i < G -> n; i++){
		if(G -> v[i] == 1){
			farmNode = i;
			break;
		}
	}

	//	In timp ce citim nodurile afisam si traseul minim:
	fscanf(infile, "%d", &deliveries);
	for(i = 0; i < deliveries; i++){
		fscanf(infile, "%d %d", &tmp, &destNode);
		// 	Drona merge pana la destinatie si apoi se intoarce la unica ferma	
		fprintf(outfile, "%d ", farmNode);
		ShowIntermediarNodes(G -> N, farmNode, destNode, outfile);
		fprintf(outfile, "%d ", destNode);
		ShowIntermediarNodes(G -> N, destNode, farmNode, outfile);
		//	Actualizam distanta parcursa de drona
		distance += G -> d[farmNode][destNode] + G -> d[destNode][farmNode];
	}
	//	Afisam ultimul nod, adica ferma, si distanta parcursa de drona:
	fprintf(outfile, "%d\n%d\n", farmNode, distance);
}

void Task2(graph *G, FILE *infile, FILE *outfile){

	int i;
	int deliveries;
	//	Citim numarul de comenzi si citim comenzile in 2 vectori:
	//	unul pentru nodurile destinatie si unul pentru tipurile comenzilor
	fscanf(infile, "%d", &deliveries);
	int *destinations = calloc(deliveries + 1, sizeof(int));
	int *farmTypes = calloc(deliveries + 1, sizeof(int));

	for(i = 0; i < deliveries; i++){
		fscanf(infile, "%d %d", &farmTypes[i + 1], &destinations[i + 1]);
	}
	//	Citim la sfarsit nodul de plecare si il punem la inceputul vectorului
	fscanf(infile, "%d", &destinations[0]);

	//	Afisarea drumului minim se realizeaza in functia urmatoare: 
	ShowFullPathAndDistance(destinations, deliveries, outfile, farmTypes, G);

	free(destinations);
	free(farmTypes);
}

void Task3(graph *G, FILE *infile, FILE *outfile){

	int i;
	int deliveries;
	int farmNode;

	//	Citim numarul de comenzi si generam vectorii de comenzi:
	fscanf(infile, "%d", &deliveries);
	int *destinations = calloc(deliveries, sizeof(int));
	int *farmTypes = calloc(deliveries, sizeof(int));

	for(i = 0; i < deliveries; i++){
		fscanf(infile, "%d %d", &farmTypes[i], &destinations[i]);
	}
	//	Citim nodul de plecare:
	fscanf(infile, "%d", &farmNode);

	//	Cautam path-ul in care se parcurge cea mai mica distanta. 
	//	Pentru asta generam toate permutarile prin functia back.

	int *permutation = calloc(deliveries + 2, sizeof(int));
	int *permTypes = calloc(deliveries + 2, sizeof(int));
	int *tmpdest = calloc(deliveries + 2, sizeof(int));
	int *tmptype = calloc(deliveries + 2, sizeof(int));
	
	tmpdest[0] = farmNode;
	for(i = 0; i < deliveries; i++){
		tmpdest[i+1] = destinations[i];
		tmptype[i + 1] = farmTypes[i];
	}
	
	//	Initializam distanta parcursa minima cu "infinit"
	int *bestdist = calloc(1, sizeof(int));
	*bestdist = INFINIT;

	//	Generam permutarile, pornind cu back de 1, deoarece pe 0 avem nodul
	//	de plecare si nu vrem sa il schimbam
	back(1, deliveries, permutation, tmpdest, permTypes, tmptype, bestdist, G);

	//	Odata ce am gasit permutarea minima a comenzilor, afisam path-ul:
	ShowFullPathAndDistance(permutation, deliveries, outfile, permTypes, G);

	free(permutation);
	free(permTypes);
	free(tmpdest);
	free(tmptype);
	free(bestdist);
	free(destinations);
	free(farmTypes);
}

int isempty(int *vector, int n){
//	Functia verifica daca un vector este plin de 1. 
//	Daca da, intoarce 1, 0 altfel
	int i;
	for(i = 0; i < n; i++){
		if(vector[i] != 1)	return 0;
	}
	return 1;
}

void bonus(graph *G, FILE *infile, FILE *outfile){

	int i,j;
	int deliveries;
	int alpha, beta;

	//	Citim comenzile in 2 vectori: unul pentru noduri
	//	si altul pentru tipul comenzilor
	fscanf(infile, "%d", &deliveries);
	int *destinations = calloc(deliveries, sizeof(int));
	int *farmTypes = calloc(deliveries, sizeof(int));

	for(i = 0; i < deliveries; i++){
		fscanf(infile, "%d %d", &farmTypes[i], &destinations[i]);
	}
	//	Citim alpha si beta, folosite la calcularea profitabilitatii
	fscanf(infile, "%d %d", &alpha, &beta);

	//	Gasim componentele tare conexe ale grafului:
	ComponenteTareConexe(G);
	//	Afisam numarul de componente tare conexe:
	fprintf(outfile, "%d\n", G -> nrCTC);
	//	In urmatorul vector vom retine profitabilitatea pentru fiecare componenta:
	int *profitabilitate = calloc(G -> nrCTC, sizeof(int));

	//	Pentru fiecare componenta tare conexa, gasim nodul ferma de plecare
	//	si comenzile aflate in acea componenta. Pentru aceste comenzi, gasim
	//	secventa care genereaza un drum minim parcurs de drona si afisam
	//	path-ul corespunzator
	for(i = 1; i <= G -> nrCTC; i++){
		int farmNode;
		//	Gasim nodul ferma cu indicele minim din componenta conexa i:
		for(j = 0; j < G -> n; j++){
			if( G -> vectorComponente[j] == i && G -> v[j] != 0 ){
				farmNode = j;
				break;
			}
		}
		//	Gasim numarul de noduri din componenta i:
		int cnt = 0;
		for(j = 0; j < G -> n; j++){
			if( G -> vectorComponente[j] == i )
				cnt ++;
		}
		//	Initializam vectorii de permutari pentru a calcula permutarea minima:
		int *permutation = calloc(cnt, sizeof(int));
		int *permTypes = calloc(cnt, sizeof(int));
		permutation[0] = farmNode;
		cnt = 1;
		for(j = 0; j < deliveries; j++){
			if(G -> vectorComponente[destinations[j]] == i){
				permutation[cnt] = destinations[j]; 
				permTypes[cnt++] = farmTypes[j];
			}
		}
		int *tmpdest = calloc(deliveries + 2, sizeof(int));
		int *tmptype = calloc(deliveries + 2, sizeof(int));
		
		//	Si distanta minima:
		int *bestdist = calloc(1, sizeof(int));
		*bestdist = INFINIT;

		//	Generam secventa de comenzi cu traseu minim:
		back(1, cnt - 1, tmpdest, permutation, tmptype, permTypes, bestdist, G);
		//	Si salvam profitabilitatea in vector:
		profitabilitate[i - 1] = alpha*(cnt - 1) - beta*(*bestdist);

		free(permutation);
		free(permTypes);
		free(tmpdest);
		free(tmptype);
		free(bestdist);
	}

	//	Afisam acum fiecare componenta tare conexa impreuna cu profitabilitatea
	//	componentei, in ordine descrescatoare a profitabilitatii:
	int *flags = calloc(G -> nrCTC, sizeof(int));
	while( !isempty(flags, G -> nrCTC) ){
	//	Cat timp nu am afisat toate componentele:
		int max = -INFINIT; int ci;
		//	Gasim componenta ce trebuie afisata:
		for(i = 0; i < G -> nrCTC; i++){
			if( !flags[i] && max < profitabilitate[i]){
				max = profitabilitate[i];
				ci = i; // Salvam in "ci" componenta cu profitabilitatea maxima.
			}
		}

		//	Afisam nodurile din componenta "ci" si profitabilitatea:
		for(j = 0; j < G -> n; j++){
			if( G -> vectorComponente[j] == ci + 1 )
				fprintf(outfile, "%d ", j);
		}
		fprintf(outfile, "%d\n", profitabilitate[ci]);
		flags[ci] = 1;
	}

	free(destinations);
	free(farmTypes);
	free(profitabilitate);
	free(flags);
}

int main(int argn, char *argv[]){

	//	Deschidem fisierele de intrare si iesire:
	FILE *infile = fopen(argv[1], "r");
	FILE *outfile = fopen(argv[2], "w");

	//	Citim tipul taskului care trebuie rezolvat
	int tasktype;
	fscanf(infile, "%d", &tasktype);

	// Indiferent de task, cream graful:
	graph *G = CreateGraph(infile);

	//	In functie de tipul cerintei, mergem pe una din functii
	switch(tasktype){
		case 1:
			Task1(G, infile, outfile);
			break;
		case 2:
			Task2(G, infile, outfile);
			break;
		case 3:
			Task3(G, infile, outfile);
			break;
		case 4:
			bonus(G, infile, outfile);
			break;
		default:
			FreeGraph(&G);
			return 1;
	}
	
	//	Eliberam memoria grafului:
	FreeGraph(&G);
	//	Si inchidem fisierele:
	fclose(infile);
	fclose(outfile);

	return 0;
}