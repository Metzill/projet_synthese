#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "utilities.h"
#include "list.h"
#include "bstree.h"

/*********************************************************************
 * Arbre Binaire de Recherche (non-équilibré).
 *********************************************************************/

/**
 * @brief
 * Construire et initialiser un nouveau nœud d'un arbre binaire de recherche (non-équilibré).
 * Renvoie le nouveau nœud créé.
 */
static BSTNode* newBSTNode(void* key, void* data) {
	BSTNode *new = (BSTNode  *) malloc(1 * sizeof(BSTNode));
	new->key=key;
	new->data=data;
	new->bfactor=0;
	new->left=NULL;
	new->right=NULL;

	return new;
}

BSTree * newBSTree(int (*preceed)(const void*, const void*),
					void (*viewKey)(const void*), void (*viewData)(const void*),
					void (*freeKey)(void*), void (*freeData)(void*)) {
	BSTree *new = (BSTree  *) malloc(1 * sizeof(BSTree));
	new->root=NULL;
	new->numelm=0;
	new->balanced=0;
	new->preceed=preceed;
	new->viewKey=viewKey;
	new->freeKey=freeKey;
	new->viewData=viewData;
	new->freeData=freeData;

	return new;
}

/**
 * @brief
 * Insérer un nouveau nœud de clé key et donnée data
 * au sous-arbre binaire de recherche (non-équilibré) raciné au nœud curr.
 * Les clés sont comparées en utilisant la fonction preceed.
 * NB : fonction récursive.
 */
static BSTNode* insertBSTNode(BSTNode* curr, void* key, void* data, int (*preceed)(const void*, const void*)) {
	if(curr==NULL)
		return newBSTNode(key,data);
	if(preceed(key,curr->viewKey)){
		curr->left=insertBSTNode(curr->left,key,data,preceed);
		return curr;
	}else
		curr->right=insertBSTNode(curr->right,key,data,preceed);
		return curr;
 }

}

/**
 * NB : Utiliser la fonction récursive insertBSTNode.
 */
void BSTreeInsert(BSTree* T, void* key, void* data) {

	insertBSTNode(T->root,key,data,T->preceed);

}

/*********************************************************************
 * Arbre Binaire de Recherche Équilibré
 *********************************************************************/

/**
 * @brief
 * Construire et initialiser un nouveau nœud d'un arbre binaire de recherche équilibré (EBST).
 * Renvoie le nouveau nœud créé.
 * N'oubliez pas à initialiser le facteur d'équilibre.
 */
static BSTNode* newEBSTNode(void* key, void* data) {
	BSTNode *new = (BSTNode  *) malloc(1 * sizeof(BSTNode));
	new->key=key;
	new->data=data;
	new->bfactor=0;
	new->left=NULL;
	new->right=NULL;
	return new;
}

BSTree * newEBSTree(int (*preceed)(const void*, const void*),
					void (*viewKey)(const void*), void (*viewData)(const void*),
					void (*freeKey)(void*), void (*freeData)(void*)) {

	BSTree *new = (BSTree  *) malloc(1 * sizeof(BSTree));
	new->root=NULL;
	new->numelm=0;
	new->balanced=1;
	new->preceed=preceed;
	new->viewKey=viewKey;
	new->viewData=viewData;
	new->freeKey=freeKey;
	new->freeData=freeData;

	return new;
}

/**
 * @brief
 * Effectuer une rotation gauche autour du nœud y.
 * N'oubliez pas à mettre à jour les facteurs d'équilibre (bfactor) des nœuds modifiés.
 * Il y a 4 cas à considérer :
 * (+) bfactor(y)=-2 et bfactor(y->right)=-1
 * (+) bfactor(y)=-1 et bfactor(y->right)=1
 * (+) bfactor(y)=-1 et bfactor(y->right)=-1
 * (+) bfactor(y)=-1 et bfactor(y->right)=0
 * (+) bfactor(y)=-2 et bfactor(y->right)=-2
 * Assurez vous que le nœud y ainsi que son fils droit existent.
 */
static BSTNode* rotateLeft(BSTNode* y) {
	assert(y);
	assert(y->right);
	/* A FAIRE */

	int by=y->bfactor;
	int byright=y->right->bfactor;
	if(by==-2 && byright==-1){
		BSTNode* rleft=y->right->left;
		y->right->left=y;
		y->right->bfactor=0;
		y->right=rleft;
		y->bfactor=0;
	}
	else if(by==-1 && byright==1){
	  rotateRight(y->right);
		BSTNode* rleft=y->right->left;
		y->right->left=y;
		y->right->bfactor=1;
		y->right=rleft;
		y->bfactor=1;
	}
	else if(by==-1 && byright==-1){
		BSTNode* rleft=y->right->left;
		y->right->left=y;
		y->right->bfactor=1;
		y->right=rleft;
		y->bfactor=1;
	}
	else if(by==-1 && byright==0){
		rotateRight(y->right);
		BSTNode* rleft=y->right->left;
		y->right->left=y;
		y->right->bfactor=0;
		y->right=rleft;
		y->bfactor=0;
	}else if(by==-2 && byright==-2){
		BSTNode* rleft=y->right->left;
		y->right->left=y;
		y->right->bfactor=0;
		y->right=rleft;
		y->bfactor=1;
	}

}

