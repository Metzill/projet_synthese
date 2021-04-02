#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "utilities.h"
#include "list.h"
#include "bstree.h"
static BSTNode* rotateLeft(BSTNode* y);
static BSTNode* rotateRight(BSTNode* x);
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
	if(preceed(key,curr->key)){
		curr->left=insertBSTNode(curr->left,key,data,preceed);
		return curr;
	}else{
		curr->right=insertBSTNode(curr->right,key,data,preceed);
		return curr;
 }

}

/**
 * NB : Utiliser la fonction récursive insertBSTNode.
 */
void BSTreeInsert(BSTree* T, void* key, void* data) {
	T->root=insertBSTNode(T->root,key,data,T->preceed);
	T->numelm++;

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
	BSTNode* x=y->right;
	y->right=x->left;
	x->left=y;
	int by=y->bfactor;
	int byright=x->bfactor;

	if(by==-2 && byright==-1){
		y->bfactor=0;
		x->bfactor=0;
	}
	else if(by==-1 && byright==1){
		y->bfactor=0;
		x->bfactor=2;
	}
	else if(by==-1 && byright==-1){
		y->bfactor=1;
		x->bfactor=1;
	}
	else if(by==-1 && byright==0){
		y->bfactor=0;
		x->bfactor=1;
	}else if(by==-2 && byright==-2){
		y->bfactor=1;
		x->bfactor=0;
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
static BSTNode* rotateRight(BSTNode* y) {
	assert(y);
	assert(y->left);
	/* A FAIRE */
	BSTNode* x=y->left;
	y->left=x->right;
	x->right=y;
	int by=y->bfactor;
	int byleft=x->bfactor;
	if(by==2 && byleft==1){
		y->bfactor=0;
		x->bfactor=0;
	}
	else if(by==1 && byleft==1){
		y->bfactor=-1;
		x->bfactor=-1;
	}
	else if(by==1 && byleft==-1){
		y->bfactor=0;
		x->bfactor=-2;
	}
	else if(by==1 && byleft==0){
		y->bfactor=0;
		x->bfactor=-1;
	}else if(by==2 && byleft==2){
		y->bfactor=-1;
		x->bfactor=0;
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
	if(preceed(key,curr->key)){
		//old bfactor Save
		int oldBfLeft;
		if(curr->left!=NULL)
			 oldBfLeft=curr->left->bfactor;
		else
			oldBfLeft=0;

		curr->left=insertEBSTNode(curr->left,key,data,preceed);
		//test increment
		if(oldBfLeft!=curr->left->bfactor && abs(curr->left->bfactor)==1)
			curr->bfactor-=1;
	}else if(!preceed(key,curr->key)){
		//old bfactor Save
		int oldBfRight;
		if(curr->left!=NULL)
			 oldBfRight=curr->left->bfactor;
		else
			oldBfRight=0;

		curr->right=insertEBSTNode(curr->right,key,data,preceed);
		//test increment
		if(oldBfRight!=curr->left->bfactor && abs(curr->left->bfactor)==1)
			curr->bfactor+=1;
  }else{
 		error("On insère une deuxième fois cette clé");
	}

	int bfactor=curr->bfactor;
 	if (bfactor > 1 && preceed(key,curr->left->key))
        return rotateRight(curr);


  // Right Right Case
  if (bfactor < -1 && !preceed(key,curr->right->key))
      return rotateLeft(curr);

  // Left Right Case
  if (bfactor > 1 && !preceed(key,curr->left->key))
  {
      curr->left =  rotateLeft(curr->left);
      return rotateRight(curr);
  }

  // Right Left Case
  if (bfactor < -1 && preceed(key,curr->right->key))
  {
      curr->right = rotateRight(curr->right);
      return rotateLeft(curr);
  }

	return curr;
}

/**
 * NB : Utiliser la fonction récursive insertEBSTNode.
 */
void EBSTreeInsert(BSTree* T, void* key, void* data) {
		T->root=insertEBSTNode(T->root,key,data,T->preceed);
		T->numelm++;
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

	if(deleteKey==0)
        T->freeKey=NULL;

	if(deleteData==0)
        T->freeData=NULL;

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
	 inorderView(curr->left,viewKey,viewData);
	 inorderView(curr->right,viewKey,viewData);
	 viewData(curr->data);
	 viewKey(curr->key);
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
static void BSTreetolist(BSTNode* curr, List* list) {
	if (curr!=NULL){
	 listInsertLast(list,curr->data);
	 BSTreetolist(curr->left,list);
	 BSTreetolist(curr->right,list);
	}

}

/**
 * NB : Utiliser la procédure récursive treetolist.
 */
List* BSTreeToList(const BSTree* T) {
	List* list=newList(T->viewData,T->freeData);
    BSTreetolist(T->root,list);
	return list;
}

BSTNode* BSTMin(BSTNode* node) {
	assert(node != NULL);

	BSTNode* tempoG=BSTMin(node->left);
	BSTNode* tempoD=BSTMin(node->right);
	BSTNode* petit=node;

	if(petit->key>tempoG->key)
		petit=tempoG;
	if(petit->key>tempoD->key)
		petit=tempoD;

	return petit;

}

BSTNode* BSTMax(BSTNode* node) {
	assert(node != NULL);

	BSTNode* tempoG=BSTMin(node->left);
	BSTNode* tempoD=BSTMin(node->right);
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
