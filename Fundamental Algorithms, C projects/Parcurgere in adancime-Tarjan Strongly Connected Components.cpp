#define _CRT_SECURE_NO_WARNINGS

// Marchis Raul , grupa 30229
// Se  cere implementarea corecta si eficienta a algoritmului de cautare in adancime (Depth-First Search - DFS).
// Pentru reprezentarea grafurilor va trebui sa folosesti liste de adiacenta. De asemenea va trebui sa:
//	- implementarea algoritmului Tarjan pentru componente tare conexe
//	- implementezi sortarea topologica



// Algoritmul DFS : Spre deosebire de BFS, unde parcurgerea se face "in latime", la DFS parcurgerea se face in adancime. Asta inseamna ca,
// pentru fiecare nod in parte, ii vom exlora primul vecin, urmand ca pentru acest prim vecin sa ii exploram si lui primul nod adiacent cu el,
// si asa mai departe. Spre exemplu, incepem cu nodul 0, care ii are vecini pe 1 si pe 2. Incepem si ii exploram primul vecin al nodului 1,
// urmand sa vizitam apoi primul vecin al nodului 1 si asa mai departe. Cand terminam de explorat "toti vecinii" pentru nodul 1, algoritmul va reveni
// din recursivitate la nodul 0, astfel ca vom trece si-l vom parcurge pe 2. Astfel, elementele parcurse nu vor mai fi adaugate intr-o coada,
// ci teoretic vor fi adaugate pe "o stiva" (intrucat algoritmul este implementat recursiv, apelurile recursive se considera a fi adaugate "pe stiva".
// EFICIENTA ALGORITMULUI : O(V + E), unde V reprezinta numarul de noduri, iar E reprezinta numarul de muchii.


// Sortarea topologica : o explicatie in limbaj natural ar fi urmatoarea: pentru fiecare muchie u -> v din graf, in sortarea topologica, u apare inaintea lui v.
// Aceasta sortare topologica este posibila doar daca graful NU CONTINE CICLURI! Pentru a verifica daca nu avem cicluri, pe parcursul DFS-ului, daca vecinul unui nod
// este gri, inseamna ca a fost deja vizitat si avem un ciclu, intrucat nodul curent "se poate intoarce la nodul deja vizitat".
// Odata ce finalizam un nod, il adaugam "intr-o stiva" (o lista simplu inlantuita, cu adaugare si stergere de la inceput). Daca nu am avut cicluri,
// afisam sortarea topologica si se poate vedea ca pentru fiecare muchie u->v din graf, nodul u apare inaintea nodului v.


// Algoritmul lui Tarjan pentru determinarea componentelor tare conexe : o componenta conexa, reprezinta "un ciclu" intr-un graf orientat, astfel ca
// fiecare nod u din ciclul respectiv, are un drum spre fiecare nod v din ciclul respectiv.
// Dupa ce incepem parcurgerea in adancime (DFS) pe graful respectiv, asignam fiecarui nod in parte un "index" si un "low-link" (index-ul reprezinta 
// al catelea nod il exploram, astfel ca nod-ul de inceput "0" spre exemplu, va avea index-ul 0 ; low-link-ul unui nod "u" reprezinta, nodul "v" cu cel
// mai mic "index" la care putem ajunge din nodul "u" in timpul parcurgerii in adancime (DFS), incluzand chiar si nodul "u").
// Cand parcurgem vecinii unui nod, daca am gasit un nod ALB (vecinul nu a fost parcurs), inseamna ca suntem pe cale a gasi una dintre componentele conexe
// ale grafului. Pe recursivitate, facem update la low-link-urile nodurilor (low-link-ul nodului "u", va deveni MINIMUL dintre low-link-ul nodului "u"
// si low-link-ul nodului "v", cel spre care u are muchie u->v). Daca vecinul este GRI (a fost deja vizitat), vom face update la low-link-ul nodului "u"
// ca fiind minimul intre low-link-ul nodului "u" si low-link-ul nodului "v" spre care are muchie DACA SI NUMAI DACA nodul "v" este pe stiva !!!!!!!!
// Odata ce revenim din recursivitate la nodul "u" din care am plecat in parcurgerea in adancime, daca nodul "u" are low-link-ul EGAL cu cu index-ul,
// am gasit una dintre componentele conexe ale grafului, scoatem toate nodurile de pe stiva incluzand nodul din care am plecat "u" si le marcam 
// ca facand parte din aceeasi componenta conexa.



