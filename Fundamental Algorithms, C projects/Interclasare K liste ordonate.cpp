#define _CRT_SECURE_NO_WARNINGS


/**
 * @author Marchis Raul
 * @group 30229
 *
	Se cere implementarea corecta si eficienta a unei metode de complexitate O(n * log k) pentru interclasarea a k siruri sortate,
	unde n este numarul total de elemente (Sugestie : folositi un heap, vezi notitele de la seminarul al 2lea).

	Explicarea in limbaj natural a algoritmului : folosim un vector de liste pentru a retine un anumit numar de liste ordonate. 
	Pentru inceput, luam fiecare element din lista (first-ul fiecarei liste) si ii dam push intr-un heap pe minim, ceea ce inseamna ca, dupa ce introducem toate elementele,
	heap-ul va fi gata aranjat intr-un HEAP PE MINIM. Astfel, primul element din heap(cel cu indice 0) va fi si cel mai mic element. 
	Extragem acel element(extractMin), heap-ul se reface cu elementele ramase in el, iar noi putem introduce elementul extras deja in lista noastra finala
	(insertLast, intrucat vrem ca in lista noastra finala elementele sa fie in ordine crescatoare, iar daca noi extragem elemente "din ce in ce mai mari",
	acestea trebuie clar introduse tot la finalul listei) ca fiind primul element de-acolo. 
	Pe urma, stiind din care lista a fost extras acel element, urmeaza sa reintroducem un element, adica ELEMENTUL CARE URMEAZA IN LISTA DIN CARE FACE PARTE ELEMENTUL EXTRAS ADINEAORI. 
	Repetam astfel acest lucru : extragem un element din heap, urmand ca apoi sa introducem elementul ce urmeaza dupa acel element extras in lista din care face parte.
									
 */



#include<stdlib.h>
#include<stdio.h>
#include "Profiler.h"

Profiler profiler;

int nConstant = 0;

// start structura pentru elementele unei liste 
typedef struct nodeType {
	int key;
	struct nodeType *next;
}NodeT;
// stop structura pentru elementele unei liste



// start structura pentru heap (intrucat dupa ce facem heap-ul pe minim si returnam elementul de pe prima pozitie (cel mai mic), trebuie sa returnam cumva si din ce lista a fost luat, ca sa introducem de-acolo urmatorul element, deci trebuie sa retinem indicele
typedef struct heapType {
	int key;
	int indiceLista;
}HeapT;
// stop structura pentru heap



// start implementarea top-down de construire a heap-ului
int Parent_TD(int i)
{
	return (i - 1) / 2;
}

int Left(int i)
{
	return 2 * i + 1;
}

int Right(int i)
{
	return (2 * i) + 2;
}

void MinHeapify(HeapT *heap, int i, int nr_elemente, int nrTotalElem, int kListe)
{
	int lowest = 0;
	int l = Left(i);
	int r = Right(i);

	int ok = 0;
	if (l < nr_elemente)
		ok = 1;
	if (ok == 1)
	{
		if (nConstant == 1)
			profiler.countOperation("nConstant", kListe, 1);
		if (kListe == 5 && nConstant == 0)
			profiler.countOperation("operatii_5Liste", nrTotalElem, 1);
		if (kListe == 10 && nConstant == 0)
			profiler.countOperation("operatii_10Liste", nrTotalElem, 1);
		if (kListe == 100 && nConstant == 0)
			profiler.countOperation("operatii_100Liste", nrTotalElem, 1);
	}
	if (l < nr_elemente && heap[l].key < heap[i].key)
	{
		lowest = l;
	}
	else
	{
		lowest = i;
	}

	ok = 0;
	if (r < nr_elemente)
		ok = 1;
	if (ok == 1)
	{
		if (nConstant == 1)
			profiler.countOperation("nConstant", kListe, 1);
		if (kListe == 5 && nConstant == 0)
			profiler.countOperation("operatii_5Liste", nrTotalElem, 1);
		if (kListe == 10 && nConstant == 0)
			profiler.countOperation("operatii_10Liste", nrTotalElem, 1);
		if (kListe == 100 && nConstant == 0)
			profiler.countOperation("operatii_100Liste", nrTotalElem, 1);
	}
	if (r < nr_elemente && heap[r].key < heap[lowest].key)
	{
		lowest = r;
	}


	if (lowest != i)
	{
		HeapT aux = heap[i];
		heap[i] = heap[lowest];
		heap[lowest] = aux;
		if (nConstant == 1)
			profiler.countOperation("nConstant", kListe, 3);
		if (kListe == 5 && nConstant == 0)
			profiler.countOperation("operatii_5Liste", nrTotalElem, 3);
		if (kListe == 10 && nConstant == 0)
			profiler.countOperation("operatii_10Liste", nrTotalElem, 3);
		if (kListe == 100 && nConstant == 0)
			profiler.countOperation("operatii_100Liste", nrTotalElem, 3);
		MinHeapify(heap, lowest, nr_elemente, nrTotalElem, kListe);
	}
}

