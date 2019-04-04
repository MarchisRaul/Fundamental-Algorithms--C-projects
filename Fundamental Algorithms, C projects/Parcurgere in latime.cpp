#define _CRT_SECURE_NO_WARNINGS

#include<stdlib.h>
#include<stdio.h>

#include "Profiler.h"

Profiler profiler;

// Nume : Marchis Raul, grupa 30229
// Cerinta : Se cere implementarea corecta si eficienta a algoritmului de cautare in latime(Breadth-First Searc - BFS)
// Pentru reprezentarea grafurilor va trebui sa folosesti liste de adiacenta. De asemenea, foloseste pretty-print pentru afisarea
// arborilor / padurilor de arbori rezultati (poti folosi aceeasi metoda de la tema 8) - pentru demo.

// Explicatie algoritm : Algoritmul BFS, primeste un nod din graf, pentru care ii "parcurge" toti vecinii adiacenti lui. Odata ce am parcurs un nod, 
// ii setam culoarea pe "GRI" pentru a putea diferentia nodurile vizitate si nodurile nevizitate. Pe urma, fiecare dintre vecinii lui adiacenti, sunt parcursi
// in acelasi mod pana se trece prin fiecare nod din graf.
// Pentru a fi realizabil acest lucru, nodurile ce se parcurg sunt adaugate intr-o coada, iar la stergerea unui nod din coada se verifica daca nu a fost parcurs inca( daca e "ALB" ), iar in caz afirmativ
// se trece la parcurgerea nodurilor adiacente cu el.

// Complexitatea algoritmului : O(V + E), unde V reprezinta numarul de noduri din graf, iar E reprezinta numarul de muchii din graf.
// Din grafice, se poate observa ca daca variem muchiile, si pastram numarul de noduri constant, algoritmul are de facut mai putine operatii decat daca
// am varia numarul de noduri si am tine numarul de muchii constant. Motivul este ca, in primul caz, numarul de noduri la inceput este 100 si numarul de muchii este abia 1000, 
// spre deosebire de al doilea caz, unde la inceput, numarul de muchii este 4500 si numarul de noduri 100. Putem observa ca, ultima valoare din grafic pentru primul caz,
// cand variem muchiile si pastram numarul de noduri constant, cand ajungem sa avem 100 noduri si 4500 muchii, numarul de atribuiri si comparatii este exact cu prima valoare
// din tabel pentru al doilea caz, cand variem numarul de noduri si avem pentru inceput 100 noduri si 4500 muchii. Asta pentru ca, complexitatea algoritmului este O(nr_noduri + nr_muchii).


int pr0filer = 0;

enum{ALB, GRI, NEGRU};

// structura ce descrie vectorul de liste 
typedef struct listnodetype {
	int key;
	struct listnodetype *next;
}List;

// structura ce descrie coada
typedef struct tailtype {
	int key;
	struct tailtype *next;
}Tail;

// structura tot pentru coada, ca sa folosesc first-ul si last-ul (e defapt o lista inlantuita cu adaugare la final si stergere de la inceput)
typedef struct tail_header {
	Tail *first;
	Tail *last;
}Queue;

// structura ce descrie graful : V(nr_noduri), E(nr_muchii), pi(vector parinti), culoareNod(culoarea fiecarui nod : ALB, GRI, NEGRU), distantaNod(momentul cand a fost parcurs fiecare nod), 
// vector(vectorul de liste pentru a retine nodurile adiacente cu fiecare nod in parte)
typedef struct graph {
	int V;	// nr de noduri
	int E;	// nr de muchii
	int *pi;
	int *culoareNod;
	int *distantaNod;
	List **vector;
}GraphT;




// functie de inserare in vectorul de liste
void insertFirst(List **first, int indice, int key) {
	List *aux = (List*)calloc(1, sizeof(List));
	aux->next = NULL;
	aux->key = key;

	if (first[indice] == NULL)
		first[indice] = aux;
	else
	{
		List *p = first[indice];
		aux->next = p;
		first[indice] = aux;
	}
}

// functie de adaugare a unui element in coada
void enqueue(Queue *Q, int key, int nr_elemente) {
	Tail *aux = (Tail*)calloc(1, sizeof(Tail));
	aux->key = key;
	aux->next = NULL;
	
	//if (pr0filer == 1)
	//	profiler.countOperation("variazaMuchiile", nr_elemente, 1);
	//if (pr0filer == 2)
	//	profiler.countOperation("variazaNodurile", nr_elemente, 1);
	if (Q->first == NULL)
	{
		Q->first = Q->last = aux;
		//if (pr0filer == 1)
		//	profiler.countOperation("variazaMuchiile", nr_elemente, 2);
		//if (pr0filer == 2)
		//	profiler.countOperation("variazaNodurile", nr_elemente, 2);
	}
	else
	{
		Tail *last = Q->last;
		last->next = aux;
		Q->last = aux;

		//if (pr0filer == 1)
		//	profiler.countOperation("variazaMuchiile", nr_elemente, 3);
		//if (pr0filer == 2)
		//	profiler.countOperation("variazaNodurile", nr_elemente, 3);
	}
}

