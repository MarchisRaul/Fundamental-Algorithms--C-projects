#define _CRT_SECURE_NO_WARNINGS

#include<stdlib.h>
#include<stdio.h>
#include "Profiler.h"

/**
 * @author Marchis Raul
 * @group 30229
 *
 * Se cere implementarea corecta ​si eficienta ​a doua metode de construire a structurii de date
	Heap i.e. “de jos in sus” (bottom­up) si “de sus in jos” (top­down). De asemenea, se cere
	implementarea algoritmului heapsort

	Algoritmul de construire a structurii de date Heap bottom-up se desfasoara in felul urmator : pe vectorul cu elemente aranjate aleator,
	se incepe algoritmul. Incepand de la jumatatea vectorului, adica de la primul nod care nu este frunza (indicele i) si se verifica relatia cu copiii, care in vector
	se afla la indicii 2*i si 2*i + 1. In cazul de fata, eu am facut implementarea algoritmului pe maxim (radacina este cel mai mare nod din arbore), urmand ca
	ambii copii sa fie mai mici(fiecare parinte este mai mare ca si copiii sai). Se cauta cea mai mare valoare dintre nodul parinte si cei doi copii, si in 
	cazul in care unul dintre copii este mai mare, se interschimba nodul parinte cu nodul copil. Efectuandu-se aceasta schimbare, este posibil ca 
	pe mai departe, noul nod "copil" sa fie mai mare ca si copiii sai si astfel se continua algoritmul in continuare(recrusviv) pana cand relatia
	parinte - copii este inregula. Pe urma, se trece la urmatorul nod care nu este frunza(i-1) si se repeta aceleasi operatii pana cand ajungem la
	radacina (primul nod din arbore). In cazul in care relatia sa cu copiii este inregula, algoritmul se termina si heap-ul este gata construit.

	Algoritmul de construire a structurii de date Heap top-down se desfasoara in felul urmator : spre deosebire de bottom-up, in functie de cum ii spune
	si numele, heap-ul se construieste de jos in sus in arbore. La top-down, efectuam "operatia inversa". Avand vectorul aleatoriu, luam o varibila separata
	pentru a retine incet noua dimensiune a viitorului vector intrucat putem lucra direct pe el. Primul element il lasam la locul lui, si incepem algoritmul
	de la al doilea element, avand indicele 1. Astfel, luam elementul cu indicele 1, il asezam dupa elementul cu indicele 0 pe care l-am lasat la locului lui
	si incepem sa verificam relatia sa cu parintele, care se afla la indicele (i-1)/2. Daca noul nod adaugat in vector este mai mare decat parintele,
	le interschimbam si continuam algoritmul de la nodul copil care acum a ajuns "parinte", intrucat poate acesta este mai mare decat parintele sau.
	Algoritmul continua pentru fiecare nod din vector incepand de la cel cu indicele 1, pana la ultimul (cel cu indicele nr_elemente-1).
	Spre exemplu : daca adaugam nodul 9 in arbore, si pana acolo relatia de heap este ok, incepem sa verificam nodul cu indicele 9 cu parintele sau care 
	se afla la indicele 4. Daca copilul (indicele 9) este mai mare decat parintele (indicele 4), le interschimbam si suntem nevoiti sa continuam
	algoritmul de la indicele 4, intrucat poate este mai mare decat noul lui parinte (indicele 1). Aceasta verificare se incheie in momentul cand relatia
	cu noul parinte este ok, sau s-a ajuns chiar la primul element din vector(radacina) si nu mai avem cu ce verifica intrucat acesta nu are un parinte.

	In ceea ce priveste compararea celor doua metode, se observa clar din grafice ca algoritmul de implementare a heap-ului bottom-up este mult
	mai eficient decat al celui top-down, atat in ceea ce priveste numarul de noduri accesate in arbore, cat si numarul total de operatii efectuate
	(adica comparatii si atribuiri). Lucru foarte asteptat, intrucat complexitatea de construire a heap-ului bottop-up este O(n), in vreme ce
	complexitatea de construire a heap-ului top-down este O(nlogn).
	Ideea de baza : algoritmul bottom-up foloseste majoritatea timpului de executie lucrand pe heap-uri micute si construindu-le intai pe ele, rezultand 
	astfel in heap-ul mare, spre deosebire de top-down care, desi fiecare inserare de element necesita un timp logaritmic (O(logn)), suntem nevoiti sa facem
	aceasta operatie pe toate elementele(de unde rezulta complexitatea O(nlogn), rezultand astfel un timp de executie mai mare.
	Ca fapt divers, sa ne imaginam cazul cel mai defavorabil al implementarii top-down : cand toate elementele sunt in ordine descrescatoare in vector.
	Elementul nou inserat va fi cel mai mare din vector, deci va trebui sa ajunga mereu in radacina, astfel, la un numar mai mare de elemente, algoritmul va fi
	mult mai ineficient decat o simpla implementare bottom-up.

	Legat de heap-sort, algoritmul este usor de inteles dupa ce pricepem logica implementarii bottom-up.
	Dupa apelul functiei, se  construieste heap-ul. Ceea ce inseamna, ca in radacina noi avem acum cel mai mare element din vector(tinand cont ca implementarea a fost
	facuta pe maxim. Astfel, interschimbam nodul radacina(cel mai mare) cu ultima frunza, pentru ca pune cel mai mare element din vector pe ultima pozitie din vectorul
	ce urmeaza a fi sortat crescator. Apelam din nou algoritmul, dar fara a lua in considerare acel ultim element care acum este cel mai mare si este la locul lui. Repetand algoritmul
	a doua oara, al doilea cel mai mare element din vector va fi pus pe penultima pozitie din vectorul sortat, adica la locul lui. Repetam algoritmul pana cand vectorul este sortat crescator,
	rezultand o complexitate O(nlogn).

									 

 *
 */

