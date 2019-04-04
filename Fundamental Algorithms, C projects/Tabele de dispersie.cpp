#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

/**
 * @author Marchis Raul
 * @group 30229
 *
	Se cere implementarea corecta si eficienta a operatiilor de inserare si cautare intr-o tabela de dispersie
	ce foloseste adresarea deschisa cu verificare patratica.

	Explicarea in limbaj natural a algoritmului : o tabela de dispersie este folosita pentru a insera si cauta o cheie in O(1).
	Fiecare element, are o pozitie in tabela de dispersie data de o anumita "formula de hashing", care poate sa difere de la o implementare la alta.
	In formula luam in considerare si un indice i, care pleaca de la 0, si in cazul in care pt i = 0, pozitia din tabela este deja ocupata, incrementam i-ul
	si incercam formula pentru i = 1.
	Algoritmul de inserare continua pana gasim o pozitie goala sau daca tabela este deja plina, nu mai putem insera.
	Cautarea are loc la fel, doar ca ne oprim in cazul in care am gasit o celula "goala"(FREE) sau am gasit cheia cautata.

 */

#include<stdlib.h>
#include<stdio.h>
#include "Profiler.h"

Profiler profiler;

enum {FREE, OCCUPIED};

// structura tabelei de dispersie : fiecare element contine VALOAREA SA, cat si STATUS-UL CELULEI (free, occupied sau deleted)
typedef struct cell {
	int value;
	int status;
}Entry;


// functia de afisare a tabelei de dispersie
void afisareHashTable(Entry *hashTable, int nrElemente) {
	printf("\n\n Tabela de dispersie este: \n");

	for (int i = 0; i < nrElemente; i++)
	{
		if (hashTable[i].status == OCCUPIED)
			printf("hashTable[%d]=%d \n", i, hashTable[i]);
		else
			printf("hashTable[%d]= -- \n", i);
	}
}


// functia care reseteaza tabela de dispersie, "stergand fiecare element" prin punerea fiecarei celule pe FREE
void setHashTableFree(Entry *hashTable, int nrElemente) {
	for (int i = 0; i < nrElemente; i++)
	{
		hashTable[i].value = 0;
		hashTable[i].status = FREE;
	}
}


// functia verifica daca tabela este plina deja (returneaza 1)
int hashTableOccupied(Entry *hashTable, int nrElemente)
{
	int i;
	for (i = 0; i < nrElemente; i++)
	{
		if (hashTable[i].status == FREE) return 0;
	}
	return 1;
}

// functia h_prime face parte din functia de hashing pentru quadraticProbing
int hashTable_prime(int value, int nrElemente)
{
	return value % nrElemente;
}

// functia de hashing
int quadraticProbing(int value, int nrElemente, int i) {
	int hashKey = ((hashTable_prime(value, nrElemente) + i + (i*i)) % nrElemente);
	return hashKey;
}

// functia de inserare in tabela de dispersie
void insertQuadraticProbing(int value, Entry *hashTable, int nrElemente) {
	int i = 0;
	int hashKey = 0;
	int collision = 1;	// collision este o variabila care semnaleaza coliziunile si devine 0 cand gasim o celula FREE

	
	if (hashTableOccupied(hashTable, nrElemente) == 1)
	{
		printf("Nu se mai poate insera cheia %d, tabela este plina \n", value);
		return;
	}

	do {
		hashKey = quadraticProbing(value, nrElemente, i);
		if (hashTable[hashKey].status == FREE)
			collision = 0;
		i++;
	} while (collision);

	hashTable[hashKey].value = value;
	hashTable[hashKey].status = OCCUPIED;

}

// functia de search in tabela de dispersie
int searchHashTable(int value, Entry *hashTable, int nrElemente) {
	int i = 0;
	int hashKey = 0;
	int celuleAccesate = 0;

	do {
		hashKey = quadraticProbing(value, nrElemente, i);
		celuleAccesate++;
		if (hashTable[hashKey].status == OCCUPIED && hashTable[hashKey].value == value)
		{
			
			return celuleAccesate;
		}
		i++;
	} while (hashTable[hashKey].status != FREE && i<nrElemente);

	return celuleAccesate;
}

