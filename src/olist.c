#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include "olist.h"
#include "utilities.h"

/**
 * @brief
 * Construire et initialiser un nouveau nœud d'une liste doublement chaînée ordonnée.
 * Renvoie le nouveau nœud créé.
 */
static OLNode * newOLNode(void* key, void* data) {
    OLNode *new = (OLNode  *) malloc(1 * sizeof(OLNode));
    new->key=key;
    new->data=data;
    new->succ=NULL;
    new->pred=NULL;
    return new;
}

OList * newOList(int (*preceed)(const void*, const void*),
				void (*viewKey)(const void*), void (*viewData)(const void*),
				void (*freeKey)(void*), void (*freeData)(void*)) {
    OList *new = (OList*) malloc(1*sizeof(OList));
    new->numelm=0;
    new->preceed=preceed;
    new->viewKey=viewKey;
    new->freeKey=freeKey;
    new->viewData=viewData;
    new->freeData=freeData;
    new->head=NULL;
    new->tail=NULL;
    return new;
}

void freeOList(OList * L, int deleteKey, int deleteData) {
	assert(deleteKey == 0 || deleteKey == 1);
	assert(deleteData == 0 || deleteData == 1);
    OLNode *cur=L->head;
    while(cur!=NULL){
        if(deleteData==1)
            L->freeData(cur->data);
        if(deleteKey==1)
            L->freeKey(cur->key);
        OLNode *aux=cur;
        cur=cur->succ;
        free(aux);
    }
    free(L);
}

void viewOList(const OList * L) {
	assert(L!=NULL);
	OLNode *Tete = L->head;
	while(Tete!=NULL){
      printf("Key : ");
	    L->viewKey(Tete->key);
      printf(" / Data : ");
	    L->viewData(Tete->data);
	    printf("\n");
	    Tete=Tete->succ;
	}
	free(Tete);
}

void OListInsert(OList * L, void * key, void * data) {
	OLNode *newNode = newOLNode(key, data);
	if(L->numelm==0) {
	    L->head=newNode;
	    L->tail=newNode;
	    L->numelm++;
	}else{
	    OLNode *Tete = L->head;
        while ( (Tete!=NULL) && (L->preceed(Tete->key,newNode->key)) < 1 ){
            Tete=Tete->succ;
        }
        if (Tete == NULL){
            L->tail->succ=newNode;
            newNode->pred=L->tail;
            L->tail=newNode;
            L->numelm++;
        }else if(Tete==L->head){
            L->head->pred=newNode;
            newNode->succ=L->head;
            L->head=newNode;
            L->numelm++;
        }else{
            Tete->pred->succ=newNode;
            newNode->pred=Tete->pred;
            newNode->succ=Tete;
            Tete->pred=newNode;
            L->numelm++;
        }
	}
}

List* OListToList(const OList* L) {
	/* A FAIRE */
	assert(L!=NULL);
	List *nL = newList(L->viewData,L->freeData);
    OLNode *Tete = L->head;
    while(Tete!=NULL){
        listInsertLast(nL,Tete->data);
        Tete = Tete->succ;
	}
    return nL;
}

void TestOList () {
    OList *OL = newOList(compareInt,viewInt,viewInt,freeInt,freeInt);
    int *k1=malloc(1 * sizeof(int));
    *k1=1;
    int *d1=malloc(1 * sizeof(int));
    *d1=12;
    int *k2=malloc(1 * sizeof(int));
    *k2=2;
    int *d2=malloc(1 * sizeof(int));
    *d2=24;
    int *k3=malloc(1 * sizeof(int));
    *k3=3;
    int *d3=malloc(1 * sizeof(int));
    *d3=36;
    int *k4=malloc(1 * sizeof(int));
    *k4=4;
    int *d4=malloc(1 * sizeof(int));
    *d4=48;
    int *k5=malloc(1 * sizeof(int));
    *k5=5;
    int *d5=malloc(1 * sizeof(int));
    *d5=60;
    viewOList(OL);
    OListInsert(OL,k1,d1);
    viewOList(OL);
    OListInsert(OL,k2,d2);
    viewOList(OL);
    OListInsert(OL,k3,d3);
    viewOList(OL);
    freeOList(OL,0,0);
    OList *OL2 = newOList(compareInt,viewInt,viewInt,freeInt,freeInt);
    printf("J'ai free\n");
    viewOList(OL2);
    OListInsert(OL2,k4,d4);
    viewOList(OL2);
    OListInsert(OL2,k5,d5);
    OListInsert(OL2,k1,d1);
    OListInsert(OL2,k2,d2);
    OListInsert(OL2,k3,d3);
    viewOList(OL2);
    List *nL = OListToList(OL2);
    viewList(nL);
    freeOList(OL2,1,1);
    OList *OL3 = newOList(compareInt,viewInt,viewInt,freeInt,freeInt);
    OListInsert(OL3,k3,d3);
    viewOList(OL2);

}
