#define _CRT_SECURE_NO_WARNINGS


/**
 * @author Marchis Raul
 * @group 30229
 *
 * Se cere implementarea corecta si eficienta a sortarii folosind heap­uri (Heapsort) si sortarea
rapida (Quicksort), cat si implementarea si exemplificarea corectitudinii pentru algoritmul QuickSelect, de 
gasire a celui mai mic k element din vectorul nesortat.
 *
									 COMPLEXITATILE ALGORITMILOR
								Heap-sort		Quick-sort	
 * Caz defavorabil :			O(nlogn)			O(n^2)				
   Caz mediu statistic :		O(nlogn)			O(nlogn)			
   Caz favorabil :				O(nlogn)			O(nlogn)				

		
		Cum functioneaza algoritmii : 
			Algoritmul Heap-sort este usor de inteles dupa ce pricepem logica implementarii bottom-up.
	Dupa apelul functiei, se  construieste heap-ul. Ceea ce inseamna, ca in radacina noi avem acum cel mai mare element din vector(tinand cont ca implementarea a fost
	facuta pe maxim. Astfel, interschimbam nodul radacina(cel mai mare) cu ultima frunza, pentru ca pune cel mai mare element din vector pe ultima pozitie din vectorul
	ce urmeaza a fi sortat crescator. Apelam din nou algoritmul, dar fara a lua in considerare acel ultim element care acum este cel mai mare si este la locul lui. Repetand algoritmul
	a doua oara, al doilea cel mai mare element din vector va fi pus pe penultima pozitie din vectorul sortat, adica la locul lui. Repetam algoritmul pana cand vectorul este sortat crescator,
	rezultand o complexitate O(nlogn).
			Algoritmul de construire a structurii de date Heap bottom-up(BuildMaxHeap + MaxHeapify) se desfasoara in felul urmator : pe vectorul cu elemente aranjate aleator,
	se incepe algoritmul. Incepand de la jumatatea vectorului, adica de la primul nod care nu este frunza (indicele i) si se verifica relatia cu copiii, care in vector
	se afla la indicii 2*i si 2*i + 1. In cazul de fata, eu am facut implementarea algoritmului pe maxim (radacina este cel mai mare nod din arbore), urmand ca
	ambii copii sa fie mai mici(fiecare parinte este mai mare ca si copiii sai). Se cauta cea mai mare valoare dintre nodul parinte si cei doi copii, si in 
	cazul in care unul dintre copii este mai mare, se interschimba nodul parinte cu nodul copil. Efectuandu-se aceasta schimbare, este posibil ca 
	pe mai departe, noul nod "copil" sa fie mai mare ca si copiii sai si astfel se continua algoritmul in continuare(recrusviv) pana cand relatia
	parinte - copii este inregula. Pe urma, se trece la urmatorul nod care nu este frunza(i-1) si se repeta aceleasi operatii pana cand ajungem la
	radacina (primul nod din arbore). In cazul in care relatia sa cu copiii este inregula, algoritmul se termina si heap-ul este gata construit.


			Algoritmul Quick-sort se bazeaza pe partitionarea vectorului. Partitionarea se face in felul urmator : alegem un element din vector, care sa fie "pivotul",
	in general acesta se alege ca fiind chiar ultimul element din vector (se poate alege si random, si primul element, etc), urmand ca prin algoritmul de partitionare, 
	elementele din vector sa fie sortate dupa urmatoarea regula : elemente mai mici decat pivotul , PIVOTUL, elemente mai mari decat pivotul. Se apeleaza astfel recursiv
	algoritmul de partitionare pe elementele mai mici strict decat pivotul, cat si pe cele mai mari strict decat pivotul. Recursivitatea se va relua pana cand elementele
	sunt sortate si algoritmul se termina.

			In ceea ce priveste compararea celor doua, se poate observa din grafice ca algoritmul Quick-sort este putin mai eficient decat Heap-sort in ceea ce priveste
	numarul total de comparatii si atribuiri efectuate pe datele de intrare. Totusi, daca ar fi sa vorbim strict de cazul defavorabil, atat din complexitatea celor doi
	algoritmi, cat si prin numarul de operatii enorme facute de quick-sort, observam ca heap-sort-ul ar castiga. Totusi, per total, intrucat majoritatea cazurilor de sortare
	au loc pe array-uri cu elemente random, quick-sort-ul ar fi varianta "to go".

			Cazul cel mai defavorabil caz pentru QuickSort este ca toate elementele sa fie deja sortate in ordine crescatoare si sa alegem pivotul pentru partitionare, ultimul element din vector.
	Astfel, va tot compara fiecare element din vector cu acel pivot, va face toate interschimbarile "fara niciun efect", urmand ca apoi algoritmul sa se reia pentru dimensiunea
	nr_elemente - 1, rezultand astfel o complexitate de O(n^2).

			Cazul cel mai favorabil caz pentru QuickSort este ca toate elementele sa fie deja sortate in ordine crescatoare, iar pivotul sa fie ales mereu elementul de la jumatatea vectorului si
	interschimbat mai apoi cu ultima pozitie din vector(elementul din mijloc va deveni astfel pivot). Astfel, dupa ce va compara elementele, va ajunge undeva la jumatatea vectorului 
	cand elementele nu vor fi mai mici decat pivotul, astfel, pivotul va fi readus din nou la jumatatea vectorului si algoritmul se va relua din nou pentru prima jumatate a vectorului.
	Numarul total de operatii va fi astfel mai scazut decat in cazul mediu statistic deoarece se va parcurge tot cate jumatate de elemente din fiecare vector curent.
 *
 */