void push(HeapT *heap, HeapT elementLista, int *heap_size, int nrTotalElem, int kListe)
{
	heap[++(*heap_size)] = elementLista;
	int i = (*heap_size);
	if (nConstant == 1)
		profiler.countOperation("nConstant", kListe, 1);
	if (kListe == 5 && nConstant == 0)
		profiler.countOperation("operatii_5Liste", nrTotalElem, 1);
	if (kListe == 10 && nConstant == 0)
		profiler.countOperation("operatii_10Liste", nrTotalElem, 1);
	if (kListe == 100 && nConstant == 0)
		profiler.countOperation("operatii_100Liste", nrTotalElem, 1);

	while (i > 0 && heap[i].key < heap[Parent_TD(i)].key)	// inainte aveam >
	{
		if (nConstant == 1)
			profiler.countOperation("nConstant", kListe, 1);
		if (kListe == 5 && nConstant == 0)
			profiler.countOperation("operatii_5Liste", nrTotalElem, 1);
		if (kListe == 10 && nConstant == 0)
			profiler.countOperation("operatii_10Liste", nrTotalElem, 1);
		if (kListe == 100 && nConstant == 0)
			profiler.countOperation("operatii_100Liste", nrTotalElem, 1);

		HeapT aux = heap[i];
		heap[i] = heap[Parent_TD(i)];
		heap[Parent_TD(i)] = aux;

		if (nConstant == 1)
			profiler.countOperation("nConstant", kListe, 3);
		if (kListe == 5 && nConstant == 0)
			profiler.countOperation("operatii_5Liste", nrTotalElem, 3);
		if (kListe == 10 && nConstant == 0)
			profiler.countOperation("operatii_10Liste", nrTotalElem, 3);
		if (kListe == 100 && nConstant == 0)
			profiler.countOperation("operatii_100Liste", nrTotalElem, 3);

		i = Parent_TD(i);
	}
}


HeapT extractMin(HeapT *heap, int *heap_size, int nr_elemente, int nrTotalElem, int kListe)
{
	HeapT elementMinim = heap[0];
	heap[0] = heap[(*heap_size)];
	if (nConstant == 1)
		profiler.countOperation("nConstant", kListe, 1);
	if (kListe == 5 && nConstant == 0)
		profiler.countOperation("operatii_5Liste", nrTotalElem, 1);
	if (kListe == 10 && nConstant == 0)
		profiler.countOperation("operatii_10Liste", nrTotalElem, 1);
	if (kListe == 100 && nConstant == 0)
		profiler.countOperation("operatii_100Liste", nrTotalElem, 1);
	(*heap_size) = (*heap_size) - 1;
	MinHeapify(heap, 0, (*heap_size) + 1, nrTotalElem, kListe);

	return elementMinim;

}
// stop implementare top-down de construire a heap-ului



// start operatii pentru inserarea si afisarea elementelor unei liste
void insert_first(NodeT **first, int val)
{
	NodeT *aux = (NodeT*)calloc(1, sizeof(NodeT));
	aux->next = NULL;
	aux->key = val;

	if (*first == NULL)
		*first = aux;
	else
	{
		aux->next = *first;
		*first = aux;
	}
	
}

void printList(NodeT **first)
{
	NodeT *parcurgere = *first;
	while (parcurgere != NULL)
	{
		printf("%d -> ", parcurgere->key);
		parcurgere = parcurgere->next;
	}
	
}
// stop operatii pentru inserarea si afisarea elementelor unei liste





// start implementarea operatiilor pe coada, adica pe "lista" noastra finala de elemente

void enqueue(NodeT **head, NodeT **tail, int key)
{
	NodeT *aux = (NodeT*)calloc(1, sizeof(NodeT));
	aux->key = key;
	aux->next = NULL;

	if ((*tail) == NULL)
		(*head) = (*tail) = aux;
	else
	{
		(*tail)->next = aux;
		(*tail) = aux;
	}

	//free(aux);
}

// stop implementarea operatiilor pe coada, adica pe "lista" noastra finala de elemente





// start functia mare de interclasare a celor K liste