/**
 * @brief
 * Effectuer une rotation droite autour du nœud x.
 * N'oubliez pas à mettre à jour les facteurs d'équilibre (bfactor) des nœuds modifiés.
 * Il y a 4 cas à considérer :
 * (+) bfactor(x)=2 et bfactor(x->left)=1
 * (+) bfactor(x)=1 et bfactor(x->left)=1
 * (+) bfactor(x)=1 et bfactor(x->left)=-1
 * (+) bfactor(x)=1 et bfactor(x->left)=0
 * (+) bfactor(x)=2 et bfactor(x->right)=2
 * Assurez vous que le nœud x ainsi que son fils gauche existent.
 */
static BSTNode* rotateRight(BSTNode* x) {
	assert(x);
	assert(x->left);
	/* A FAIRE */
	int bx=x->bfactor;
	int bxright=x->left->bfactor;
	if(by==2 && byright==1){
		BSTNode* lright=y->left->right;
		y->left->right=y;
		y->left->bfactor=0;
		y->left=lright;
		y->bfactor=0;
	}
	else if(by==1 && byright==1){
		BSTNode* lright=y->left->right;
		y->left->right=y;
		y->left->bfactor=-1;
		y->left=lright;
		y->bfactor=-1;
	}
	else if(by==1 && byright==-1){
		rotateLeft(y->left);
		BSTNode* lright=y->left->right;
		y->left->right=y;
		y->left->bfactor=-1;
		y->left=lright;
		y->bfactor=-1;
	}
	else if(by==1 && byright==0){
		rotateLeft(y->left);
		BSTNode* lright=y->left->right;
		y->left->right=y;
		y->left->bfactor=0;
		y->left=lright;
		y->bfactor=0;
	}else if(by==2 && byright==2){
		BSTNode* lright=y->left->right;
		y->left->right=y;
		y->left->bfactor=0;
		y->left=lright;
		y->bfactor=0;
	}
}

/**
 * @brief
 * Insérer un nouveau nœud de clé key et donnée data
 * au sous-arbre binaire de recherche équilibré raciné au nœud curr.
 * Les clés sont comparées en utilisant la fonction preceed.
 * N'oubliez pas à mettre à jour la facteur d'équilibre du nœud curr.
 * N'oubliez pas à faire les rotations nécessaires (4 cas à considérer).
 * NB : fonction récursive.
 */
static BSTNode* insertEBSTNode(BSTNode* curr, void* key, void* data, int (*preceed)(const void*, const void*)) {
	if(curr==NULL)
		return newBSTNode(key,data);
	if(preceed(key,curr->viewKey)){
		curr->bfactor+=1;
		curr->left=insertBSTNode(curr->left,key,data,preceed);
		if(curr->bfactor>1)
			rotateRight(curr);
		return curr;
	}else{
		curr->bfactor-=1;
		curr->right=insertBSTNode(curr->right,key,data,preceed);
		if(curr->bfactor<-1)
			rotateLeft(curr);
		return curr;
 }
}

/**
 * NB : Utiliser la fonction récursive insertEBSTNode.
 */
void EBSTreeInsert(BSTree* T, void* key, void* data) {
		insertEBSTNode(T->root,key,data,T->preceed);
}

/*********************************************************************
 * Fonctions communes pour :
 * Arbre Binaire de Recherche (non-équilibré), et
 * Arbre Binaire de Recherche Équilibré.
 *********************************************************************/

/**
 * @brief
 * Libérer récursivement le sous-arbre raciné au nœud curr.
 * Dans le cas où le pointeur de fonction freeKey (resp. freeData) n'est pas NULL,
 * la mémoire de la clé (resp. de la donnée) du nœud actuel est aussi libérée.
 * NB : procédure récursive.
 */
static void freeBSTNode(BSTNode* curr, void (*freeKey)(void*), void (*freeData)(void*)) {
	if(curr!=NULL){
		freeBSTNode(curr->left,freeKey,freeData);
		freeBSTNode(curr->right,freeKey,freeData);
		if(freeKey!=NULL)
			freeKey(curr->key);
		if(freeData!=NULL)
		 freeData(curr->data);
		free(curr);
}

}

/**
 * NB : Utiliser la procédure récursive freeBSTNode.
 * Vous devez utiliser les bons paramètres freeKey et freeData
 * par rapport aux valeurs deleteKey et deleteData.
 */