int CaseOfTestingQuickSort = 0;

#include<stdlib.h>
#include<stdio.h>
#include "Profiler.h"

Profiler profiler;


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

void MaxHeapify(int *A, int i, int nr_elemente, int nr_elemente_nemodificat)
{
	int largest = 0;
	int l = Left(i);
	int r = Right(i);


	if (l < nr_elemente && A[l] > A[i])
	{
		profiler.countOperation("comparatii_HeapSort", nr_elemente_nemodificat, 1);
		largest = l;
	}
	else
	{
		int ok = 0;
		if (l < nr_elemente)
			ok = 1;
		if (ok == 1)
			profiler.countOperation("comparatii_HeapSort", nr_elemente_nemodificat, 1);
		largest = i;
	}


	if (r < nr_elemente && A[r] > A[largest])	
	{
		int ok = 0;
		if (r < nr_elemente)
			ok = 1;
		if (ok == 1)
			profiler.countOperation("comparatii_HeapSort", nr_elemente_nemodificat, 1);
		largest = r;
	}


	if (largest != i)
	{
		int aux = A[i];
		A[i] = A[largest];
		A[largest] = aux;
		profiler.countOperation("atribuiri_HeapSort", nr_elemente_nemodificat, 3);

		MaxHeapify(A, largest, nr_elemente, nr_elemente_nemodificat);
	}
}

void BuildMaxHeap(int *A, int nr_elemente, int nr_elemente_nemodificat)
{
	int i = 0;
	int heap_size = nr_elemente;

	for (i = heap_size / 2; i >= 0; i--)
		MaxHeapify(A, i, nr_elemente, nr_elemente_nemodificat);
}

void HeapSort(int *A, int nr_elemente, int nr_elemente_nemodificat)
{
	int heap_size = nr_elemente;
	BuildMaxHeap(A, nr_elemente, nr_elemente_nemodificat);

	for (int i = nr_elemente - 1; i >= 1; i--)
	{
		int aux = A[0];
		A[0] = A[i];
		A[i] = aux;
		profiler.countOperation("atribuiri_HeapSort", nr_elemente_nemodificat, 3);

		heap_size--;
		MaxHeapify(A, 0, heap_size, nr_elemente_nemodificat);
	}

}

