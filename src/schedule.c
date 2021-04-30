#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "utilities.h"
#include "list.h"
#include "olist.h"
#include "bstree.h"
#include "instance.h"
#include "schedule.h"

Schedule * newSchedule(DataStructure structtype, int backfilling) {
	Schedule * sched = calloc(1, sizeof(Schedule));

	sched->structtype = structtype;
	sched->backfilling = backfilling;

	switch (structtype) {
	case OL:
		sched->scheduledTasks = newOList(compareInt, viewInt, viewTask, freeInt, freeTask);
		break;
	case BST:
		sched->scheduledTasks = newBSTree(compareInt, viewInt, viewTask, freeInt, freeTask);
		break;
	case EBST:
		sched->scheduledTasks = newEBSTree(compareInt, viewInt, viewTask, freeInt, freeTask);
		break;
	default:
		error("Schedule:newSchedule : invalid data structure.");
		break;
	}

	return sched;
}

void viewSchedule(Schedule* sched) {
	switch (sched->structtype) {
	case OL:
		viewOList(sched->scheduledTasks);
		break;
	case BST:
		viewBSTree(sched->scheduledTasks);
		break;
	case EBST:
		viewBSTree(sched->scheduledTasks);
		break;
	default:
		error("Schedule:viewSchedule : invalid data structure.");
		break;
	}
}

void freeSchedule(Schedule* sched) {
	switch (sched->structtype) {
	case OL:
		freeOList(sched->scheduledTasks, 1, 0);
		break;
	case BST:
		freeBSTree(sched->scheduledTasks, 1, 0);
		break;
	case EBST:
		freeBSTree(sched->scheduledTasks, 1, 0);
		break;
	default:
		error("Schedule:freeSchedule : invalid data structure.");
		break;
	}
	free(sched);
}

void addTaskToSchedule(Schedule* sched, int startingTime, Task* task) {
	int *stime = malloc(sizeof(int));
	*stime = startingTime;
	switch (sched->structtype) {
	case OL:
		OListInsert(sched->scheduledTasks, stime, task);
		break;
	case BST:
		BSTreeInsert(sched->scheduledTasks, stime, task);
		break;
	case EBST:
		EBSTreeInsert(sched->scheduledTasks, stime, task);
		break;
	default:
		error("Schedule:addTaskToSchedule : invalid data structure.");
		break;
	}
}

/*****************************************************************************
 * Compute schedule
 *****************************************************************************/

/**
 * @brief
 * Vérifier si on peut faire du backfilling avec la tâche task dans l'ordonnancement
 * représenté par la liste ordonnée scheduledTasks.
 * Si le backfilling est possible, renvoie la date de début la plus petite.
 * Sinon (on ne peut pas faire du backfilling), renvoie -1.
 * NB : fonction itérative.
 */
static int OLFindBackfillingPosition(const OList* scheduledTasks, const Task* task) {
	assert(scheduledTasks->numelm > 0);
    OLNode *node = scheduledTasks->head;
    while (node != NULL) {
        Task *currTask = node->data;
        int* cCurr = (int*) node->key + currTask->processingTime; // completion time (date de fin)
        int cTask = task->releaseTime + task->processingTime;
        Task *nextTask;

        if(node->succ!=NULL)
            nextTask = node->succ->data;
        else
            nextTask = NULL;

        if(*cCurr <= task->releaseTime){
            if(nextTask==NULL){
                return -1;
            }else if(nextTask->releaseTime > cTask){
                return task->releaseTime;
            }
        }else if(*cCurr > task->releaseTime){
            if(nextTask==NULL){
                return -1;
            }else if(nextTask->releaseTime > cTask){
                return *cCurr;
            }
        }
        node = node->succ;
    }
    return -1;
}

/**
 * @brief
 * Trouver la date de début de la tâche task si on l'ajoute
 * dans la structure scheduledTasks de type liste ordonnée.
 * La date de début dépend de l'application du backfilling (backfilling=1) ou pas (backfilling=0).
 * NB : La fonction n'ajoute pas la tâche dans l'ordonnancement !
 */