int main() {

	Entry *test = (Entry*)calloc(17, sizeof(Entry));
	int testNrElem = 17;
	setHashTableFree(test, testNrElem);
	insertQuadraticProbing(3, test, testNrElem);
	insertQuadraticProbing(2, test, testNrElem);
	insertQuadraticProbing(4, test, testNrElem);
	insertQuadraticProbing(10, test, testNrElem);
	insertQuadraticProbing(14, test, testNrElem);
	insertQuadraticProbing(12, test, testNrElem);
	insertQuadraticProbing(20, test, testNrElem);
	insertQuadraticProbing(19, test, testNrElem);
	insertQuadraticProbing(40, test, testNrElem);
	insertQuadraticProbing(100, test, testNrElem);
	insertQuadraticProbing(1, test, testNrElem);
	insertQuadraticProbing(22, test, testNrElem);
	insertQuadraticProbing(32, test, testNrElem);
	insertQuadraticProbing(50, test, testNrElem);
	insertQuadraticProbing(61, test, testNrElem);
	insertQuadraticProbing(55, test, testNrElem);
	afisareHashTable(test, testNrElem);
	printf("Numarul de celule accesate pentru a cauta cheia %d este: %d \n", 20, searchHashTable(20, test, testNrElem));
	printf("Numarul de celule accesate pentru a cauta cheia %d este: %d \n", 2, searchHashTable(2, test, testNrElem));
	printf("Numarul de celule accesate pentru a cauta cheia %d este: %d \n", 2001, searchHashTable(2001, test, testNrElem));
	printf("Numarul de celule accesate pentru a cauta cheia %d este: %d \n", 40, searchHashTable(40, test, testNrElem));
	printf("Numarul de celule accesate pentru a cauta cheia %d este: %d \n", 1, searchHashTable(1, test, testNrElem));


	FILE *f = fopen("EfortHashTable.csv", "w");
	fprintf(f, "%s,%s,%s,%s,%s\n", "Factor de umplere", "Efort mediu gasite", "Efort maxim gasite", "Efort mediu ne-gasite", "Efort maxim ne-gasite");


	int nrTotalElemente = 10007;
	Entry *hashTable = (Entry*)calloc(nrTotalElemente, sizeof(Entry));
	setHashTableFree(hashTable, nrTotalElemente);
	




	float factorUmplere[] = { 0.8f, 0.85f, 0.9f, 0.95f, 0.99f };
	int nrFactoriUmplere = sizeof(factorUmplere) / sizeof(float);

	int vectorAuxiliar[10007];
	int nrElemente = 0;

	int vectorElementeGasite[1500];
	int vectorElementeNegasite[1500];
	int indiceDeplasare = 0;

	
		for (int i = 0; i < nrFactoriUmplere; i++)
		{
			int efortMediuGasite = 0;
			int efortMaximGasite = -1;
			int efortMediuNegasite = 0;
			int efortMaximNegasite = -1;

			for (int q = 0; q < 5; q++) {
				

				setHashTableFree(hashTable, nrTotalElemente);
				int aux = 0;

				nrElemente = factorUmplere[i] * nrTotalElemente;
				FillRandomArray(vectorAuxiliar, nrElemente, 1, 15000, 1, 0);

				indiceDeplasare = nrElemente / 1500 - 1;
				//indiceDeplasare = 5;

				for (int w = 0; w < nrElemente; w++)
					insertQuadraticProbing(vectorAuxiliar[w], hashTable, nrTotalElemente);
				

				for (int r = 0; r < 1500; r++)
				{
					vectorElementeGasite[r] = vectorAuxiliar[aux];
					aux += indiceDeplasare;
				}

				

				for (int t = 0; t < 1500; t++)
				{
					int efort = searchHashTable(vectorElementeGasite[t], hashTable, nrTotalElemente);
					if (efort > efortMaximGasite)
						efortMaximGasite = efort;
					efortMediuGasite += efort;
				}

				

				FillRandomArray(vectorElementeNegasite, 1500, 16000, 30000, 1, 0);
				for (int y = 0; y < 1500; y++)
				{
					int efort = searchHashTable(vectorElementeNegasite[y], hashTable, nrTotalElemente);
					if (efort > efortMaximNegasite)
						efortMaximNegasite = efort;
					efortMediuNegasite += efort;
				}

				
			}

			fprintf(f, "%f,%d,%d,%d,%d\n", factorUmplere[i], efortMediuGasite/7500, efortMaximGasite, efortMediuNegasite/7500, efortMaximNegasite);

		}


	free(hashTable);
	getchar();
	return 0;
}