void freeBSTree(BSTree* T, int deleteKey, int deleteData) {
	assert(deleteKey == 0 || deleteKey == 1);
	assert(deleteData == 0 || deleteData == 1);

	if(deleteKey==1)
		void (*freeKey)(void*)=T->freeKey;
	if(deleteData==1)
		void (*freeData)(void*)=T->freeData;

	freeBSTNode(T->root,T->freeKey,T->freeData);
	free(T);
}

/**
 * @brief
 * Afficher récursivement le sous-arbre raciné au nœud curr
 * en utilisant un ordre infixe.
 * NB : procédure récursive.
 */
static void inorderView(BSTNode *curr, void (*viewKey)(const void*), void (*viewData)(const void*)) {
	if (curr!=NULL){
	 viewData(curr->data);
	 viewKey(curr->key);
	 inorderView(curr->left,viewkey,viewData);
	 inorderView(curr->right,viewkey,viewData);
	}
}

/**
 * NB : Utiliser la procédure récursive inorderView.
 */
void viewBSTree(const BSTree* T) {
	inorderView(T->root,T->viewKey,T->viewData);
}

/**
 * @brief
 * Transformer récursivement le sous-arbre raciné au nœud curr
 * à une liste doublement chaînée.
 * L'argument list est en entrée/sortie.
 * La liste va contenir les éléments de l'arbre dans l'ordre infixe.
 * NB : procédure récursive.
 */
static void treetolist(BSTNode* curr, List* list) {

	if (curr!=NULL){
	 listInsertLast(list,curr);
	 treetolist(curr->left,list);
	 treetolist(curr->right,list);
	}

}

/**
 * NB : Utiliser la procédure récursive treetolist.
 */
List* BSTreeToList(const BSTree* T) {
	List* list=newList(T->viewData,t->freeData);
	return treetolist(T->root,list);
}

BSTNode* BSTMin(BSTNode* node) {
	assert(node != NULL);

	BSTNode* tempoG=BSTMin(noded->left);
	BSTNode* tempoD=BSTMin(noded->right);
	BSTNode* petit=node;

	if(petit->key>tempoG->key)
		petit=tempoG;
	if(petit->key>tempoD->key)
		petit=tempoD;

	return petit;

}

BSTNode* BSTMax(BSTNode* node) {
	assert(node != NULL);

	BSTNode* tempoG=BSTMin(noded->left);
	BSTNode* tempoD=BSTMin(noded->right);
	BSTNode* grand=node;

	if(grand->key<tempoG->key)
		grand=tempoG;
	if(grand->key<tempoD->key)
		grand=tempoD;

	return grand;
}

/**
 * @brief
 * Chercher récursivement dans le sous-arbre raciné au nœud curr
 * et renvoyer le noeud qui contient la clé qui précède la clé key (prédécesseur).
 * Le pointeur de fonction preceed donne l'ordre entre deux clés.
 * NB : fonction récursive.
 */
static BSTNode* predecessor(BSTNode* curr, void* key, int (*preceed)(const void*, const void*)) {
	assert(curr != NULL);

	BSTNode* tempoG=predecessor(curr->left,key,preceed);
	BSTNode* tempoD=predecessor(curr->right,key,preceed);

	if(preceed(curr->key,key)==1)
		return curr;
	else if (tempoG!=NULL)
		return tempoG;
	else if (tempoD!=NULL)
		return tempoD;
	else
		return NULL;


}

/**
 * NB : Utiliser la fonction récursive predecessor.
 */
BSTNode * findPredecessor(const BSTree * T, const BSTNode* node) {
	assert(T->root != NULL);
	assert(node != NULL);

	return predecessor(T->root,node->key,T->preceed);
}

/**
 * @brief
 * Chercher récursivement dans le sous-arbre raciné au nœud curr
 * et renvoyer le noeud qui contient la clé qui succède la clé key (successeur).
 * Le pointeur de fonction preceed donne l'ordre entre deux clés.
 * NB : fonction récursive.
 */
static BSTNode* successor(BSTNode* curr, void* key, int (*preceed)(const void*, const void*)) {
	assert(curr != NULL);
	/* A FAIRE */
	BSTNode* tempoG=successor(curr->left,key,preceed);
	BSTNode* tempoD=successor(curr->right,key,preceed);

	if(preceed(curr->key,key)==0)
		return curr;
	else if (tempoG!=NULL)
		return tempoG;
	else if (tempoD!=NULL)
		return tempoD;
	else
		return NULL;
}

/**
 * NB : Utiliser la fonction récursive successor.
 */
BSTNode * findSuccessor(const BSTree * T, const BSTNode* node) {
	assert(T->root != NULL);
	assert(node != NULL);

	return predecessor(T->root,node->key,T->preceed);
}
