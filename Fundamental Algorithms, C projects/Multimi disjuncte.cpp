#define _CRT_SECURE_NO_WARNINGS

#include<stdlib.h>
#include<stdio.h>
#include "Profiler.h"

// Nume : Marchis Raul
// Se cere implementarea corecta si eficienta a operatiilor de baza pe multimi disjuncte (capitolul
// 21.1 din carte) si a algoritmului lui Kruskal(gasirea arborelui de acoperire minima) folosing multimi
// disjuncte (capitolul 23.2)

// Descrierea algoritmului : Pentru a genera graful aleator, am facut in felul urmator : am conectat nodul 0 cu toate celelalte noduri din graf, pentru a asigura
// conexitatea grafului. Pe urma, pentru restul de muchii ramase, am generat aleator doua noduri, ce reprezinta capetele unei muchii. In cazul in care muchia nu
// a aparut deja, o introducem in graf (vectorul de muchii al grafului). Pentru a verifica daca o muchie se repeta, am creeat un vector de liste, unde fiecare indice
// din vector reprezinta un nod din graf. Fiecare vecin a nodului, l-am introdus in lista sa, iar astfel, la generarea a unei muchii reprezentate prin capetele n si m,
// daca in lista lui n, nu il gasim pe m, inseamna ca muchia este buna (si daca n si m sunt diferite, bineinteles).
// Pe urma, urmeaza sortarea muchiilor in ordine crescatoare a costurilor, pentru care am folosit algoritmul QuickSort. Dupa ce facem make-set pentru fiecare nod in parte(
// (punem parintele nodului sa pointeze spre el insusi), parcurgem vectorul de muchii gata sortate, iar daca parintii capetelor unei muchii sunt diferiti (find-set-urile
// pentru fiecare nod difera), atunci unim cele doua noduri (union) si punem muchia in arborele de acoperire minim.

// Complexitatea operatiilor pentru algoritmul Kruskal:
// MAKE-SET O(V), QuickSort O(ElogE), iar for-ul din interiorul algoritmului este O(E).


Profiler profiler;

int pr0filer = 0;

// structura ce descrie tipul unui nod pentru multimile disjuncte
typedef struct nodetype {
	int key;
	struct nodetype *parent;
	int rang;
}NodeT;

// structura ce descrie o muchie
typedef struct edgetype {
	int startOfEdge;
	int finalOfEdge;
	int cost;
}EdgeT;

// structura ce descrie graful
typedef struct graph {
	int V;	// nr de noduri
	int E;	// nr de muchii

	EdgeT *edges;
}GraphT;

// structura ce descrie vectorul de liste pentru a verifica unicitatea generarii unei muchii random
typedef struct listnodetype {
	int key;
	struct listnodetype *next;
}List;




// start operatii pe multimi disjuncte

void makeSet(NodeT *x) {
	x->parent = x;
	x->rang = 0;
}


NodeT *findSet(NodeT *x, int nr_elemente) {
	NodeT *currentNode = x;
	while (currentNode->parent != currentNode)
	{
		currentNode = currentNode->parent;
		if (pr0filer == 1)
			profiler.countOperation("operatiiKruskal", nr_elemente, 2);
	}
		return currentNode;
}

void LINK(NodeT *x, NodeT *y, int nr_elemente)
{
	if (pr0filer == 1)
		profiler.countOperation("operatiiKruskal", nr_elemente, 1);
	if (x->rang > y->rang)
	{
		y->parent = x;
		if (pr0filer == 1)
			profiler.countOperation("operatiiKruskal", nr_elemente, 1);
	}
	else
	{
		x->parent = y;
		if (pr0filer == 1)
			profiler.countOperation("operatiiKruskal", nr_elemente, 1);
	}

	if (pr0filer == 1)
		profiler.countOperation("operatiiKruskal", nr_elemente, 1);
	if (x->rang == y->rang)
	{
		(y->rang)++;
		if (pr0filer == 1)
			profiler.countOperation("operatiiKruskal", nr_elemente, 1);
	}
}

void unionSet(NodeT *firstNode, NodeT *secondNode, int nr_elemente) {
	LINK(findSet(firstNode, nr_elemente), findSet(secondNode, nr_elemente), nr_elemente);
}

// stop operatii pe multimi disjuncte



NodeT *newNode(int key) {
	NodeT *aux = (NodeT*)calloc(1, sizeof(NodeT));
	aux->key = key;
	aux->parent = NULL;
	aux->rang = 0;

	return aux;
}

