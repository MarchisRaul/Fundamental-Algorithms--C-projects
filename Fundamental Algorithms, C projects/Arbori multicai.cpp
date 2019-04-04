#define _CRT_SECURE_NO_WARNINGS

// Nume : Marchis Raul grupa 30229
// Se cere implementarea corecta si eficienta a unor algoritme de complexitate liniara
// pentru transformarea arborilor multicai intre urmatoarele reprezentari.

// Algoritmii si modul de lucru este usor de inteles: trebuie sa transformam un vector de parinti
// intr-un arbore multicai (structura contine o cheie si un vector de noduri copil), iar pe urma, acest
// arbore multicai trebuie transformat intr-o reprezentare binara (fiecare nod contine cheia si alti doi pointeri:
// un copil si fratele nodului din dreapta.

// Complexitatea pentru cele doua transformari este O(n).
// Memoria aditionala consta in folosirea unor variabile ajutatoare in structurile nodurilor.

#include<stdlib.h>
#include<stdio.h>
#include "Profiler.h"

Profiler profiler;

int const nrMaximCopii = 3;



typedef struct multiWay {
	int key;
	int indiceCopilNou;
	int alCateleaCopil;
	struct multiWay *children[nrMaximCopii];
}MultiWayNode;



typedef struct binaryWay {
	int key;
	struct binaryWay *child;
	struct binaryWay *brother;
}BinaryWayNode;



void initParentRepresentation(MultiWayNode *multiWayTree, int nrElementeVectorParinti) {

	for (int i = 0; i < nrElementeVectorParinti; i++)
	{
		if (i == 0) {
			multiWayTree[i].key = -1;
			multiWayTree[i].indiceCopilNou = 0;
			multiWayTree[i].alCateleaCopil = 1;
			multiWayTree[i].children[0] = NULL;
			multiWayTree[i].children[1] = NULL;
			multiWayTree[i].children[2] = NULL;
		}
		else
		{
			multiWayTree[i].indiceCopilNou = 0;
			multiWayTree[i].key = i;
			multiWayTree[i].alCateleaCopil = 1;
			multiWayTree[i].children[0] = NULL;
			multiWayTree[i].children[1] = NULL;
			multiWayTree[i].children[2] = NULL;
		}
	}
}

MultiWayNode *newNodeMultiWay(int key) {
	MultiWayNode *aux = (MultiWayNode*)calloc(1, sizeof(MultiWayNode));
	aux->key = key;
	
	return aux;
}

void buildMultiWayTree(int *vectorParinti, MultiWayNode *multiWayTree, int nrElementeVectorParinti) {
	for (int i = 0; i < nrElementeVectorParinti; i++)
	{
		if (vectorParinti[i] == -1) {
			multiWayTree[0].key = 0;
			multiWayTree[0].children[0] = newNodeMultiWay(i + 1);
			multiWayTree[0].indiceCopilNou++;
		}
		else
		{
			multiWayTree[vectorParinti[i]].children[multiWayTree[vectorParinti[i]].indiceCopilNou] = newNodeMultiWay(i + 1);
			(multiWayTree[vectorParinti[i]].indiceCopilNou)++;
		}
	}
}

void prettyPrintMultiWayTree(MultiWayNode *multiWayTree, int nrElementeMultiWayTree, int nodCurent, int level) {
	for (int j = 0; j < multiWayTree[nodCurent].indiceCopilNou; j++)
	{
		for (int k = 0; k <= level; k++)
			printf(" ");
		printf("%d\n\n", multiWayTree[nodCurent].children[j]->key);
		prettyPrintMultiWayTree(multiWayTree, nrElementeMultiWayTree, multiWayTree[nodCurent].children[j]->key, level+3);	
	}
}

void prettyPrintParentTree(int *vectorParinti, int nrElementeVectorParinti, int curentNode, int level) {
	for (int i = 0; i < nrElementeVectorParinti; i++)
	{
		
		if (vectorParinti[i] == curentNode)
		{
			for (int k = 0; k <= level; k++)
				printf(" ");
			printf("%d\n\n", i + 1);
			prettyPrintParentTree(vectorParinti, nrElementeVectorParinti, i + 1, level + 2);
		}

	}
}