#include<stdlib.h>
#include<stdio.h>
#include "Profiler.h"




Profiler profiler;

int pr0filer = 0;

enum { ALB, GRI, NEGRU };



// structura ce descrie vectorul de liste 
typedef struct listnodetype {
	int key;
	struct listnodetype *next;
}List;



// structura ce descrie tipul unei stive
typedef struct stacktype {
	int key;
	struct stacktype *next;
}Stack;



// structura ce descrie graful : V(nr_noduri), E(nr_muchii), pi(vector parinti), culoareNod(culoarea fiecarui nod : ALB, GRI, NEGRU), distantaNod(momentul cand a fost parcurs fiecare nod), 
// , distantaNodNegru (momentul cand a fost finalizat un nod => devine negru), vector(vectorul de liste pentru a retine nodurile adiacente cu fiecare nod in parte)
typedef struct graph {
	int V;	// nr de noduri
	int E;	// nr de muchii
	int *pi;
	int *culoareNod;
	int *distantaNod;
	int *distantaNodNegru;
	List **vector;
}GraphT;

// functie de inserare la final intr-o lista, pentru a putea afisa parcurgerea in adancime
void insertLast(List **first, List **last, int key) {
	List *aux = (List*)calloc(1, sizeof(List));
	aux->next = NULL;
	aux->key = key;

	List *iterator = *first;
	if (iterator == NULL)
		*first = *last = aux;
	else
	{
		(*last)->next = aux;
		(*last) = aux;
	}
}

// functie pentru a afisa nodurile rezultate in urma parcurgerii in adancime
void afisareDFS(List **first) {
	List *iterator = *first;

	while (iterator != NULL)
	{
		printf("%d -> ", iterator->key);
		iterator = iterator->next;
	}
}

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

// functie de adaugare a unui element in stiva, folosita atat pentru sortarea topologica cat si pentru determinarea componentelor tare conexe
void addToStack(Stack **stack, int key) {
	Stack *aux = (Stack*)calloc(1, sizeof(Stack));
	aux->key = key;
	aux->next = NULL;

	
	if (*stack == NULL)
	{
		*stack = aux;
	}
	else
	{
		aux->next = *stack;
		*stack = aux;
	}
}

