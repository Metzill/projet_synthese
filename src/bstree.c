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
	if(preceed(curr->key,key)){
		curr->right=insertBSTNode(curr->right,key,data,preceed);
		return curr;
	}else{
		curr->left=insertBSTNode(curr->left,key,data,preceed);
		return curr;
 }
}

int hauteur(BSTNode* curr){
	if(curr==NULL)
		return 0;
	else
		return max(hauteur(curr->left),hauteur(curr->right))+1;
}

/**
 * NB : Utiliser la fonction récursive insertBSTNode.
 */
void BSTreeInsert(BSTree* T, void* key, void* data) {
	if(T->numelm==0){
		T->root=newBSTNode(key,data);
		}
	else{
		BSTNode *root=insertBSTNode(T->root,key,data,T->preceed);
		T->root=root;
	}
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
	x->bfactor=hauteur(x->left)-hauteur(x->right);
	y->bfactor=hauteur(y->left)-hauteur(y->right);
	return x;

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

	BSTNode* x=y->left;
	y->left=x->right;
	x->right=y;
	x->bfactor=hauteur(x->left)-hauteur(x->right);
	y->bfactor=hauteur(y->left)-hauteur(y->right);
	return x;
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
static BSTNode* insertEBSTNode(BSTNode* node, void* key, void* data, int (*preceed)(const void*, const void*)) {
	if (node == NULL)
	        return(newEBSTNode(key,data));

	    if (preceed(key,node->key))
	        node->left  = insertEBSTNode(node->left, key,data,preceed);
	    else if (preceed(node->key,key))
	        node->right = insertEBSTNode(node->right, key,data,preceed);
	    else // Equal keys are not allowed in BST
	        return node;

	    int balance = node->bfactor;

	    // Left Left Case
	    if (balance > 1 && preceed(key,node->left->key))
	        return rotateRight(node);

	    // Right Right Case
	    if (balance < -1 && preceed(node->right->key,key))
	        return rotateLeft(node);

	    // Left Right Case
	    if (balance > 1 && preceed(node->left->key,key))
	    {
	        node->left =  rotateLeft(node->left);
	        return rotateLeft(node);
	    }

	    // Right Left Case
	    if (balance < -1 && preceed(key,node->right->key))
	    {
	        node->right = rotateRight(node->right);
	        return rotateLeft(node);
	    }

	    return node;
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
	 printf("\n");
	 printf("Data : ");
	 viewData(curr->data);
	 printf("Key : ");
	 viewKey(curr->key);
	  printf("\n");
	 inorderView(curr->right,viewKey,viewData);
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
	 treetolist(curr->left,list);
	 listInsertLast(list,curr->data);
	 treetolist(curr->right,list);
	}

}

/**
 * NB : Utiliser la procédure récursive treetolist.
 */
List* BSTreeToList(const BSTree* T) {
	List* list=newList(T->viewData,T->freeData);
    treetolist(T->root,list);
	return list;
}

BSTNode* BSTMin(BSTNode* node) {
	assert(node != NULL);
 if(node->left==NULL)
 	return node;
else
	return BSTMax(node->left);

}

BSTNode* BSTMax(BSTNode* node) {
    assert(node != NULL);
 if(node->right==NULL)
 	return node;
else
	return BSTMax(node->right);

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
	BSTNode* save=NULL;
	BSTNode* res;

	if(preceed(curr->key,key)){
         save=curr;
		 res=predecessor(curr->right,key,preceed);
	}else if(preceed(key,curr->key)){
		res=predecessor(curr->left,key,preceed);
	}
	else {
		return curr->left != NULL ? BSTMax(curr->left) : curr;
	}
	return save!=NULL && !(preceed(res->key,key) || preceed(key,res->key)) ? save : res;

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
    BSTNode* save=NULL;
    BSTNode* res;

    if(preceed(curr->key,key)){
        res=successor(curr->right,key,preceed);
    }else if(preceed(key,curr->key)){
        save=curr;
        res=successor(curr->left,key,preceed);
    }
    else {
        return curr->right != NULL ? BSTMin(curr->right) : curr;
    }
    return save!=NULL && !(preceed(res->key,key) || preceed(key,res->key)) ? save : res;

}

/**
 * NB : Utiliser la fonction récursive successor.
 */
BSTNode * findSuccessor(const BSTree * T, const BSTNode* node) {
	assert(T->root != NULL);
	assert(node != NULL);

	return successor(T->root,node->key,T->preceed);
}
