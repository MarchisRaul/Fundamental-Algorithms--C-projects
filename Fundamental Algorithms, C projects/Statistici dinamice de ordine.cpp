#define _CRT_SECURE_NO_WARNINGS

#include<stdlib.h>
#include<stdio.h>
#include "Profiler.h"

/**
 * @author Marchis Raul
 * @group 30229
 *
	Se cere implementarea corecta si eficienta a operatiilor de management ale unui arbore de statistica de ordine.

	Explicarea in limbaj natural a algoritmului : Fiind dat un vector cu n elemente crescatoare, retinem vectorul sub forma de arbore binar de cautare echilibrat.
	Odata cu creearea arborelui, setam si size-ul fiecarui nod din arbore (cate elemente are in subarborele stang + cate elemente are in subarborele drept + 1(el insusi))
	pentru ca ne vom folosi de el in operatia de cautare si de stergere a nodurilor.
	Operatiile de efectuat pe arbore sunt : SO-SELECT-UL : cautam elementul cu a k-a cea mai mica cheie dintre elemente.
											SO-DELET-UL : folosim SO-SELECT-UL pentru a stabili care este nodul cu a k-a cea mai mica cheie dintre toate elementele, urmand ca pe urma
											sa stergem nodul din arbore, sa refacem size-urile (la stergerea unui nod, pe ramura de mostenire in sus spre radacina, size-ul fiecarui nod
											scade cu 1) si sa ne asiguram ca arborele ramane binar de cautare.
	Personal, m-am gandit ca "actualizarea size-urilor" poate fi facuta direct la cautarea nodului in arbore (folosind SO-SELECT-UL), astfel nu mai avem nevoie de o parcurgere de la nod-ul de sters
	pe relatia parinte pana la radacina pentru a scadea size-ul nodurilor, deoarece facem acest lucru direct in cautarea nodului de sters.




 */


Profiler profiler;

int pr0filer = 0;

// structura de tipul fiecarui nod din arbore
typedef struct nodetype
{
	int key;
	int size;
	struct nodetype *left;
	struct nodetype *right;
	struct nodetype *parent;
}NodeT;

/*/
void preOrder(NodeT *root) {
	NodeT *aux = root;
	if (aux != NULL)
	{
		printf("%d(%d) ", aux->key, aux->parent != NULL ? aux->parent->key : -1);
		preOrder(aux->left);
		preOrder(aux->right);
	}
}
*/

// functia pretty print a arborelui, parcurgerea se face in inordine
void inOrder(NodeT *root, int level)
{
	if (root != NULL)
	{
		inOrder(root->left, ++level);
		for (int i = 0; i < level; i++)
			printf("            ");
		printf("%d(size %d) \n", root->key, root->size);
		inOrder(root->right, level);
	}
}

// functia pentru constructia arborelui folosind vectorul de elemente crescatoare
NodeT *BuildTree(NodeT *root, int *a, int low, int high) {
	if (high < low)
		return NULL;
	
	int mediana = (low + high) / 2;
	NodeT *aux = (NodeT*)calloc(1, sizeof(NodeT));
	aux->parent = NULL;

	aux->key = a[mediana];
	
	aux->left = BuildTree(root, a, low, mediana - 1);
	
	aux->right = BuildTree(root, a, mediana + 1, high);
	
	

	root = aux;

	if (root->left != NULL)
		(root->left)->parent = root;
	if (root->right != NULL)
		(root->right)->parent = root;

	if (root->left == NULL && root->right == NULL)
		root->size = 1;
	if (root->left != NULL && root->right == NULL)
		root->size = (root->left)->size + 1;
	if (root->left == NULL && root->right != NULL)
		root->size = (root->right)->size + 1;
	if (root->left != NULL && root->right != NULL)
		root->size = (root->left)->size + (root->right)->size + 1;

	return root;

}

// functia returneaza minimul dintr-un arbore trimis ca parametru
NodeT *findTreeMin(NodeT *root, int scadeSize, int nrElemTotal) {
	NodeT *aux = root;
	while (aux->left != NULL) {
		if (pr0filer == 1)
			profiler.countOperation("osDelete", nrElemTotal, 2);
		if(scadeSize == 1)
			aux->size--;
		aux = aux->left;
	}
	return aux;
}

// functie ce returneaza succesorul unui nod trimis ca parametru
NodeT *returnSuccesor(NodeT *root, int scadeSize, int nrElemTotal) {
	if (pr0filer == 1)
		profiler.countOperation("osDelete", nrElemTotal, 1);
	if (root->right != NULL)
		return findTreeMin(root->right, scadeSize, nrElemTotal);
	NodeT *parentOfRoot = root->parent;
	while (parentOfRoot != NULL && root == parentOfRoot->right)
	{
		if (pr0filer == 1)
			profiler.countOperation("osDelete", nrElemTotal, 2);
		if(scadeSize == 1)
			parentOfRoot->size--;
		root = parentOfRoot;
		parentOfRoot = root->parent;
	}
	return parentOfRoot;
}

