#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "utilities.h"
#include "list.h"
#include "olist.h"
#include "bstree.h"
#include "instance.h"

/***********************************************
 * TASK
 ***********************************************/

Task *newTask(char *id, int proctime, int reltime, int deadline, int weight) {
    assert(proctime > 0);
    assert(reltime >= 0);
    assert((deadline >= reltime + proctime));
    assert(strcmp(id, ""));
    Task *new = (Task *) malloc(1 * sizeof(Task));
    new->id = (char *) calloc(strlen(id) + 1, sizeof(char));
    strcpy(new->id, id);
    new->processingTime = proctime;
    new->releaseTime = reltime;
    new->deadline = deadline;
    new->weight = weight;
    return new;
}

void freeTask(void *task) {
    assert(task != NULL);
    Task *tache = (Task *) task;
    free(tache->id);
    free(task);
}

void viewTask(const void *task) {
    assert(task != NULL);
    const Task *tache = (Task *) task;
    fprintf(stdout, "Tâche : %s - ", tache->id);
    fprintf(stdout, "Durée : %d - ", tache->processingTime);
    fprintf(stdout, "Date de libération : %d - ", tache->releaseTime);
    fprintf(stdout, "Date limite : %d - ", tache->deadline);
    fprintf(stdout, "Poids : %d \n", tache->weight);

}

/************************************************
 * INSTANCE
 ************************************************/

Instance readInstance(char *filename) {
    Instance newInstance = newList(viewTask, freeTask);
    int lineLength = 16;
    char line[lineLength];
    char idS[16];
    int id, processingTime, releaseTime, deadline, weight;
    FILE *fp;
    if ((fp = fopen(filename, "rt")) == NULL) {
        fprintf(stderr, "Error while opening %s\n", filename);
        exit(EXIT_FAILURE);
    }
    while (fgets(line, lineLength, fp) != NULL) {
        sscanf(line, "%d %d %d %d %d", &id, &processingTime, &releaseTime, &deadline, &weight);
        sprintf(idS, "%d", id);
        //printf("%s\n",idS );
        Task *taskn = newTask(idS, processingTime, releaseTime, deadline, weight);
        listInsertLast(newInstance, taskn);
    }
    return newInstance;

}

void viewInstance(Instance I) {
    assert(I != NULL);
    viewList(I);
}

void freeInstance(Instance I, int deleteData) {
    freeList(I, deleteData);
}

/*****************************************************************************
 * Ordonner l'instance
 *****************************************************************************/

/**
 * @brief
 * Comparer les tâches a et b par rapport à l'ordre SPT :
 * (+) Renvoie 1, si a précède b
 * (+) Renvoie 0, si b précède a
 * Ordre SPT : a précède b si
 * (+) durée de a < durée de b
 * (+) durée de a = durée de b ET date de libération de a < date de libération de b
 */