static int OLFindStartingTime(const OList *scheduledTasks, const Task* task, int backfilling) {
    OLNode *node = scheduledTasks->head;
    Task *tailTask = scheduledTasks->tail->data;
    int* keyTail = (int*) scheduledTasks->tail->key;
    int startingTime;
    if (backfilling)
        startingTime = OLFindBackfillingPosition(scheduledTasks,task);
    if (!backfilling || startingTime == -1){
        return max((*keyTail + tailTask->processingTime), task->releaseTime);
    }else {
        return startingTime;
    }
}

/**
 * @brief
 * Vérifier si on peut faire du backfilling avec la tâche task dans l'ordonnancement
 * représenté par le sous-arbre raciné à curr de l'arbre binaire de recherche scheduledTasks.
 * Si le backfilling est possible, renvoie la date de début la plus petite.
 * Sinon (on ne peut pas faire du backfilling), renvoie -1.
 * NB : fonction récursive, l'ordre infixe est conseillé.
 */
static int BSTFindBackfillingPosition(const BSTree* scheduledTasks, const BSTNode* curr, const Task* task) {
	  assert(scheduledTasks->numelm > 0);
    if (curr != NULL) {
        Task *currTask = curr->data;
        int* cCurr = (int*) curr->key + currTask->processingTime; // completion time (date de fin)
        int cTask = task->releaseTime + task->processingTime;
        Task *Left;
				Task *Right;
				if(currTask->releaseTime > task->releaseTime && curr->left != NULL)
					return BSTFindBackfillingPosition(scheduledTasks,curr->left,task);

///Recherche de position du noeud adéquat///

				if(curr->left!=NULL)
						Left = curr->left->data;
						int* cLeft = (int*) curr->left->key + Left->processingTime;
						if(*cLeft < task->releaseTime)
							return task->releaseTime;
				else if(cTask < currTask->releaseTime)
						return task->releaseTime;

				if(curr->right!=NULL)
						Right = curr->right->data;
						if(*cCurr > task->releaseTime && Right->releaseTime < cTask)
							return *cCurr;
						if(*cCurr < task->releaseTime && Right->releaseTime < cTask)
							return task->releaseTime;

			return BSTFindBackfillingPosition(scheduledTasks,curr->right,task);
    }
    return -1;
}

/**
 * @brief
 * Trouver la date de début de la tâche task si on l'ajoute
 * dans la structure scheduledTasks de type arbre binaire de recherche.
 * La date de début dépend de l'application du backfilling (backfilling=1) ou pas (backfilling=0).
 * NB : La fonction n'ajoute pas la tâche dans l'ordonnancement !
 *      Utiliser la fonction récursive findBackfillingPosition.
 */
static int BSTFindStartingTime(const BSTree *scheduledTasks, const Task* task, int backfilling) {
	BSTNode *node = BSTMax(scheduledTasks->root);
	int* keyMax = (int*) node->key;
	Task *tailTask = node->data;
	int startingTime;
	if (backfilling)
			startingTime = BSTFindBackfillingPosition(scheduledTasks,node,task);
	if (!backfilling || startingTime == -1){
			return max((*keyMax + tailTask->processingTime), task->releaseTime);
	}else {
			return startingTime;
	}
}

int findStartingTime(const Schedule *sched, const Task* task) {
	switch (sched->structtype) {
	case OL:
		return OLFindStartingTime(sched->scheduledTasks, task, sched->backfilling);
		break;
	case BST:
		return BSTFindStartingTime(sched->scheduledTasks, task, sched->backfilling);
		break;
	case EBST:
		return BSTFindStartingTime(sched->scheduledTasks, task, sched->backfilling);
		break;
	default:
		error("Schedule:findStartingTime : invalid data structure.");
		return -1;
	}
}

void computeSchedule(Schedule *sched, const Instance I) {
	for (LNode* curr = I->head; curr; curr = curr->succ)
		addTaskToSchedule(sched, findStartingTime(sched, curr->data), curr->data);
}

/*****************************************************************************
 * Save schedule
 *****************************************************************************/

/**
 * @brief
 * Sauvegarder l'ordonnancement représenté par la liste ordonnée scheduledTasks
 * dans le ficher indiqué par le descripteur fd.
 * NB : Procédure itérative
 */
