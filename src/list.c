#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include "utilities.h"

/**
 * @brief
 * Construire et initialiser un nouveau nœud d'une liste doublement chaînée.
 * Renvoie le nouveau nœud créé.
 */
static LNode * newLNode(void * data) {
	LNode *new = (LNode  *) malloc(1 * sizeof(LNode));
	new->data=data;
	new->succ=NULL;
	new->pred=NULL;
	return new;
}

List * newList(void (*viewData)(const void*), void (*freeData)(void*)) {
	List *new =(List*) malloc(1*sizeof( List));
	new->numelm=0;
	new->viewData=viewData;
	new->freeData=freeData;
	new->head=NULL;
	new->tail=NULL;
	return new;
}

void freeList(List * L, int deleteData) {
	assert(deleteData == 0 || deleteData == 1);
	LNode *cur=L->head;
  while(cur!=NULL){
		if(deleteData==1)
			L->freeData(cur->data);
		LNode *aux=cur;
		cur=cur->succ;
		free(aux);
	}
	free(L);
}

void viewList(const List * L) {
	if(L!=NULL){
		LNode *Tete = L->head;
		while(Tete!=NULL){
			L->viewData(Tete->data);
			printf("\n");
			Tete=Tete->succ;
		}
		free(Tete);
	}

}

void fonctionTest(){
	List *fils=newList(*viewInt,*freeInt);
	int *p=malloc(1 * sizeof(int));
	*p=9;
	listInsertFirst(fils,p);
	viewList(fils);
	printf("\n");

	int *o=malloc(1 * sizeof(int));
	*o=7;
	listInsertLast(fils,o);
	viewList(fils);
	printf("\n");

	int *m=malloc(1 * sizeof(int));
	*m=13;
	LNode *test=fils->head;
	listInsertAfter(fils,m,test);
	viewList(fils);
}

void listInsertFirst(List * L, void * data) {
	LNode *first=newLNode(data);
	first->succ=L->head;
	if(L->head!=NULL)
		L->head->pred=first;

  L->head=first;

	if(L->tail==NULL)
		L->tail=first;

  L->numelm++;
}

void listInsertLast(List * L, void * data) {
	LNode *last=newLNode(data);
	last->pred=L->tail;

	if(L->tail!=NULL)
		L->tail->succ=last;
	L->tail=last;
	
	if(L->head==NULL)
		L->head=last;

	L->numelm++;
}

void listInsertAfter(List * L, void * data, LNode * ptrelm) {
	LNode *new=newLNode(data);
	new->succ=ptrelm->succ;
	ptrelm->succ=new;
	new->pred=ptrelm;
	if(ptrelm==L->tail)
		L->tail=new;
	L->numelm++;
}

LNode* listRemoveFirst(List * L) {
	assert(L->head);
	assert(L->numelm!=0);
	LNode *adieu=L->head;
	L->head=adieu->succ;
	if(L->tail==adieu)
		L->tail=NULL;
	L->freeData(adieu->data);
	free(adieu);
	L->numelm--;
}

LNode* listRemoveLast(List * L) {
	assert(L->head);
	assert(L->numelm==0);
	LNode *adieu=L->tail;
	L->tail=adieu->pred;
	if(L->head==adieu)
		L->head=NULL;
	L->freeData(adieu->data);
	free(adieu);
	L->numelm--;
}

LNode* listRemoveNode(List * L, LNode * node) {
	assert(L->numelm==0);
	node->pred->succ=node->succ;
	node->succ->pred=node->pred;

	if(L->tail==node)
		L->tail=node->pred;
	if(L->head==node);
	  L->head=node->succ;

	L->freeData(node->data);
	free(node);
	L->numelm--;
}