void INTERCLASARE(NodeT **vectorListe, HeapT* heap, NodeT** listaFinalaHead, NodeT** listaFinalaTail, int *heap_size, int nrElementeMaxHeap, int kListe, int nrTotalElem)	// kListe reprezinta numarul de liste, iar nrTotalElem suma numarului de elemente din fiecare lista, gen totalul de elemente din lista mea finala
{
	HeapT elementLista;
	NodeT* elementListaAux;

	for (int i = 0; i < kListe; i++)
	{
		
		elementListaAux = vectorListe[i];	// elementListaAux ii efectiv un auxliar ca sa pot transfera data de la NodeT, tipul vectorului lista, la HeapT, tipul elementului din lista

		if (elementListaAux != NULL)
		{
			elementLista.key = elementListaAux->key;
			elementLista.indiceLista = i;

			push(heap, elementLista, heap_size, nrTotalElem, kListe);

		}
	}



	while ((*heap_size) >= 0)
	{

		HeapT nodExtras = extractMin(heap, heap_size, nrElementeMaxHeap, nrTotalElem, kListe);
		enqueue(listaFinalaHead, listaFinalaTail, nodExtras.key);
		vectorListe[nodExtras.indiceLista] = vectorListe[nodExtras.indiceLista]->next;	// minimul a fost gasit in lista nodExtras.indiceLista, deci mutam "first-ul" sa pointeze la urmatorul element

		elementListaAux = vectorListe[nodExtras.indiceLista];	// asignam urmatorul element din lista 
		if (elementListaAux != NULL)
		{
			elementLista.key = elementListaAux->key;
			elementLista.indiceLista = nodExtras.indiceLista;	

			push(heap, elementLista, heap_size, nrTotalElem, kListe);
			//vectorListe[elementLista.indiceLista] = vectorListe[elementLista.indiceLista]->next;	
		}
	}


}

// stop functia mare de interclasare a celor K liste




// start functia de afisare finala a listei

void afisareListaFinala(NodeT **head, NodeT **tail)
{
	NodeT* parcurgere = *head;
	while (parcurgere != NULL)
	{
		printf("%d -> ", parcurgere->key);
		parcurgere = parcurgere->next;
	}
}

// stop functia de afisare finala a listei


// start functie pentru dezalocarea memoriei, pentru ca dupa vreo doua ore pierdute cu profiler-ul, mi-am dat seama ca imi crapa la fiecare rulare din cauza ca se depaseste memoria :(

void curatareListe(NodeT **vectorListe, int k)
{
	for (int i = 0; i < k; i++)
	{
		NodeT *aux = vectorListe[i];

		while (aux != NULL)
		{
			NodeT *q = aux;
			aux = aux->next;
			free(q);
		}
		vectorListe[i] = NULL;
	}
}

// stop functie pentru dezalocarea memoriei