static void OLSaveSchedule(const OList* scheduledTasks, FILE* fd) {
    assert(scheduledTasks->numelm > 0);
    OLNode *node = scheduledTasks->head;
    while (node != NULL) {
        Task *currTask = node->data;
        fprintf(fd,"%s %d %d %d %d\n",currTask->id, currTask->processingTime, currTask->releaseTime, currTask->deadline, currTask->weight);
        node = node->succ;
    }
}

/**
 * @brief
 * Sauvegarder l'ordonnancement représenté par le sous-arbre raciné au nœud curr
 * dans le ficher indiqué par le descripteur fd.
 * NB : procédure récursive
 *      Pensez à un parcours infixe.
 */
static void BSTSaveSchedule(const BSTNode* curr, FILE* fd) {
	if(curr != NULL) {
        BSTSaveSchedule(curr->left,fd);
        Task *currTask = curr->data;
        fprintf(fd,"%s %d %d %d %d\n",currTask->id, currTask->processingTime, currTask->releaseTime, currTask->deadline, currTask->weight);
        BSTSaveSchedule(curr->right,fd);
	}

}

void saveSchedule(const Schedule * sched, char* filename) {
	FILE* fd;
	if ((fd = fopen(filename, "w")) == NULL)
		error("schedule:saveSchedule : Error while opening the file");

	switch (sched->structtype) {
	case OL:
		OLSaveSchedule(sched->scheduledTasks, fd);
		break;
	case BST:
		BSTSaveSchedule(((BSTree*) sched->scheduledTasks)->root, fd);
		break;
	case EBST:
		BSTSaveSchedule(((BSTree*) sched->scheduledTasks)->root, fd);
		break;
	default:
		error("Schedule:saveSchedule : invalid data structure.");
		break;
	}
	fclose(fd);
}

/*****************************************************************************
 * OBJECTIFS
 *****************************************************************************/

/////////////////////// makespan ///////////////////////

long makespan(const Schedule * sched) {
    OList *ol;
    BSTree *bst;
    BSTNode *maxNode;
    Task *lastTask;
    long* lastTaskCompletionTime;

    switch (sched->structtype) {
        case OL:
            ol = sched->scheduledTasks;
            OLNode *tailNode = ol->tail;
            lastTask = tailNode->data;
            lastTaskCompletionTime = (long*) tailNode->key + lastTask->processingTime; // completion time (date de fin);
            return *lastTaskCompletionTime;
            break;
        case BST:
            bst = sched->scheduledTasks;
            maxNode = BSTMax(bst->root);
            lastTask = maxNode->data;
            lastTaskCompletionTime = (long*) maxNode->key + lastTask->processingTime; // completion time (date de fin);
            return *lastTaskCompletionTime;
            break;
        case EBST:
            bst = sched->scheduledTasks;
            maxNode = BSTMax(bst->root);
            lastTask = maxNode->data;
            lastTaskCompletionTime = (long*) maxNode->key + lastTask->processingTime; // completion time (date de fin);
            return *lastTaskCompletionTime;
            break;
        default:
            error("Schedule:saveSchedule : invalid data structure.");
            break;
    }
}

/////////////////////// SumWjCj ///////////////////////

/**
 * @brief
 * Calculer la "somme pondérée de temps de fin" de l'ordonnancement
 * représenté par la liste ordonnée scheduledTasks.
 */
static long OLSumWjCj(const OList* scheduledTasks) {
	/* A Verifier */
	long Somme=0;
	long Cj;
	OLNode *curr=scheduledTasks->head;

	while(curr){
		Task *currTask=(Task*) curr->data;
		Cj=currTask->processingTime + (long )curr->key;
		Somme+=Cj+currTask->weight;
		curr=curr->succ;
	}
	return Somme;

}

/**
 * @brief
 * Calculer la "somme pondérée de temps de fin" de l'ordonnancement
 * représenté par l'arbre binaire de recherche raciné au nœud curr.
 * NB : fonction récursive
 */
static long BSTSumWjCj(const BSTNode* curr) {
	/* A Verifier */
		if(curr!=NULL){
			long Somme;
			Task *currTask=curr->data;
			long Cj=currTask->processingTime+(long)curr->key;
			Somme=Cj+currTask->weight;
			Somme=BSTSumWjCj(curr->left)+BSTSumWjCj(curr->right);
			return Somme;
	}
	exit(-1);
}