Profiler profiler;

// inceput implementare bottom-up

int Parent(int i)
{
	return i / 2;
}

int Left(int i)
{
	return 2 * i + 1;
}

int Right(int i)
{
	return (2 * i) + 2;
}

void MaxHeapify(int *A, int i, int nr_elemente)
{
	int largest = 0;
	int l = Left(i);
	int r = Right(i);

	
	if (l < nr_elemente && A[l] > A[i])	// pseudocodul are l <= nr_elemente, dar pt ca vectorul incepe de la 0, mergem pana <nr_elemente
	{
		largest = l;
		profiler.countOperation("comparatii_BottomUp", nr_elemente, 1);
	}
	else
	{
		int ok = 0;
		if (l < nr_elemente)
			ok = 1;
		if(ok == 1)
			profiler.countOperation("comparatii_BottomUp", nr_elemente, 1);
		largest = i;
	}

	
	if (r < nr_elemente && A[r] > A[largest])	// pseudocodul are l <= nr_elemente, dar pt ca vectorul incepe de la 0, mergem pana <nr_elemente
	{
		largest = r;
		profiler.countOperation("comparatii_BottomUp", nr_elemente, 1);
	}


	//profiler.countOperation("comparatii_BottomUp", nr_elemente, 1);	/// idkkkkkkkkk
	if (largest != i)
	{
		int aux = A[i];
		A[i] = A[largest];
		A[largest] = aux;
		profiler.countOperation("atribuiri_BottomUp", nr_elemente, 3);
		profiler.countOperation("nr_noduri_accesate_BottomUp", nr_elemente, 1);
		MaxHeapify(A, largest, nr_elemente);
	}
}

void BuildMaxHeap(int *A, int nr_elemente)
{
	int i = 0;
	int heap_size = nr_elemente;
	for (i = heap_size / 2; i >= 0; i--)
		MaxHeapify(A, i, nr_elemente);
}

// sfarsit implementare bottom-up





// inceput implementare top-down