// functie de inserare in liste pentru a verifica unicitatea muchiilor
void insertFirst(List **first, int i, int key) {
	List *aux = (List*)calloc(1, sizeof(List));
	aux->next = NULL;
	aux->key = key;

	if (first[i] == NULL)
		first[i] = aux;
	else
	{
		List *p = first[i];
		aux->next = p;
		first[i] = aux;
	}
}

// start implementare quicksort si adaptare pentru operatii pe muchii
int partition(EdgeT arr[], int low, int high, int nr_elemente)
{
	int pivot = arr[high].cost;
	int i = (low - 1);

	for (int j = low; j <= high - 1; j++)
	{
		if (arr[j].cost <= pivot)
		{
			i++;
			EdgeT aux = arr[i];
			arr[i] = arr[j];
			arr[j] = aux;
		}
	}

	EdgeT aux = arr[i + 1];
	arr[i + 1] = arr[high];
	arr[high] = aux;
	
	return (i + 1);
}

void quickSort(EdgeT arr[], int low, int high, int nr_elemente)
{
	if (low < high)
	{
		int pi;
		pi = partition(arr, low, high, nr_elemente);

		quickSort(arr, low, pi - 1, nr_elemente);
		quickSort(arr, pi + 1, high, nr_elemente);
	}
}
// stop implementare quicksort si adaptare pentru operatii pe muchii


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
GraphT *createGraph(int V, int E, List **first) {
	GraphT *aux = (GraphT*)calloc(1, sizeof(GraphT));
	aux->V = V;
	aux->E = E;
	aux->edges = (EdgeT*)calloc(aux->E, sizeof(EdgeT));

	
	int nrMuchiiAux = aux->E;
	int indiceContinuare = 0;
	for (int i = 1; i < V; i++)
	{
		aux->edges[i - 1].cost = rand() % (10000 + 1 - 1) + 1;
		//aux->edges[i - 1].cost = rand() % 10000 + 1;
		aux->edges[i - 1].startOfEdge = 0;
		aux->edges[i - 1].finalOfEdge = i;
		insertFirst(first, aux->edges[i - 1].startOfEdge, aux->edges[i - 1].finalOfEdge);
		insertFirst(first, aux->edges[i - 1].finalOfEdge, aux->edges[i - 1].startOfEdge);

		nrMuchiiAux--;
		indiceContinuare = i;
	}

	indiceContinuare++;

	while (nrMuchiiAux != 0) {
		int startAux = rand() % ((V-1) + 1 - 1) + 1;
		int finalAux = rand() % ((V-1) + 1 - 1) + 1;
		//int startAux = rand() % (V-1) + 1;
		//int finalAux = rand() % (V-1) + 1;
	
	
		if (startAux != finalAux)
		{
			int ok = verificaUnicitateMuchie(first, startAux, finalAux);
			if (ok == 1)	// daca muchia e unica, si nu am mai avut-o pana acum
			{
				aux->edges[indiceContinuare - 1].cost = rand() % (10000 + 1 - 1) + 1;
				//aux->edges[indiceContinuare - 1].cost = rand() % 10000 + 1;
				aux->edges[indiceContinuare - 1].startOfEdge = startAux;
				aux->edges[indiceContinuare - 1].finalOfEdge = finalAux;
				insertFirst(first, aux->edges[indiceContinuare - 1].startOfEdge, aux->edges[indiceContinuare - 1].finalOfEdge);
				insertFirst(first, aux->edges[indiceContinuare - 1].finalOfEdge, aux->edges[indiceContinuare - 1].startOfEdge);

				nrMuchiiAux--;
				indiceContinuare++;
			}
		}
		
	}

	return aux;
}

// algoritmul lui KRUSKAL
EdgeT* MST_KRUSKAL(GraphT *graph, NodeT *nodes, int nr_elemente) {
	int nrMuchiiFinale = graph->V - 1;
	EdgeT *finalEdges = (EdgeT*)calloc(nrMuchiiFinale, sizeof(EdgeT));
	int indice = 0;

	for(int i = 0; i < graph->V; i++)
	{
		if (pr0filer == 1)
			profiler.countOperation("operatiiKruskal", nr_elemente, 1);
		makeSet(&nodes[i]);
	}

	quickSort(graph->edges, 0, graph->E - 1, 1);

	for(int i=0; i<graph->E; i++)
		if (findSet(&nodes[graph->edges[i].startOfEdge], nr_elemente) != findSet(&nodes[graph->edges[i].finalOfEdge], nr_elemente))
		{
			finalEdges[indice] = graph->edges[i];
			indice++;
			unionSet(&nodes[graph->edges[i].startOfEdge], &nodes[graph->edges[i].finalOfEdge], nr_elemente);

			if (indice == nrMuchiiFinale)
				break;
		}
	return finalEdges;
}