// functia de partitionare pentru quickSort in cazul mediu statistic si defavorabil
int partition(int arr[], int low, int high, int nr_elemente)
{
	profiler.countOperation("atribuiri_QuickSort_MS", nr_elemente, 1);
	int pivot = arr[high];    
	int i = (low - 1);  

	for (int j = low; j <= high - 1; j++)
	{
		if(CaseOfTestingQuickSort == 0)
			profiler.countOperation("comparatii_QuickSort_MS", nr_elemente, 1);
		if (CaseOfTestingQuickSort == 1)
			profiler.countOperation("comparatii_QuickSort_DEFAV", nr_elemente, 1);
		if (CaseOfTestingQuickSort == 2)
			profiler.countOperation("comparatii_QuickSort_FAV", nr_elemente, 1);
		if (arr[j] <= pivot)
		{
			i++;    
			int aux = arr[i];
			arr[i] = arr[j];
			arr[j] = aux;
			if (CaseOfTestingQuickSort == 0)
				profiler.countOperation("atribuiri_QuickSort_MS", nr_elemente, 3);
			if (CaseOfTestingQuickSort == 1)
				profiler.countOperation("atribuiri_QuickSort_DEFAV", nr_elemente, 3);
			if (CaseOfTestingQuickSort == 2)
				profiler.countOperation("atribuiri_QuickSort_FAV", nr_elemente, 3);
		}
	}

	int aux = arr[i + 1];
	arr[i + 1] = arr[high];
	arr[high] = aux;
	if (CaseOfTestingQuickSort == 0)
		profiler.countOperation("atribuiri_QuickSort_MS", nr_elemente, 3);
	if (CaseOfTestingQuickSort == 1)
		profiler.countOperation("atribuiri_QuickSort_DEFAV", nr_elemente, 3);
	if (CaseOfTestingQuickSort == 2)
		profiler.countOperation("atribuiri_QuickSort_FAV", nr_elemente, 3);
	return (i + 1);
}

// functia de partitionare pentru quickSort in cazul favorabil
int partition_favorabil(int arr[], int low, int high, int nr_elemente)
{
	profiler.countOperation("atribuiri_QuickSort_FAV", nr_elemente, 1);
	int pivot_temp = (high + low) / 2;
	int auxiliar = arr[pivot_temp];
	arr[pivot_temp] = arr[high];
	arr[high] = auxiliar;

	int pivot = arr[high];
	int i = (low - 1);

	for (int j = low; j <= high - 1; j++)
	{
		profiler.countOperation("comparatii_QuickSort_FAV", nr_elemente, 1);
		
		if (arr[j] <= pivot)
		{
			i++;
			int aux = arr[i];
			arr[i] = arr[j];
			arr[j] = aux;

				profiler.countOperation("atribuiri_QuickSort_FAV", nr_elemente, 3);

		}
	}

	int aux = arr[i + 1];
	arr[i + 1] = arr[high];
	arr[high] = aux;
	
		profiler.countOperation("atribuiri_QuickSort_FAV", nr_elemente, 3);
	
	return (i + 1);
}

// functia de partitionare pentru quickSelect
int partition_quickSelect(int arr[], int low, int high)
{
	int pivot = arr[high];
	int i = (low - 1);

	for (int j = low; j <= high - 1; j++)
	{
	
		if (arr[j] <= pivot)
		{
			i++;
			int aux = arr[i];
			arr[i] = arr[j];
			arr[j] = aux;
			
		}
	}

	int aux = arr[i + 1];
	arr[i + 1] = arr[high];
	arr[high] = aux;
	
	return (i + 1);
}

void quickSort(int arr[], int low, int high, int nr_elemente)
{
	if (low < high)
	{
		int pi;
		if (CaseOfTestingQuickSort == 2)
			pi = partition_favorabil(arr, low, high, nr_elemente);
		else pi = partition(arr, low, high, nr_elemente);

		quickSort(arr, low, pi - 1, nr_elemente);
		quickSort(arr, pi + 1, high, nr_elemente);
	}
}


int QuickSelect(int arr[], int low, int high, int k)
{
	int poz = partition_quickSelect(arr, low, high);
	
	if (poz == k)
		return arr[k];
	else if (poz < k)
		return QuickSelect(arr, poz + 1, high, k);
	else
		return QuickSelect(arr, low, poz - 1, k);
}