int Parent_TD(int i)
{
	return (i - 1) / 2;
}

void push(int *A, int key, int *heap_size, int nr_elemente)
{
	profiler.countOperation("atribuiri_TopDown", nr_elemente, 1);
	A[++(*heap_size)] = key;
	int localSize = (*heap_size);
	int i = (*heap_size);
	
	int ok = 0;
	if (i > 0)
		ok = 1;
	if(ok == 1)
		profiler.countOperation("comparatii_TopDown", nr_elemente, 1);
	while (i > 0 && A[i] > A[Parent_TD(i)])
	{
		profiler.countOperation("comparatii_TopDown", nr_elemente, 1);
		int aux = A[i];
		A[i] = A[Parent_TD(i)];
		A[Parent_TD(i)] = aux;
		i = Parent_TD(i);
		profiler.countOperation("atribuiri_TopDown", nr_elemente, 3);
		profiler.countOperation("nr_noduri_accesate_TopDown", nr_elemente, 1);
	}
}

void top_down(int *A, int *heap_size, int nr_elemente)
{
	(*heap_size) = 0;
	for (int i = 1; i < nr_elemente; i++)
		push(A, A[i], heap_size, nr_elemente);
}

// sfarsit implementare TOP-DOWN



// inceput implementare heap-sort
void HeapSort(int *A, int nr_elemente)
{
	int heap_size = nr_elemente;
	BuildMaxHeap(A, nr_elemente);
	for (int i = nr_elemente - 1; i >= 1; i--)
	{
		int aux = A[0];
		A[0] = A[i];
		A[i] = aux;
		heap_size--;
		BuildMaxHeap(A, heap_size);
	}

}

int main()
{
	
	int nr_elemente = 0;
		
	// BOTTOM_UP
	
	int A[11000];

	for (nr_elemente = 100; nr_elemente <= 10000; nr_elemente += 100)
	{
		for (int i = 0; i < 5; i++)
		{
			FillRandomArray(A, nr_elemente, 1, 10000, 1, 0);
			BuildMaxHeap(A, nr_elemente);
		}
	}

	profiler.divideValues("comparatii_BottomUp", 5);
	profiler.divideValues("atribuiri_BottomUp", 5);
	profiler.divideValues("nr_noduri_accesate_BottomUp", 5);
	profiler.addSeries("atrib_plus_comp_BU", "comparatii_BottomUp", "atribuiri_BottomUp");
	
	// BOTTOM UP
	



	// TOP-DOWN
	int B[11000];
	int heap_size = 0;

	for (nr_elemente = 100; nr_elemente <= 10000; nr_elemente += 100)
	{
		for (int i = 0; i < 5; i++)
		{
			FillRandomArray(B, nr_elemente, 1, 10000, 1, 0);
			top_down(B, &heap_size, nr_elemente);
		}
	}

	profiler.divideValues("comparatii_TopDown", 5);
	profiler.divideValues("atribuiri_TopDown", 5);
	profiler.divideValues("nr_noduri_accesate_TopDown", 5);
	profiler.addSeries("atrib_plus_comp_TD", "comparatii_TopDown", "atribuiri_TopDown");
	// TOP-DOWN

	profiler.createGroup("atrib_plus_comp", "atrib_plus_comp_BU", "atrib_plus_comp_TD");
	profiler.createGroup("nr_noduri_accesate", "nr_noduri_accesate_BottomUp", "nr_noduri_accesate_TopDown");

	profiler.showReport();



	// heap-sort
	int C[101];
	FillRandomArray(C, 100, 1, 100, 1, 0);

	printf("Elementele vectorului random: ");
	printf("\n");
	for (int i = 0; i < 100; i++)
		printf("%d, ", C[i]);
	printf("\n");

	printf("Elementele vectorului sortat prin heap-sort: ");
	printf("\n");
	HeapSort(C, 100);
	for (int i = 0; i < 100; i++)
		printf("%d, ", C[i]);
	printf("\n");
	getchar();
}