BinaryWayNode* newNodeBinaryWay(MultiWayNode *multiWayTree, int currentNode) {
	BinaryWayNode *aux = (BinaryWayNode*)calloc(1, sizeof(BinaryWayNode));
	aux->brother = NULL;
	aux->child = NULL;
	aux->key = -10;

	if (currentNode == 0)
	{
		if (multiWayTree[0].children[0] != NULL)
		{
			aux->key = multiWayTree[0].children[0]->key;
			aux->brother = NULL;
			aux->child = NULL;

			return aux;
		}
	}
	else
	{
		aux->key = currentNode;
		aux->brother = NULL;
		aux->child = NULL;

		return aux;
	}

	return NULL;
	
}

void buildBinaryWayTree(BinaryWayNode **binaryWayTree, MultiWayNode *multiWayTree, int currentNode, int lastNode) {
	
	if (multiWayTree[currentNode].children[0] == NULL)
	{

		*binaryWayTree = newNodeBinaryWay(multiWayTree, currentNode);
		if (*binaryWayTree != NULL && multiWayTree[lastNode].children[multiWayTree[lastNode].alCateleaCopil] != NULL && multiWayTree[lastNode].alCateleaCopil < 3)
			buildBinaryWayTree(&(*binaryWayTree)->brother, multiWayTree, multiWayTree[lastNode].children[multiWayTree[lastNode].alCateleaCopil++]->key, lastNode);
		return;
	}
	
	*binaryWayTree = newNodeBinaryWay(multiWayTree, currentNode);
	if(*binaryWayTree != NULL && multiWayTree[(*binaryWayTree)->key].children[0] != NULL)
		buildBinaryWayTree(&(*binaryWayTree)->child, multiWayTree, multiWayTree[(*binaryWayTree)->key].children[0]->key, (*binaryWayTree)->key);
	if (*binaryWayTree != NULL && multiWayTree[lastNode].children[multiWayTree[lastNode].alCateleaCopil] != NULL)
		buildBinaryWayTree(&(*binaryWayTree)->brother, multiWayTree, multiWayTree[lastNode].children[multiWayTree[lastNode].alCateleaCopil++]->key, lastNode);
}

void prettyPrintBinaryWayTree(BinaryWayNode **binaryWayTree, int level) {
	if (*binaryWayTree == NULL)
		return;

	for (int i = 0; i <= level; i++)
		printf(" ");
	printf("%d\n\n", (*binaryWayTree)->key);
	prettyPrintBinaryWayTree(&(*binaryWayTree)->child, level + 2);
	prettyPrintBinaryWayTree(&(*binaryWayTree)->brother, level);
}

int main() {
	
	int vectorParinti[] = { 2, 7, 5, 2, 7, 7, -1, 5, 2 };	// 9 elem
	int nrElementeVectorParinti = sizeof(vectorParinti) / sizeof(int);
	printf("Pretty print pentru parent representation: \n");
	prettyPrintParentTree(vectorParinti, nrElementeVectorParinti, -1, 0);
	printf("\n\n\n");




	int nrElementeMultiWayTree = sizeof(vectorParinti) / sizeof(int) + 1;	// 10
	MultiWayNode multiWayTree[10];
	initParentRepresentation(multiWayTree, nrElementeMultiWayTree);
	buildMultiWayTree(vectorParinti, multiWayTree, nrElementeVectorParinti);
	printf("Pretty print pentru multi-way representation: \n");
	prettyPrintMultiWayTree(multiWayTree, nrElementeMultiWayTree, 0, 0);
	printf("\n\n\n");


	BinaryWayNode *binaryWayTree = NULL;
	buildBinaryWayTree(&binaryWayTree, multiWayTree, 0, 0);
	//printf("%d %d", binaryWayTree->key, binaryWayTree->child->key);
	printf("Pretty print pentru binary-way representation: \n\n\n");
	prettyPrintBinaryWayTree(&binaryWayTree, 0);
	
	getchar();
}