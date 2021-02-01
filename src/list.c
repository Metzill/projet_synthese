#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

/**
 * @brief
 * Construire et initialiser un nouveau nœud d'une liste doublement chaînée.
 * Renvoie le nouveau nœud créé.
 */
static LNode * newLNode(void * data) {
	/* A FAIRE */
}

List * newList(void (*viewData)(const void*), void (*freeData)(void*)) {
	/* A FAIRE */
}

void freeList(List * L, int deleteData) {
	assert(deleteData == 0 || deleteData == 1);
	/* A FAIRE */
}

void viewList(const List * L) {
	/* A FAIRE */
}

void listInsertFirst(List * L, void * data) {
	/* A FAIRE */
}

void listInsertLast(List * L, void * data) {
	/* A FAIRE */
}

void listInsertAfter(List * L, void * data, LNode * ptrelm) {
	/* A FAIRE */
}

LNode* listRemoveFirst(List * L) {
	assert(L->head);
	/* A FAIRE */
}

LNode* listRemoveLast(List * L) {
	assert(L->head);
	/* A FAIRE */
}

LNode* listRemoveNode(List * L, LNode * node) {
	/* A FAIRE */
}