// functia OS-SELECT, care returneaza al k-lea element dorit din arbore / vector
NodeT *osSelect(NodeT *root, int alCateleaElement, int scadeSize, int nrElemTotal) {
	int rangRoot;

	if (pr0filer == 1)
		profiler.countOperation("osSelect", nrElemTotal, 1);
	if (root != NULL) {
		if (pr0filer == 1)
			profiler.countOperation("osSelect", nrElemTotal, 1);
		if (root->left != NULL)
		{
			if (pr0filer == 1)
				profiler.countOperation("osSelect", nrElemTotal, 1);
			rangRoot = (root->left)->size + 1;
		}
		else if (root->left == NULL)
		{
			if (pr0filer == 1)
				profiler.countOperation("osSelect", nrElemTotal, 1);
			rangRoot = 1;
		}

		if (pr0filer == 1)
			profiler.countOperation("osSelect", nrElemTotal, 1);
		if (alCateleaElement == rangRoot)
		{
			if (scadeSize == 1)
			{
				root->size = root->size - 1;
				if (pr0filer == 1)
					profiler.countOperation("osSelect", nrElemTotal, 1);
			}
			return root;
		}
		else if (alCateleaElement < rangRoot)
		{
			if (pr0filer == 1)
				profiler.countOperation("osSelect", nrElemTotal, 1);

			if (scadeSize == 1)
			{
				root->size = root->size - 1;
				if (pr0filer == 1)
					profiler.countOperation("osSelect", nrElemTotal, 1);

			}
			return osSelect(root->left, alCateleaElement, scadeSize, nrElemTotal);
		}
		else {
			if (scadeSize == 1)
			{
				root->size = root->size - 1;
				if (pr0filer == 1)
					profiler.countOperation("osSelect", nrElemTotal, 1);

			}
			return osSelect(root->right, alCateleaElement - rangRoot, scadeSize, nrElemTotal);
		}
	}
	return NULL;
}

// functia OS-DELETE, care sterge al k-lea element dorit din arbore / vector
void osDelete(NodeT **root, int alCateleaElement, int nrElemTotal) {
	int scadeSize = 1;
	NodeT *nodCautat = osSelect(*root, alCateleaElement, scadeSize, nrElemTotal);
	NodeT *nodInlocuitor;
	NodeT *copilNodCautat;

	if (pr0filer == 1)
		profiler.countOperation("osDelete", nrElemTotal, 1);
	if (nodCautat != NULL) {
		if (pr0filer == 1)
			if(nodCautat->left == NULL)
					profiler.countOperation("osDelete", nrElemTotal, 1);
			else
				profiler.countOperation("osDelete", nrElemTotal, 2);
		if (nodCautat->left == NULL || nodCautat->right == NULL)
		{
			nodInlocuitor = nodCautat;
			if (pr0filer == 1)
				profiler.countOperation("osDelete", nrElemTotal, 1);
		}
		else
		{
			if (pr0filer == 1)
				profiler.countOperation("osDelete", nrElemTotal, 1);
			nodInlocuitor = returnSuccesor(nodCautat, scadeSize, nrElemTotal);
		}

		if (pr0filer == 1)
			profiler.countOperation("osDelete", nrElemTotal, 1);
		if (nodInlocuitor->left != NULL)
		{
			copilNodCautat = nodInlocuitor->left;
			if (pr0filer == 1)
				profiler.countOperation("osDelete", nrElemTotal, 1);
		}
		else
		{
			copilNodCautat = nodInlocuitor->right;
			if (pr0filer == 1)
				profiler.countOperation("osDelete", nrElemTotal, 1);
		}

		if (pr0filer == 1)
			profiler.countOperation("osDelete", nrElemTotal, 1);
		if (copilNodCautat != NULL)
		{
			copilNodCautat->parent = nodInlocuitor->parent;
			if (pr0filer == 1)
				profiler.countOperation("osDelete", nrElemTotal, 1);
		}

		if (pr0filer == 1)
			profiler.countOperation("osDelete", nrElemTotal, 1);
		if (nodInlocuitor->parent == NULL)
		{
			*root = copilNodCautat;
			if (pr0filer == 1)
				profiler.countOperation("osDelete", nrElemTotal, 1);
		}
		else if (nodInlocuitor == (nodInlocuitor->parent)->left)
		{
			(nodInlocuitor->parent)->left = copilNodCautat;
			if (pr0filer == 1)
				profiler.countOperation("osDelete", nrElemTotal, 2);
		}
		else
		{
			(nodInlocuitor->parent)->right = copilNodCautat;
			if (pr0filer == 1)
			profiler.countOperation("osDelete", nrElemTotal, 1);
		}

		if (pr0filer == 1)
			profiler.countOperation("osDelete", nrElemTotal, 1);
		if (nodInlocuitor != nodCautat)
		{
			nodCautat->key = nodInlocuitor->key;
			if (pr0filer == 1)
			profiler.countOperation("osDelete", nrElemTotal, 1);
		}
	
	}
}

int main()
{
	// start demo
	NodeT *root = (NodeT*)calloc(1, sizeof(NodeT));
	int a[10000];
	int nrElemDemo = 11;
	FillRandomArray(a, nrElemDemo, 1, 11, 1, 1);
	int scadeSize = 0;
	root = BuildTree(root, a, 0, nrElemDemo - 1);

	int level = 0;
	
	int randomHigh = nrElemDemo;
	while (randomHigh > 0)
	{
		inOrder(root, level);
		osDelete(&root, rand() % (randomHigh + 1 - 1) + 1, nrElemDemo);
		printf("\n\n\n\n");
		randomHigh--;
	}
	// stop demo




	// start implementare profiler
	pr0filer++;
	int nrElem = 0;
	for (int i = 0; i < 5; i++)
	for (int n = 100; n < 10000; n += 100)
	{

			nrElem = n;
			FillRandomArray(a, nrElem, 1, nrElem, 1, 1);
			root = BuildTree(root, a, 0, nrElem - 1);

			randomHigh = nrElem;
			while (randomHigh > 0)
			{
				osDelete(&root, rand() % (randomHigh + 1 - 1) + 1, n);
				randomHigh--;
			}
	

	}

	profiler.divideValues("osSelect", 5);
	profiler.divideValues("osDelete", 5);

	profiler.addSeries("osDeleteTotal", "osSelect", "osDelete");
	profiler.createGroup("soSelect vs soDelete", "osSelect", "osDeleteTotal");
	profiler.showReport();
	// stop implementare profiler


	getchar();
}