static int spt(const void *a, const void *b) {
    Task *ta = (Task *) a;
    Task *tb = (Task *) b;
    if ((ta->processingTime < tb->processingTime)
        || ((ta->processingTime == tb->processingTime) && (ta->releaseTime < tb->releaseTime))) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief
 * Comparer les tâches a et b par rapport à l'ordre LPT :
 * (+) Renvoie 1, si a précède b
 * (+) Renvoie 0, si b précède a
 * Ordre LPT : a précède b si
 * (+) durée de a > durée de b
 * (+) durée de a = durée de b ET date de libération de a < date de libération de b
 */
static int lpt(const void *a, const void *b) {
    Task *ta = (Task *) a;
    Task *tb = (Task *) b;
    if ((ta->processingTime > tb->processingTime)
        || ((ta->processingTime == tb->processingTime) && (ta->releaseTime < tb->releaseTime))) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief
 * Comparer les tâches a et b par rapport à l'ordre WSPT :
 * (+) Renvoie 1, si a précède b
 * (+) Renvoie 0, si b précède a
 * Ordre WSPT : a précède b si
 * (+) poids / durée de a > poids / durée de b
 * (+) poids / durée de a = poids / durée de b
 *     ET durée de a < durée de b
 * (+) poids / durée de a = poids / durée de b
 *     ET durée de a = durée de b
 *     ET date de libération de a < date de libération de b
 */
static int wspt(const void *a, const void *b) {
    Task *ta = (Task *) a;
    Task *tb = (Task *) b;
    int awd = ta->weight / ta->processingTime, bwd = tb->weight / tb->processingTime;
    if ((awd > bwd) || ((awd == bwd) && (ta->processingTime > tb->processingTime))
        || ((awd == bwd) && (ta->processingTime == tb->processingTime) && (ta->releaseTime < tb->releaseTime))) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief
 * Comparer les tâches a et b par rapport à l'ordre FCFS :
 * (+) Renvoie 1, si a précède b
 * (+) Renvoie 0, si b précède a
 * Ordre FCFS : a précède b si
 * (+) date de libération de a < date de libération de b
 * (+) date de libération de a = date de libération de b
 *     ET durée de a > durée de b
 */
static int fcfs(const void *a, const void *b) {
    Task *ta = (Task *) a;
    Task *tb = (Task *) b;

    if ((ta->releaseTime < tb->releaseTime)
        || (ta->releaseTime == tb->releaseTime) && (ta->processingTime > tb->processingTime)) {
        return 1;
    } else {
        return 0;
    }
}

void reorderInstance(Instance I, DataStructure structtype, Order order) {
    OList *ol;
    BSTree *bst;
    LNode *node = I->head;
    switch (structtype) {
        case OL:
            switch (order) {
                case SPT:
                    ol = newOList(*spt, I->viewData, I->viewData, I->freeData, I->freeData);
                    break;
                case LPT:
                    ol = newOList(*lpt, I->viewData, I->viewData, I->freeData, I->freeData);
                    break;
                case WSPT:
                    ol = newOList(*wspt, I->viewData, I->viewData, I->freeData, I->freeData);
                    break;
                case FCFS:
                    ol = newOList(*fcfs, I->viewData, I->viewData, I->freeData, I->freeData);
                    break;
            }
            while (node != NULL) {
                OListInsert(ol, node->data, node->data);
                node = node->succ;
            }
            I->head = OListToList(ol)->head;
            freeOList(ol,0,0);
            break;
        case BST:
            switch (order) {
                case SPT:
                    bst = newBSTree(*spt, I->viewData, I->viewData, I->freeData, I->freeData);
                    break;
                case LPT:
                    bst = newBSTree(*lpt, I->viewData, I->viewData, I->freeData, I->freeData);
                    break;
                case WSPT:
                    bst = newBSTree(*wspt, I->viewData, I->viewData, I->freeData, I->freeData);
                    break;
                case FCFS:
                    bst = newBSTree(*fcfs, I->viewData, I->viewData, I->freeData, I->freeData);
                    break;
            }
            while (node != NULL) {
                BSTreeInsert(bst, node->data, node->data);
                node = node->succ;
            }
            viewBSTree(bst);
            printf("\n");
            I->head = BSTreeToList(bst)->head;
            freeBSTree(bst, 0, 0);
            break;
        case EBST:

            switch (order) {
                case SPT:
                    bst = newEBSTree(*spt, I->viewData, I->viewData, I->freeData, I->freeData);
                    break;
                case LPT:
                    bst = newEBSTree(*lpt, I->viewData, I->viewData, I->freeData, I->freeData);
                    break;
                case WSPT:
                    bst = newEBSTree(*wspt, I->viewData, I->viewData, I->freeData, I->freeData);
                    break;
                case FCFS:
                    bst = newEBSTree(*fcfs, I->viewData, I->viewData, I->freeData, I->freeData);
                    break;
            }
            while (node != NULL) {
                EBSTreeInsert(bst, node->data, node->data);
                node = node->succ;
            }
            I->head = BSTreeToList(bst)->head;
            freeBSTree(bst, 0, 0);
            break;
        default:
            break;
    }

}
