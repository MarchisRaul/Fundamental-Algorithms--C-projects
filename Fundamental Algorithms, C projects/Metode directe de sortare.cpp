#include<stdio.h>
#include "Profiler.h"

/**
 * @author Marchis Raul
 * @group 30229
 *
 * Comparati metodele de sortate : Bubble-sort, Selection-sort si Insertion-sort
 *
									 COMPLEXITATILE ALGORITMILOR					
							Bubble-sort		Insertion-sort		Selection-sort
 * Caz defavorabil :			O(n^2)			O(n^2)				O(n^2)
   Caz mediu statistic :		O(n^2)			O(n^2)				O(n^2)
   Caz favorabil :				O(n)			O(n)				O(n^2)

		Vorbind despre complexitatea spatiului, pentru toti cei trei algoritmi avem O(1).
		Legat de numarul de atribuiri + comparatii efectuate pe toti cei trei algoritmii,
   se poate observa ca, in cazul defavorabil, cel mai eficient ar fi selection-sort-ul,
   insertion-sort-ul si pe urma bubble-sort-ul, desi toti trei algoritmii au complexitatea O(n^2).
		In cazul favorabil, legat de atribuiri + comparatii, se pare ca insertion-sort-ul este considerabil
   cel mal eficient dintre toate trei. Totodata, o observatie legata de cazul favorabil : atribuirile sunt 0
   pentru toti trei algoritmii, intrucat in implementarile facute, daca vectorii sunt deja sortati nu va avea loc 
   nici macar o atribuire.
		In cazul mediu statistic, se pare ca selection-sort-ul este cel mai eficient dintre toate trei, fiind urmat apoi de
   insertion-sort si in final de bubble-sort.


		Avand in vedere, ca majoritatea cazurilor de sortare sunt in cazul mediu statistic si foarte rar in cazul defavorabil,
   selection-sort-ul pare a fi cea mai buna alegere (in cazul favorabil, tot el) intrucat suma atribuirilor si a comparatiilor
   este mai mica ca la celelalte doua metode de sortare.

		Legat de stabilitatea algoritmilor, bubble-sort-ul si insertion-sort-ul sunt stabili, iar selection sort-ul este instabil.

 * 
 */



Profiler profiler;

int caz_of_testing = 0;

void bubble_sort(int *a, int n)
{
	for (int i = 0; i < n; i++) {							// comparam elementele doua cate doua
		for (int j = 0; j < n - 1; j++) {

			if(caz_of_testing == 0)
				profiler.countOperation("bubble_comparatii_MS", n);
			if(caz_of_testing == 1)
				profiler.countOperation("bubble_comparatii_FAV", n);
			if (caz_of_testing == 2)
				profiler.countOperation("bubble_comparatii_DEFAV", n);

			if (a[j] > a[j + 1]) {
				int aux = a[j];
				a[j] = a[j + 1];
				a[j + 1] = aux;

				if(caz_of_testing == 0)
					profiler.countOperation("bubble_atribuiri_MS", n, 3);
				//if(caz_of_testing == 1)
					//profiler.countOperation("bubble_atribuiri_FAV", n, 3);
				if(caz_of_testing == 2)
					profiler.countOperation("bubble_atribuiri_DEFAV", n, 3);

			}
			if (caz_of_testing == 1)
				profiler.countOperation("bubble_atribuiri_FAV", n, 0);	// dupa vreo 2 ore mi-am dat si eu seama ca defapt, in cazul favorabil nu intra niciodata pe ramura asta, asa ca l-am scos in afara if-ului ca sa apara totusi pe grafice
		}
	}
}

void insertion_sort(int *a, int n) {
	for (int i = 1; i < n; i++) {		// selectam fiecare element, incepand de la al doilea, si ii facem loc intre elementele de dinainte
		int ok = 0;
		int j = i;
		while (j > 0 && ok == 0) {

			if(caz_of_testing == 0)
				profiler.countOperation("insertion_comparatii_MS", n);
			if(caz_of_testing == 1)
				profiler.countOperation("insertion_comparatii_FAV", n);
			if(caz_of_testing == 2)
				profiler.countOperation("insertion_comparatii_DEFAV", n);

			if (a[j - 1] > a[j]) {
				int aux = a[j - 1];
				a[j - 1] = a[j];
				a[j] = aux;
				j--;

				if (caz_of_testing == 0)
					profiler.countOperation("insertion_atribuiri_MS", n, 3);
				//if(caz_of_testing == 1)
					//profiler.countOperation("insertion_atribuiri_FAV", n, 3);
				if(caz_of_testing == 2)
					profiler.countOperation("insertion_atribuiri_DEFAV", n, 3);

			}
			else
				ok = 1;
			if (caz_of_testing == 1)
				profiler.countOperation("insertion_atribuiri_FAV", n, 0);
		}
	}
	
}

void selection_sort(int *a, int n) {
	int min, i, j, aux;
	for (i = 0; i < n - 1; i++)		// am facut pe foaie, mergem numai pana la n-1 pentru ca dupa ce tot gasim minimele, cand ajungem la penultimul element, in cel mai rau caz il interschimbam cu ultimul, dar ultimul sigur nu mai		
	{
		min = i;
		for (j = i + 1; j < n; j++) {

			if(caz_of_testing == 0)
				profiler.countOperation("selection_comparatii_MS", n);
			if(caz_of_testing == 1)
				profiler.countOperation("selection_comparatii_FAV", n);
			if(caz_of_testing == 2)
				profiler.countOperation("selection_comparatii_DEFAV", n);

			if (a[j] < a[min])
				min = j;
		}
		if (min != i) {
			aux = a[min];
			a[min] = a[i];
			a[i] = aux;

			if(caz_of_testing == 0)
				profiler.countOperation("selection_atribuiri_MS", n, 3);
			//if(caz_of_testing == 1)
				//profiler.countOperation("selection_atribuiri_FAV", n, 3);
			if(caz_of_testing == 2)
				profiler.countOperation("selection_atribuiri_DEFAV", n, 3);

		}
		if (caz_of_testing == 1)
			profiler.countOperation("selection_atribuiri_FAV", n, 0);
	}
}