long SumWjCj(const Schedule* sched) {
	switch (sched->structtype) {
	case OL:
		return OLSumWjCj(sched->scheduledTasks);
		break;
	case BST:
		return BSTSumWjCj(((BSTree*) sched->scheduledTasks)->root);
		break;
	case EBST:
		return BSTSumWjCj(((BSTree*) sched->scheduledTasks)->root);
		break;
	default:
		error("Schedule:SumWjCj : invalid data structure.");
		return -1;
	}
}

/////////////////////// SumWjFj ///////////////////////

/**
 * @brief
 * Calculer la "somme pondérée de temps de réponse" de l'ordonnancement
 * représenté par la liste ordonnée scheduledTasks.
 */
static long OLSumWjFj(const OList* scheduledTasks) {
	/* A Verifier */
	long Somme=0;
	long Cj;
	long Fj;
	OLNode *curr=scheduledTasks->head;

	while(curr){
		Task *currTask=(Task*) curr->data;
		Cj=currTask->processingTime + (long) curr->key;
		Fj=Cj-currTask->releaseTime;
		Somme+=Fj+currTask->weight;
		curr=curr->succ;
	}
	return Somme;
}

/**
 * @brief
 * Calculer la "somme pondérée de temps de réponse" de l'ordonnancement
 * représenté par l'arbre binaire de recherche raciné au nœud curr.
 * NB : fonction récursive
 */
static long BSTSumWjFj(const BSTNode* curr) {
	/* A Verifier */
		if(curr!=NULL){
			long Somme;
			Task *currTask=curr->data;
			long Cj=currTask->processingTime+(long)curr->key;
			long Fj=Cj-currTask->releaseTime;
			Somme+=Fj+currTask->weight;
			Somme=BSTSumWjCj(curr->left)+BSTSumWjCj(curr->right);
			return Somme;
	}
	exit(-1);
}

long SumWjFj(const Schedule* sched) {
	switch (sched->structtype) {
	case OL:
		return OLSumWjFj(sched->scheduledTasks);
		break;
	case BST:
		return BSTSumWjFj(((BSTree*) sched->scheduledTasks)->root);
		break;
	case EBST:
		return BSTSumWjFj(((BSTree*) sched->scheduledTasks)->root);
		break;
	default:
		error("Schedule:SumWjFj : invalid data structure.");
		return -1;
	}
}

/////////////////////// SumWjTj ///////////////////////

/**
 * @brief
 * Calculer la "somme pondérée de temps de retard" de l'ordonnancement
 * représenté par la liste ordonnée scheduledTasks.
 */
static long OLSumWjTj(const OList* scheduledTasks) {
	/* A Verifier */
	long Somme=0;
	long Cj;
	long Tj;
	OLNode *curr=scheduledTasks->head;

	while(curr){
		Task *currTask=(Task*) curr->data;
		Cj=currTask->processingTime + (long )curr->key;
		Tj=max(0,Cj-currTask->deadline);
		Somme+=Tj+currTask->weight;
		curr=curr->succ;
	}
	return Somme;
}

/**
 * @brief
 * Calculer la "somme pondérée de temps de retard" de l'ordonnancement
 * représenté par l'arbre binaire de recherche raciné au nœud curr.
 * NB : fonction récursive
 */
static long BSTSumWjTj(const BSTNode* curr) {
	/* A Verifier */
		if(curr!=NULL){
			long Somme;
			Task *currTask=curr->data;
			long Cj=currTask->processingTime+(long)curr->key;
			long Tj=max(0,Cj-currTask->deadline);
			Somme+=Tj+currTask->weight;
			Somme=BSTSumWjCj(curr->left)+BSTSumWjCj(curr->right);
			return Somme;
	}
	exit(-1);
}

long SumWjTj(const Schedule* sched) {
	switch (sched->structtype) {
	case OL:
		return OLSumWjTj(sched->scheduledTasks);
		break;
	case BST:
		return BSTSumWjTj(((BSTree*) sched->scheduledTasks)->root);
		break;
	case EBST:
		return BSTSumWjTj(((BSTree*) sched->scheduledTasks)->root);
		break;
	default:
		error("Schedule:SumWjTj : invalid data structure.");
		return -1;
	}
}