// functie de stergere a unui element de coada si returnarea lui
Tail *dequeue(Queue *Q, int nr_elemente) {
	Tail *aux = (Tail*)calloc(1, sizeof(Tail));
	aux = NULL;

	//if (pr0filer == 1)
	//	profiler.countOperation("variazaMuchiile", nr_elemente, 1);
	//if (pr0filer == 2)
	//	profiler.countOperation("variazaNodurile", nr_elemente, 1);
	if (Q->first != NULL)
	{
	//	if (pr0filer == 1)
	//		profiler.countOperation("variazaMuchiile", nr_elemente, 1);
	//	if (pr0filer == 2)
	//		profiler.countOperation("variazaNodurile", nr_elemente, 1);
		if (Q->first->next == NULL)
		{
			aux = Q->first;
			Q->first = Q->last = NULL;

	//		if (pr0filer == 1)
	//			profiler.countOperation("variazaMuchiile", nr_elemente, 3);
	//		if (pr0filer == 2)
	//			profiler.countOperation("variazaNodurile", nr_elemente, 3);
		}
		else 
		{
			aux = Q->first;
			Q->first = aux->next;

	//		if (pr0filer == 1)
	//			profiler.countOperation("variazaMuchiile", nr_elemente, 2);
	//		if (pr0filer == 2)
	//			profiler.countOperation("variazaNodurile", nr_elemente, 2);
		}
	}

	return aux;
}



// functia verifica unicitatea unei muchii, si daca este unica(nu am mai avut-o pana acum), returneaza 1, altfel, daca nu e unica, returneaza 0
int verificaUnicitateMuchie(List **first, int startAux, int finalAux) {
	List *p = first[startAux];

	while (p != NULL) {
		if (p->key == finalAux)
			return 0;
		p = p->next;
	}

	return 1;
}

// functia pentru crearea grafului random
GraphT *createGraph(int V, int E) {
	GraphT *aux = (GraphT*)calloc(1, sizeof(GraphT));
	aux->V = V;
	aux->E = E;
	aux->pi = (int*)calloc(aux->V, sizeof(int));
	aux->culoareNod = (int*)calloc(aux->V, sizeof(int));
	aux->distantaNod = (int*)calloc(aux->V, sizeof(int));
	aux->vector = (List**)calloc(V, sizeof(List*));

	for (int i = 0; i < V; i++)
	{
		aux->vector[i] = NULL;
		aux->pi[i] = -1;
		aux->culoareNod[i] = ALB;
		aux->distantaNod[i] = INT_MAX;
	}

	int nrMuchiiAux = aux->E;


	while (nrMuchiiAux != 0) {
		int startMuchie = rand() % ((V - 1) + 1 - 0) + 0;
		int finalMuchie = rand() % ((V - 1) + 1 - 0) + 0;


		if (startMuchie != finalMuchie)
		{
			int ok = verificaUnicitateMuchie(aux->vector, startMuchie, finalMuchie);
			if (ok == 1)	// daca muchia e unica, si nu am mai avut-o pana acum
			{
				insertFirst(aux->vector, startMuchie, finalMuchie);
				insertFirst(aux->vector, finalMuchie, startMuchie);

				nrMuchiiAux--;
			}
		}

	}

	return aux;
}