int main()
{
	int A[10000], B[10000], C[10000], D[10000];	// A pentru HeapSort ; B pentru QuickSort mediu statistic; C pentru QuickSort defavorabil ; D pentru quicksort favorabil
	int nr_elemente = 10000;

	// start mediu statistic quickSort vs heapSort
	for (int m = 0; m < 5; m++) 
	for (int i = 100; i <= nr_elemente; i += 100)
	{
		
			FillRandomArray(A, i, 0, 10000, 0, 0);
		
			for (int j = 0; j < i; j++)
				B[j] = A[j];

			quickSort(B, 0, i - 1, i);
			HeapSort(A, i, i);
		
			
	}

	profiler.divideValues("comparatii_QuickSort_MS", 5);
	profiler.divideValues("atribuiri_QuickSort_MS", 5);
	profiler.divideValues("comparatii_HeapSort", 5);
	profiler.divideValues("atribuiri_HeapSort", 5);



	profiler.createGroup("comparatii_QUICKSORTvsHEAPSORT", "comparatii_HeapSort", "comparatii_QuickSort_MS");
	profiler.createGroup("atribuiri_QUICKSORTvsHEAPSORT", "atribuiri_HeapSort", "atribuiri_QuickSort_MS");

	profiler.addSeries("comp_plus_atrib_HeapSort", "comparatii_HeapSort", "atribuiri_HeapSort");
	profiler.addSeries("comp_plus_atrib_QuickSort", "comparatii_QuickSort_MS", "atribuiri_QuickSort_MS");
	profiler.createGroup("atrib_plus_comp_QUICKSvsHEAPS_MS", "comp_plus_atrib_HeapSort", "comp_plus_atrib_QuickSort");


	// stop mediu statistic quickSort vs heapSort




	// start cazul defavorabil pentru quickSelect
	
	CaseOfTestingQuickSort++;	// acum fac pentru cazul defavorabil la QuickSort, si am pus variabila globala ca in functia partition sa-mi contorizeze in alte variabile numarul de comparatii si atribuiri

	for (int i = 100; i <= nr_elemente; i += 100)
	{
		FillRandomArray(C, i, 0, 10000, 1, 1);
		
		quickSort(C, 0, i - 1, i);
		
	}

	profiler.addSeries("comp_plus_atrib", "comparatii_QuickSort_DEFAV", "atribuiri_QuickSort_DEFAV");
	profiler.createGroup("comp_plus_atrib_QuickSort_defav", "comp_plus_atrib", "comparatii_QuickSort_DEFAV", "atribuiri_QuickSort_DEFAV");


	// stop cazul defavorabil pentru quickSelect



	// start cazul favorabil pentru quickSelect

	CaseOfTestingQuickSort++;
	for (int i = 100; i <= nr_elemente; i += 100)
	{
		FillRandomArray(D, i, 0, 10000, 1, 1);
		quickSort(D, 0, i - 1, i);
	}

	profiler.addSeries("comp_plus_atrib_FAV", "comparatii_QuickSort_FAV", "atribuiri_QuickSort_FAV");
	profiler.createGroup("comp_plus_atrib_QuickSort_fav", "comp_plus_atrib_FAV", "comparatii_QuickSort_FAV", "atribuiri_QuickSort_FAV");

	
	// stop cazul favorabil pentru quickSelect


	// start demonstrare corectitudine QuickSelect
	int Q[7] = { 10, 4, 5, 8, 6, 11, 26 };
	int nr_elemente_qs = 7;
	
	printf("Vectorul inainte de QuickSelect: \n");
	for (int i = 0; i < nr_elemente_qs; i++)
		printf("%d ", Q[i]);
	printf("Introduceti al catelea cel mai mic element din vector k doriti sa-l gasiti: ");
	int k = 0;
	scanf("%d", &k);
	int k_cautat = QuickSelect(Q, 0, nr_elemente_qs - 1, k-1);

	printf("Elementul al k-lea cel mai mic din vectorul nesortat cautat este: %d", k_cautat);
	printf("\n\n\n");
	
	// stop demonstrare corectitudine QuickSelect

	profiler.showReport();
	getchar();
	getchar();


}