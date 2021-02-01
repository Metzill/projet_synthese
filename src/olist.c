#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include "olist.h"

/**
 * @brief
 * Construire et initialiser un nouveau nœud d'une liste doublement chaînée ordonnée.
 * Renvoie le nouveau nœud créé.
 */
static OLNode * newOLNode(void* key, void* data) {
	/* A FAIRE */
}

OList * newOList(int (*preceed)(const void*, const void*),
				void (*viewKey)(const void*), void (*viewData)(const void*),
				void (*freeKey)(void*), void (*freeData)(void*)) {
	/* A FAIRE */
}

void freeOList(OList * L, int deleteKey, int deleteData) {
	assert(deleteKey == 0 || deleteKey == 1);
	assert(deleteData == 0 || deleteData == 1);
	/* A FAIRE */
}

void viewOList(const OList * L) {
	/* A FAIRE */
}

void OListInsert(OList * L, void * key, void * data) {
	/* A FAIRE */
}

List* OListToList(const OList* L) {
	/* A FAIRE */
}