// algoritmul BFS
void BFS(GraphT *graph, int currentNode, Queue *Q, int nr_elemente) {
	graph->culoareNod[currentNode] = GRI;
	graph->distantaNod[currentNode] = 0;
	graph->pi[currentNode] = -1;
	if (pr0filer == 1)
		profiler.countOperation("variazaMuchiile", nr_elemente, 3);
	if (pr0filer == 2)
		profiler.countOperation("variazaNodurile", nr_elemente, 3);

	enqueue(Q, currentNode, nr_elemente);

	while (Q->first != NULL)
	{

		Tail *nodScos = dequeue(Q, nr_elemente);
		if (pr0filer == 0)
			printf("%d, ", nodScos->key);
		List *v = graph->vector[nodScos->key];

		if (pr0filer == 1)
			profiler.countOperation("variazaMuchiile", nr_elemente, 3);
		if (pr0filer == 2)
			profiler.countOperation("variazaNodurile", nr_elemente, 3);

		while (v != NULL)
		{
			if (pr0filer == 1)
				profiler.countOperation("variazaMuchiile", nr_elemente, 2);
			if (pr0filer == 2)
				profiler.countOperation("variazaNodurile", nr_elemente, 2);

			if (graph->culoareNod[v->key] == ALB)
			{
				graph->culoareNod[v->key] = GRI;
				graph->distantaNod[v->key] = graph->distantaNod[nodScos->key] + 1;
				graph->pi[v->key] = nodScos->key;
				enqueue(Q, v->key, nr_elemente);

				if (pr0filer == 1)
					profiler.countOperation("variazaMuchiile", nr_elemente, 3);
				if (pr0filer == 2)
					profiler.countOperation("variazaNodurile", nr_elemente, 3);
			}
			v = v->next;
			if (pr0filer == 1)
				profiler.countOperation("variazaMuchiile", nr_elemente, 1);
			if (pr0filer == 2)
				profiler.countOperation("variazaNodurile", nr_elemente, 1);
		}
		graph->culoareNod[nodScos->key] = NEGRU;
		if (pr0filer == 1)
			profiler.countOperation("variazaMuchiile", nr_elemente, 1);
		if (pr0filer == 2)
			profiler.countOperation("variazaNodurile", nr_elemente, 1);
	}
}

// in caz ca graful nu este conex, sa fie asigurata parcurgerea nodurilor albe care probabil se afla intr-o alta componenta conexa
void BFS_VISIT(GraphT *graph, int nr_elemente) {
	Queue Q = { NULL, NULL };
	
	for (int i = 0; i < graph->V; i++)
	{
		if (graph->culoareNod[i] == ALB)
			BFS(graph, i, &Q, nr_elemente);
	}
}

// pretty print pentru vectorul de parinti
void prettyPrintParentTree(int *vectorParinti, int nrElementeVectorParinti, int curentNode, int level) {

	for (int i = 0; i < nrElementeVectorParinti; i++)
	{

		if (vectorParinti[i] == curentNode)
		{
			for (int k = 0; k <= level; k++)
				printf("  ");
			printf("%d\n\n\n", i);
			prettyPrintParentTree(vectorParinti, nrElementeVectorParinti, i, level + 2);
		}

	}
}

int main() {
	srand(time(NULL));

	
	// start demo pentru BFS
	int V = 6;
	int E = 2 * V;

	GraphT *graph = NULL;
	graph = createGraph(V, E);

	
	printf("Nodurile impreuna cu vecinii lor: \n");
	List *p;
	for (int i = 0; i < V; i++)
	{
		p = graph->vector[i];
		printf("%d: ", i);
		while (p != NULL)
		{
			printf("%d -> ", p->key);
			p = p->next;
		}
		printf("\n");
	}
	
	printf("\n\n");
	printf("Parcurgerea in latime a grafului este: \n");
	BFS_VISIT(graph, 0);

	printf("\n\n\n");
	printf("Pretty print: \n\n");
	prettyPrintParentTree(graph->pi, graph->V, -1, 0);
	

	for (int i = 0; i < V; i++)
	{
		List *p = graph->vector[i];
		while (p != NULL)
		{
			List *q = p;
			p = p->next;
			free(q);
		}
		graph->vector[i] = NULL;
	}
	free(graph->pi);
	free(graph->culoareNod);
	free(graph->distantaNod);
	free(graph->vector);
	free(graph);
	// stop demo pentru BFS

	
	pr0filer++;
	// generarea primul grafic, variaza numarul de muchii (E)
	V = 100;
	for (int E = 1000; E <= 4500; E += 100)
	{
		graph = createGraph(V, E);
		BFS_VISIT(graph, E);

		for (int i = 0; i < V; i++)
		{
			List *p = graph->vector[i];
			while (p != NULL)
			{
				List *q = p;
				p = p->next;
				free(q);
			}
			graph->vector[i] = NULL;
		}
		free(graph->pi);
		free(graph->culoareNod);
		free(graph->distantaNod);
		free(graph->vector);
		free(graph);
	}
	


	// generarea celui de-al doilea grafic, variaza numarul de noduri
	pr0filer++;
	E = 4500;
	for (V = 100; V <= 200; V += 10) {
		graph = createGraph(V, E);
		BFS_VISIT(graph, V);

		for (int i = 0; i < V; i++)
		{
			List *p = graph->vector[i];
			while (p != NULL)
			{
				List *q = p;
				p = p->next;
				free(q);
			}
			graph->vector[i] = NULL;
		}
		free(graph->pi);
		free(graph->culoareNod);
		free(graph->distantaNod);
		free(graph->vector);
		free(graph);
	}

	profiler.showReport();

	getchar();
}