// functie de stergere a unui element de pe stiva si returnarea lui, folosita pentru a scoate nodurile de pe stiva in timpul determinarii componentelor tare conexe
Stack *takeFromStack(Stack **stack) {
	Stack *aux = (Stack*)calloc(1, sizeof(Stack));
	aux = NULL;

	
	if (*stack != NULL)
	{
		if ((*stack)->next == NULL)
		{
			aux = *stack;
			*stack = NULL;
		}
		else
		{
			aux = *stack;
			*stack = aux->next;
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

// functie ce returneaza minimul a doua valori
int minim(int firstNumber, int secondNumber) {
	if (firstNumber < secondNumber)
		return firstNumber;
	return secondNumber;
}

// functie pentru a creea graful random
GraphT *createGraph(int V, int E) {
	GraphT *aux = (GraphT*)calloc(1, sizeof(GraphT));
	aux->V = V;
	aux->E = E;
	aux->pi = (int*)calloc(aux->V, sizeof(int));
	aux->culoareNod = (int*)calloc(aux->V, sizeof(int));
	aux->distantaNod = (int*)calloc(aux->V, sizeof(int));
	aux->distantaNodNegru = (int*)calloc(aux->V, sizeof(int));
	aux->vector = (List**)calloc(V, sizeof(List*));

	for (int i = 0; i < V; i++)
	{
		aux->vector[i] = NULL;
		aux->pi[i] = -1;
		aux->culoareNod[i] = ALB;
		aux->distantaNod[i] = INT_MAX;
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

				nrMuchiiAux--;
			}
		}

	}

	return aux;
}


// parcurgerea in adancime + sortarea topologica + determinarea componentelor tare conexe
void DFS_VISIT(GraphT *graph, int currentNode, int nr_elemente, int *time, Stack **topologicalSortStack, int *hasCycle, int *indexOfNodes, int *lowLinkOfNodes, int *isOnStack, Stack **SCCStack, List **first, List **last) {
	if (pr0filer == 1)
		profiler.countOperation("variazaMuchiile", nr_elemente, 3);
	if (pr0filer == 2)
		profiler.countOperation("variazaNodurile", nr_elemente, 3);
	*time += 1;
	graph->distantaNod[currentNode] = *time;
	graph->culoareNod[currentNode] = GRI;
	
	if (pr0filer == 0)
	{
		static int timeForSCC = -1;	// o variabila statica, folosita pentru a seta "index-ul" si "low-link-ul" fiecarui nod, in timpul determinarii componentelor tare conexe
		timeForSCC++;
		indexOfNodes[currentNode] = timeForSCC;
		lowLinkOfNodes[currentNode] = timeForSCC;
		addToStack(SCCStack, currentNode);
		isOnStack[currentNode] = 1;
	}
	
	List *adjiacentNodes = graph->vector[currentNode];
	if (pr0filer == 1)
		profiler.countOperation("variazaMuchiile", nr_elemente, 2);
	if (pr0filer == 2)
		profiler.countOperation("variazaNodurile", nr_elemente, 2);
	while (adjiacentNodes != NULL)
	{
		if (pr0filer == 1)
			profiler.countOperation("variazaMuchiile", nr_elemente, 2);
		if (pr0filer == 2)
			profiler.countOperation("variazaNodurile", nr_elemente, 2);
		if (graph->culoareNod[adjiacentNodes->key] == ALB)
		{
			graph->pi[adjiacentNodes->key] = currentNode;
			if (pr0filer == 1)
				profiler.countOperation("variazaMuchiile", nr_elemente, 1);
			if (pr0filer == 2)
				profiler.countOperation("variazaNodurile", nr_elemente, 1);
			DFS_VISIT(graph, adjiacentNodes->key, nr_elemente, time, topologicalSortStack, hasCycle, indexOfNodes, lowLinkOfNodes, isOnStack, SCCStack, first, last);
			if(pr0filer == 0)
				lowLinkOfNodes[currentNode] = minim(lowLinkOfNodes[currentNode], lowLinkOfNodes[adjiacentNodes->key]);
		}
		else if (graph->culoareNod[adjiacentNodes->key] == GRI)
		{
			if (pr0filer == 0)
			{
				*hasCycle = 1;
				if (isOnStack[adjiacentNodes->key] == 1)
					lowLinkOfNodes[currentNode] = minim(lowLinkOfNodes[currentNode], lowLinkOfNodes[adjiacentNodes->key]);	
			}
		}
		adjiacentNodes = adjiacentNodes->next;
	}
	graph->culoareNod[currentNode] = NEGRU;
	*time += 1;
	graph->distantaNodNegru[currentNode] = *time;
	if (pr0filer == 1)
		profiler.countOperation("variazaMuchiile", nr_elemente, 3);
	if (pr0filer == 2)
		profiler.countOperation("variazaNodurile", nr_elemente, 3);

	if ((*hasCycle) != 1 && pr0filer == 0)
		addToStack(topologicalSortStack, currentNode);


	// afisarea componentelor tare conexe
	if (pr0filer == 0) {
		Stack *lastNodeToPop = *SCCStack;
		if (lowLinkOfNodes[currentNode] == indexOfNodes[currentNode]) {
			while (lastNodeToPop->key != currentNode)
			{
				Stack *nodePopped = takeFromStack(SCCStack);
				printf("%d(SCC), ", nodePopped->key);
				isOnStack[nodePopped->key] = 0;
				lowLinkOfNodes[nodePopped->key] = indexOfNodes[currentNode];
				lastNodeToPop = *SCCStack;
			}
			Stack *nodePopped = takeFromStack(SCCStack);
			printf("%d(SCC) \n", nodePopped->key);
			isOnStack[nodePopped->key] = 0;
		}
	}

	// Inserarea elementelor rezultate in urma parcurgerii in adancime, intr-o lista simplu inlantuita, pentru a putea afisa separat nodurile astfel incat sa nu se 
	// amestece cu nodurile rezultate in urma determinarii componentelor tare conexe
	insertLast(first, last, currentNode);
}

// parcurgerea in adancime + sortarea topologica + determinarea componentelor tare conexe
int *DFS(GraphT *graph, int nr_elemente, List **first, List **last) {
	int *indexOfNodes = (int*)calloc(graph->V, sizeof(int));	// vector de indecsi, folosit pentru determinarea componentelor tare conexe
	int *lowLinkOfNodes = (int*)calloc(graph->V, sizeof(int));	// vector pentru lowLink-ul fiecarui nod, folosit pentru determinarea componentelor tare conexe
	// lowLink-ul unui nod "u" inseamna : Nodul "v", cu cel mai mic INDEX, la care putem ajunge din "u" intr-o parcurgere in adancime, incluzand insusi nodul "u"
	int *isOnStack = (int*)calloc(graph->V, sizeof(int));	// vector pentru a tine evidenta nodurilor de pe stiva, in timpul determinarii componentelor tare conexe
	// daca valoarea unui celule din vector este 1, nodul cu indicele respectiv SE AFLA PE STIVA, respectiv daca este 0, nodul cu indicele respectiv NU SE AFLA PE STIVA

	// initializarea vectorilor auxiliari folositi
	for (int i = 0; i < graph->V; i++)
	{
		graph->culoareNod[i] = ALB;
		graph->pi[i] = -1;

		indexOfNodes[i] = -1;
		lowLinkOfNodes[i] = -1;
		isOnStack[i] = 0;
	}


	int time = 0;	// time reprezinta o variabila, ce o trimit prin referinta ca sa se modifice constant, o folosesc pentru a putea seta timpii de parcurgere / finalizare a nodurilor de-a lungul parcurgerii in adancime
	Stack *topologicalSortStack = NULL;	// stiva pentru a adauga nodurile rezultate in urma sortarii topologice
	Stack *SCCStack = NULL;	// stiva pentru a adauga nodurile in timpul determinarii componentelor tare conexe
	int hasCycle = 0; // variabila care, dupa parcurgerea in adancime, are valoarea 0 daca graful NU CONTINE CICLURI, respectiv valoarea 1 daca graful CONTINE CICLURI
	


	for (int i = 0; i < graph->V; i++)
	{
		if (graph->culoareNod[i] == ALB)
			DFS_VISIT(graph, i, nr_elemente, &time, &topologicalSortStack, &hasCycle, indexOfNodes, lowLinkOfNodes, isOnStack, &SCCStack, first, last);
	}



	// afisare sortare topologica : daca graful NU ARE CICLURI, sortarea topologica este posibila, iar daca ARE CICLURI, nu putem sorta topologic nodurile grafului
	if (hasCycle == 0 && pr0filer == 0) {
		printf("\nSortarea topologica a grafului este: \n");
		Stack *printTopologicalSort = topologicalSortStack;
		while (printTopologicalSort != NULL)
		{
			printf("%d -> ", printTopologicalSort->key);
			printTopologicalSort = printTopologicalSort->next;
		}
	}
	else if(pr0filer == 0)
			printf("\nGraful nu poate fi sortat topologic intrucat contine cel putin un ciclu !");
	return lowLinkOfNodes;
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


	////////// START DEMO DFS
	// Ca sa demonstrez DFS si algoritmul lui Tarjan pentru a determina componentele tare conexe, am folosit acelasi graf ca exemplu, si am inserat muchiile pe rand
	int V = 8;
	int E = 0;

	GraphT *graph = NULL;
	graph = createGraph(V, E);
	graph->E = 13;
	insertFirst(graph->vector, 3, 7);
	insertFirst(graph->vector, 7, 3);
	insertFirst(graph->vector, 3, 4);
	insertFirst(graph->vector, 7, 5);
	insertFirst(graph->vector, 4, 5);
	insertFirst(graph->vector, 5, 6);
	insertFirst(graph->vector, 6, 4);
	insertFirst(graph->vector, 5, 0);
	insertFirst(graph->vector, 6, 0);
	insertFirst(graph->vector, 2, 0);
	insertFirst(graph->vector, 6, 2);
	insertFirst(graph->vector, 0, 1);
	insertFirst(graph->vector, 1, 2);



	// Am creeat inca o lista simplu inlantuita pentru a putea insera pe rand elementele din parcurgerea DFS aici, deoarece, 
	// cum afisez componentele tare conexe direct din DFS, daca lasam afisarea parcurgerii DFS tot asa, direct din interiorul functiei, se amestecau valorile
	// si nu puteam deosebi care la care face parte .. am facut asa sa se vada frumos care cui apartine
	List *first = NULL;
	List *last = NULL;




	// afisarea nodurilor si nodurile adiacente lor
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


	// afisarea componentelor tare conexe + parcurgerea in adancime a grafului
	printf("\nComponentele tare conexe ale grafului sunt: \n");
	printf("\n\n");
	int *lowLinkOfNodes = DFS(graph, 0, &first, &last);
	printf("\n");
	for (int i = 0; i < graph->V; i++)
		printf("Nodul %d : %d low-link\n", i, lowLinkOfNodes[i]);

	printf("Parcurgerea in adancime a grafului este: \n");
	afisareDFS(&first);
	
	

	// afisarea distantelor (de parcurgere cat si de finalizare a fiecarui nod), cat si vectorul de parinti rezultat in urma parcurgerii 
	printf("\nDistanta fiecarui nod in timpul parcurgerii: ");
	for (int i = 0; i < graph->V; i++)
	{
		printf("%d, ", graph->distantaNod[i]);
	}

	printf("\nDistanta de finalizare a fiecarui nod: ");
	for (int i = 0; i < graph->V; i++)
	{
		printf("%d ", graph->distantaNodNegru[i]);
	}

	printf("\nVectorul de parinti al nodurilor este: \n");
	for (int i = 0; i < graph->V; i++)
	{
		printf("%d - %d", i, graph->pi[i]);
		printf("\n");
	}
	

	// pretty print pentru parcurgerea in adancime
	printf("\n\n\n");
	printf("Pretty print: \n\n");
	prettyPrintParentTree(graph->pi, graph->V, -1, 0);


	// dezalocarea memoriei 
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
	free(graph->distantaNodNegru);
	free(graph->vector);
	free(graph);
	List *iteratorPentruDezalocare = first;
	while (iteratorPentruDezalocare != NULL)
	{
		List *freeNode = iteratorPentruDezalocare;
		iteratorPentruDezalocare = iteratorPentruDezalocare->next;
		free(freeNode);
	}
	first = NULL;
	last = NULL;
	///////// stop demo pentru DFS

	printf("\n\n\n\n\n\n");

	//////// start demo pentru demonstratia sortarii topologice
	graph = NULL;
	V = 6;
	E = 0;
	graph = createGraph(V, E);
	graph->E = 13;
	insertFirst(graph->vector, 0, 1);
	insertFirst(graph->vector, 2, 3);
	insertFirst(graph->vector, 4, 5);
	insertFirst(graph->vector, 0, 2);
	insertFirst(graph->vector, 1, 3);
	insertFirst(graph->vector, 2, 4);
	insertFirst(graph->vector, 3, 5);
	insertFirst(graph->vector, 2, 1);
	insertFirst(graph->vector, 3, 4);

	// afisarea nodurilor si nodurile adiacente lor
	printf("Nodurile impreuna cu vecinii lor: \n");
	
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


	// afisarea componentelor tare conexe + parcurgerea in adancime a grafului
	printf("\nComponentele tare conexe ale grafului sunt: \n");
	DFS(graph, 0, &first, &last);
	printf("\n\n");
	printf("Parcurgerea in adancime a grafului este: \n");
	afisareDFS(&first);


	// afisarea distantelor (de parcurgere cat si de finalizare a fiecarui nod), cat si vectorul de parinti rezultat in urma parcurgerii 
	printf("\nDistanta fiecarui nod in timpul parcurgerii: ");
	for (int i = 0; i < graph->V; i++)
	{
		printf("%d, ", graph->distantaNod[i]);
	}

	printf("\nDistanta de finalizare a fiecarui nod: ");
	for (int i = 0; i < graph->V; i++)
	{
		printf("%d ", graph->distantaNodNegru[i]);
	}

	printf("\nVectorul de parinti al nodurilor este: \n");
	for (int i = 0; i < graph->V; i++)
	{
		printf("%d - %d", i, graph->pi[i]);
		printf("\n");
	}


	// pretty print pentru parcurgerea in adancime
	printf("\n\n\n");
	printf("Pretty print: \n\n");
	prettyPrintParentTree(graph->pi, graph->V, -1, 0);


	// dezalocarea memoriei 
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
	free(graph->distantaNodNegru);
	free(graph->vector);
	free(graph);
	iteratorPentruDezalocare = first;
	while (iteratorPentruDezalocare != NULL)
	{
		List *freeNode = iteratorPentruDezalocare;
		iteratorPentruDezalocare = iteratorPentruDezalocare->next;
		free(freeNode);
	}
	first = NULL;
	last = NULL;

	///////// stop demo pentru sortarea topologica







	
	pr0filer++;
	// generarea primul grafic, variaza numarul de muchii (E)
	V = 100;
	for (int E = 1000; E <= 4500; E += 100)
	{
		graph = createGraph(V, E);
		DFS(graph, E, &first, &last);

		// dezalocarea memoriei 
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
		free(graph->distantaNodNegru);
		free(graph->vector);
		free(graph);
		iteratorPentruDezalocare = first;
		while (iteratorPentruDezalocare != NULL)
		{
			List *freeNode = iteratorPentruDezalocare;
			iteratorPentruDezalocare = iteratorPentruDezalocare->next;
			free(freeNode);
		}
		first = NULL;
		last = NULL;
	}



	// generarea celui de-al doilea grafic, variaza numarul de noduri
	pr0filer++;
	E = 4500;
	for (V = 100; V <= 200; V += 10) {
		graph = createGraph(V, E);
		DFS(graph, V, &first, &last);

		// dezalocarea memoriei 
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
		free(graph->distantaNodNegru);
		free(graph->vector);
		free(graph);
		iteratorPentruDezalocare = first;
		while (iteratorPentruDezalocare != NULL)
		{
			List *freeNode = iteratorPentruDezalocare;
			iteratorPentruDezalocare = iteratorPentruDezalocare->next;
			free(freeNode);
		}
		first = NULL;
		last = NULL;
	}
	
	

	profiler.showReport();
	
	getchar();
}