int main()
{
	int a[11000], b[11000], c[11000];	// a este pentru bubble-sort, b pentru insertion-sort, c pentru selection-sort
	int n;




	 //inceput grafice pentru cazul mediu statistic
	
		for (n = 100; n <= 10000; n += 100) {
			for (int i = 0; i < 5; i++) {
				FillRandomArray(a, n, 0, 10000, 0, 0);
				for (int i = 0; i < n; i++) {
					b[i] = a[i];
					c[i] = a[i];
				}
				bubble_sort(a, n);
				insertion_sort(b, n);
				selection_sort(c, n);

			}
			
			profiler.faMedia("bubble_comparatii_MS", n, 5);
			profiler.faMedia("insertion_comparatii_MS", n, 5);
			profiler.faMedia("selection_comparatii_MS", n, 5);
			profiler.faMedia("insertion_atribuiri_MS", n, 5);
			profiler.faMedia("bubble_atribuiri_MS", n, 5);
			profiler.faMedia("selection_atribuiri_MS", n, 5);
		}

	
	

	profiler.createGroup("Comparation_CazMS", "bubble_comparatii_MS", "insertion_comparatii_MS", "selection_comparatii_MS");
	profiler.createGroup("Atribution_CazMS", "bubble_atribuiri_MS", "insertion_atribuiri_MS", "selection_atribuiri_MS");
	profiler.addSeries("Sum_MS_bubble", "bubble_comparatii_MS", "bubble_atribuiri_MS");
	profiler.addSeries("Sum_MS_insertion", "insertion_comparatii_MS", "insertion_atribuiri_MS");
	profiler.addSeries("Sum_MS_selection", "selection_comparatii_MS", "selection_atribuiri_MS");
	profiler.createGroup("CompPlusAtrib_CazMS", "Sum_MS_bubble", "Sum_MS_insertion", "Sum_MS_selection");
	
	

	// sfarsit grafice pentru cazul mediu statistic





	caz_of_testing++;		// acum, caz_of_testing este 1, deci o sa intre pe a doua ramura a if-ului din functii, adica pe cazul favorabil


	// inceput grafice pentru cazul favorabil

	for (n = 100; n <= 10000; n += 100) {
		FillRandomArray(a, n, 0, 10000, 0, 1);
		for (int i = 0; i < n; i++) {
			b[i] = a[i];
			c[i] = a[i];
		}
		bubble_sort(a, n);
		insertion_sort(b, n);
		selection_sort(c, n);
	}

	profiler.createGroup("Comparation_CazFAV", "bubble_comparatii_FAV", "insertion_comparatii_FAV", "selection_comparatii_FAV");
	profiler.createGroup("Atribution_CazFAV", "bubble_atribuiri_FAV", "insertion_atribuiri_FAV", "selection_atribuiri_FAV");
	profiler.addSeries("Sum_FAV_bubble", "bubble_comparatii_FAV", "bubble_atribuiri_FAV");
	profiler.addSeries("Sum_FAV_insertion", "insertion_comparatii_FAV", "insertion_atribuiri_FAV");
	profiler.addSeries("Sum_FAV_selection", "selection_comparatii_FAV", "selection_atribuiri_FAV");
	profiler.createGroup("CompPlusAtrib_CazFAV", "Sum_FAV_bubble", "Sum_FAV_insertion", "Sum_FAV_selection");

	// sfarsit grafice pentru cazul favorabil






	caz_of_testing++;		// acum, caz_of_testing este 2, deci o sa intre pe al treilea if in functiile de sortare

	 //inceput grafice pentru cazul defavorabil

	for (n = 100; n <= 10000; n += 100) {
		FillRandomArray(a, n, 0, 10000, 0, 2);
		for (int i = 0; i < n; i++) {
			b[i] = a[i];
			c[i] = a[i];
			//c[0] = 20000;	pentru selection-sort, cazul defavorabil este atunci cand primul element din vector
			//este cel mai mare, urmand ca restul de la c[1] -> c[n] sa fie ordonate crescator,
			//dar am lasat ordonarea descrescatoare a vectorului.
		}
		bubble_sort(a, n);
		insertion_sort(b, n);
		selection_sort(c, n);
	}

	profiler.createGroup("Comparation_CazDEFAV", "bubble_comparatii_DEFAV", "insertion_comparatii_DEFAV", "selection_comparatii_DEFAV");
	profiler.createGroup("Atribution_CazDEFAV", "bubble_atribuiri_DEFAV", "insertion_atribuiri_DEFAV", "selection_atribuiri_DEFAV");
	profiler.addSeries("Sum_DEFAV_bubble", "bubble_comparatii_DEFAV", "bubble_atribuiri_DEFAV");
	profiler.addSeries("Sum_DEFAV_insertion", "insertion_comparatii_DEFAV", "insertion_atribuiri_DEFAV");
	profiler.addSeries("Sum_DEFAV_selection", "selection_comparatii_DEFAV", "selection_atribuiri_DEFAV");
	profiler.createGroup("CompPlusAtrib_CazDEFAV", "Sum_DEFAV_bubble", "Sum_DEFAV_insertion", "Sum_DEFAV_selection");

	// sfarsit grafice pentru cazul defavorabil
	
	profiler.showReport();
	getchar();

}
	