int main() {
	srand(time(NULL));

	// inceput demo pentru multimi disjuncte
	
	NodeT *node1 = newNode(1);
	NodeT *node2 = newNode(2);
	NodeT *node3 = newNode(3);
	NodeT *node4 = newNode(4);
	NodeT *node5 = newNode(5);
	NodeT *node6 = newNode(6);
	NodeT *node7 = newNode(7);
	NodeT *node8 = newNode(8);
	NodeT *node9 = newNode(9);
	NodeT *node10 = newNode(10);

	makeSet(node1);
	makeSet(node2);
	makeSet(node3);
	makeSet(node4);
	makeSet(node5);
	makeSet(node6);
	makeSet(node7);
	makeSet(node8);
	makeSet(node9);
	makeSet(node10);

	unionSet(node2, node4, 0);
	unionSet(node3, node5, 0);
	unionSet(node2, node5, 0);
	unionSet(node8, node3, 0);
	unionSet(node9, node10, 0);
	
	printf("Parintele nodului 2 : %d, Rangul parintelui nodului 2 : %d\n", node2->parent->key, node2->parent->rang);
	printf("Parintele nodului 3 : %d, Rangul parintelui nodului 3 : %d\n", node3->parent->key, node3->parent->rang);
	printf("Parintele nodului 4 : %d, Rangul parintelui nodului 4 : %d\n", node4->parent->key, node4->parent->rang);
	printf("Parintele nodului 5 : %d, Rangul parintelui nodului 5 : %d\n", node5->parent->key, node5->parent->rang);
	printf("Parintele nodului 8 : %d, Rangul parintelui nodului 8 : %d\n", node8->parent->key, node8->parent->rang);
	printf("Parintele nodului 9 : %d, Rangul parintelui nodului 9 : %d\n", node9->parent->key, node9->parent->rang);
	printf("Parintele nodului 10 : %d, Rangul parintelui nodului 10 : %d\n", node10->parent->key, node10->parent->rang);
	
	printf("\n\n\n");
	// sfarsit demo pentru multimi disjuncte

	// start demo pentru kruskal
	List *vector[10000];
	for (int i = 0; i < 10000; i++)
		vector[i] = NULL;
	int V = 6;
	int E = 2 * V;
	NodeT *nodes = (NodeT*)calloc(V, sizeof(NodeT));
	GraphT *graph = NULL;
	graph = createGraph(V, E, vector);
	
	printf("Nodurile impreuna cu vecinii lor: \n");
	List *p;
	for (int i = 0; i < V; i++)
	{
		p = vector[i];
		printf("%d: ", i);
		while (p != NULL)
		{
			printf("%d -> ", p->key);
			p = p->next;
		}
		printf("\n");
	}
	
	printf("\n\n");
	for (int i = 0; i < graph->E; i++)
		printf("Cost : %d, start muchie : %d - final muchie : %d\n", graph->edges[i].cost, graph->edges[i].startOfEdge, graph->edges[i].finalOfEdge);

	printf("\n\n\n");
	printf("Arborele de acoperire minim contine urmatoarele muchii impreuna cu costurile lor: \n");
	EdgeT *finalEdges = MST_KRUSKAL(graph, nodes, V);
	for (int i = 0; i < graph->V - 1; i++)
		printf("Cost %d, start %d -> final %d\n", finalEdges[i].cost, finalEdges[i].startOfEdge, finalEdges[i].finalOfEdge);

	free(graph->edges);
	for (int i = 0; i < 10000; i++)
	{
		List *p = vector[i];
		while (p != NULL)
		{
			List *q = p;
			p = p->next;
			free(q);
		}
		vector[i] = NULL;
	}
	free(graph);
	free(nodes);
	// stop demo pentru kruskal

	pr0filer++;	// acum o sa inceapa sa contorizeze operatiile

	for (int n = 100; n < 10000; n += 100)
	{
		for (int q = 0; q < 5; q++)
		{
			V = n;
			E = 4 * V;
			nodes = (NodeT*)calloc(V, sizeof(NodeT));
			graph = NULL;
			graph = createGraph(V, E, vector);
			finalEdges = MST_KRUSKAL(graph, nodes, n);

			free(graph->edges);
			for (int i = 0; i < 10000; i++)
			{
				List *p = vector[i];
				while (p != NULL)
				{
					List *q = p;
					p = p->next;
					free(q);
				}
				vector[i] = NULL;
			}
			free(graph);
			free(nodes);
		}
	}


	profiler.divideValues("operatiiKruskal", 5);
	profiler.showReport();
	getchar();
}