int main()
{
	// START EX 1 PENTRU PROFILER : numarul de liste este 5, 10 sau 100, iar pentru fiecare dintre aceste valori, numarul de elemente total variaza intre 100-10000

	int const kListe_5 = 5;
	int const kListe_10 = 10;
	int const kListe_100 = 100;
	int const kListe_500 = 500;
	int kListe = 0;

	NodeT *vectorListe[kListe_500];
	int nrElementeHeap = 10000;
	HeapT* heap = (HeapT*)calloc(10000, sizeof(HeapT));
	int nr_elemente = 10000;
	NodeT* listaFinalaHead = NULL;
	NodeT* listaFinalaTail = NULL;
	int B[10000];

	for (int i = 0; i < 3; i++) {

	
		if (i == 0)
		{
			kListe = kListe_5;

			for (int i = 0; i < kListe; i++)
				vectorListe[i] = NULL;
		}

		if (i == 1)
		{
			kListe = kListe_10;

			for (int i = 0; i < kListe; i++)
				vectorListe[i] = NULL;
		}

		if (i == 2)
		{
			kListe = kListe_100;

			for (int i = 0; i < kListe; i++)
				vectorListe[i] = NULL;
		}


		int k = 5;

	

	
	for (int m = 100; m < 10000; m += 400) {
		
		nr_elemente = m;
		for (int i = 0; i < kListe; i++)
		{
			FillRandomArray(B, nr_elemente / kListe, 1, 10000, 1, 2);
			for (int k = 0; k < nr_elemente / kListe; k++)
				insert_first(&vectorListe[i], B[k]);
		}

		/*/
		printf("Vectorul de liste generate aleator: \n");
		for (int i = 0; i < kListe; i++)
		{
			printf("Lista numarul %d : ", i + 1);
			printList(&vectorListe[i]);
			printf("\n\n\n\n");
		}
		*/

		
		for (int i = 0; i < nrElementeHeap; i++)
		{
			heap[i].key = 0;
			heap[i].indiceLista = 0;
		}
		int heap_size = -1;


		listaFinalaHead = NULL;	// aceasta "lista finala", am implementat-o ca si o coada defapt, ca pe masura ce scoatem cate un element din heap, sa le putem pune pe rand in ordine crescatoare => operatia enqueue intr-o coada defapt
		listaFinalaTail = NULL;	// pe lsitaFinalTail, vom insera mereu elementele extrase din HEAP, intrucat operatia se efectueaza in O(1) si este cel mai eficient mod posibil

		INTERCLASARE(vectorListe, heap, &listaFinalaHead, &listaFinalaTail, &heap_size, nrElementeHeap, kListe, m);
		/*/
		printf("Interclasarea celor k liste este: \n\n");
		afisareListaFinala(&listaFinalaHead, &listaFinalaTail);
		printf("\n\n\n\n\n\n\n");*/
		curatareListe(vectorListe, kListe);
		
	}
}
	// STOP EX 1 PENTRU PROFILER : numarul de liste este 5, 10 sau 100, iar pentru fiecare dintre aceste valori, numarul de elemente total variaza intre 100-10000

	


	// START EX 2 : numarul de elemente din liste este constant tinut la 10000, iar numarul de liste variaza intre 10-500
	nConstant++;	// variabila globala ca sa stiu in functii cand incrementez cu profiler.countOperation numarul de operatii si sa nu se amestece cu operatiile de la ex 1

	kListe = kListe_500;
	curatareListe(vectorListe, kListe);
	nr_elemente = 10000;

	for (int m = 10; m <= 500; m += 10) {
		
		kListe = m;
		nr_elemente = 10000;
		for (int i = 0; i < kListe; i++)
		{
			FillRandomArray(B, nr_elemente / kListe, 1, 10000, 1, 2);
			for (int k = 0; k < nr_elemente / kListe; k++)
				insert_first(&vectorListe[i], B[k]);
		}

		/*/
		printf("Vectorul de liste generate aleator: \n");
		for (int i = 0; i < kListe; i++)
		{
			printf("Lista numarul %d : ", i + 1);
			printList(&vectorListe[i]);
			printf("\n\n\n\n");
		}
		*/

	
		for (int i = 0; i < nrElementeHeap; i++)
		{
			heap[i].key = 0;
			heap[i].indiceLista = 0;
		}
		int heap_size = -1;


		listaFinalaHead = NULL;	// aceasta "lista finala", am implementat-o ca si o coada defapt, ca pe masura ce scoatem cate un element din heap, sa le putem pune pe rand in ordine crescatoare => operatia enqueue intr-o coada defapt
		listaFinalaTail = NULL;	// pe lsitaFinalTail, vom insera mereu elementele extrase din HEAP, intrucat operatia se efectueaza in O(1) si este cel mai eficient mod posibil

		INTERCLASARE(vectorListe, heap, &listaFinalaHead, &listaFinalaTail, &heap_size, nrElementeHeap, kListe, nr_elemente);
		/*/
		printf("Interclasarea celor k liste este: \n\n");
		afisareListaFinala(&listaFinalaHead, &listaFinalaTail);
		printf("\n\n\n\n\n\n\n");*/
		curatareListe(vectorListe, kListe);
	
	}
	
	// STOP EX 2 : numarul de elemente din liste este constant tinut la 10000, iar numarul de liste variaza intre 10-500







	///// START AFISAREA DEMO A ALGORITMULUI /////
	nConstant++;

	nr_elemente = 200;
	kListe = 10;

	for (int i = 0; i < kListe; i++)
	{
		FillRandomArray(B, nr_elemente / kListe, 1, 1000, 1, 2);
		for (int k = 0; k < nr_elemente / kListe; k++)
			insert_first(&vectorListe[i], B[k]);
	}

	printf("Vectorul de liste generate aleator: \n");
	for (int i = 0; i < kListe; i++)
	{
		printf("Lista numarul %d : ", i + 1);
		printList(&vectorListe[i]);
		printf("\n\n\n\n");
	}


	nrElementeHeap = 10;
	for (int i = 0; i < nrElementeHeap; i++)
	{
		heap[i].key = 0;
		heap[i].indiceLista = 0;
	}
	int heap_size = -1;


	listaFinalaHead = NULL;	// aceasta "lista finala", am implementat-o ca si o coada defapt, ca pe masura ce scoatem cate un element din heap, sa le putem pune pe rand in ordine crescatoare => operatia enqueue intr-o coada defapt
	listaFinalaTail = NULL;	// pe lsitaFinalTail, vom insera mereu elementele extrase din HEAP, intrucat operatia se efectueaza in O(1) si este cel mai eficient mod posibil

	INTERCLASARE(vectorListe, heap, &listaFinalaHead, &listaFinalaTail, &heap_size, nrElementeHeap, kListe, nr_elemente);

	printf("Interclasarea celor k liste este: \n\n");
	afisareListaFinala(&listaFinalaHead, &listaFinalaTail);



	//////// STOP AFISAREA DEMO A ALGORITMULUI ///////






	profiler.createGroup("OperatiiEfectuate", "operatii_5Liste", "operatii_10Liste", "operatii_100Liste");
	profiler.showReport();

	getchar();
}