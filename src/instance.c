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

Task * newTask(char* id, int proctime, int reltime, int deadline, int weight) {
	assert(proctime > 0);
	assert(reltime >= 0);
	assert((deadline >= reltime + proctime));
	assert(strcmp(id,""));
	Task *new = (Task *) malloc(1 * sizeof(Task));
    new->id=id;
    new->processingTime=proctime;
    new->releaseTime=reltime;
    new->deadline=deadline;
    new->weight=weight;
    return new;
}

void freeTask(void* task) {
    assert(task!=NULL);
	free(task);
}

void viewTask(const void *task) {
    assert(task!=NULL);
    const Task *tache = task;
	printf("Tâche : %s \n",tache->id);
    printf("Durée : %d \n",tache->processingTime);
    printf("Date de libération : %d \n",tache->releaseTime);
    printf("Date limite : %d \n",tache->deadline);
    printf("Poids : %d \n",tache->weight);

}

/************************************************
 * INSTANCE
 ************************************************/

Instance readInstance(char * filename) {
    Instance newInstance = newList(viewTask,freeTask);
    int lineLength = 13;
	char line[lineLength];
	char idS[10];
	int processingTime, releaseTime, deadline, weight;
	FILE *fp;
    if((fp = fopen("filename", "rt")) == NULL) {
        fprintf(stderr, "Error while opening %s\n", "data/town.txt");
        exit(EXIT_FAILURE);
    }
    while(fgets(line,lineLength,fp) != NULL){
        sscanf(line, "%s %d %d %d %d", idS, &processingTime, &releaseTime, &deadline, &weight);
        Task *taskn = newTask(idS,processingTime,releaseTime,deadline,weight);
        listInsertFirst(newInstance,taskn);
    }
    return newInstance;

}

void viewInstance(Instance I) {
	assert(I!=NULL);
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
static int spt(const void* a, const void* b) {
	/* A FAIRE */
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
static int lpt(const void* a, const void* b) {
	/* A FAIRE */
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
static int wspt(const void* a, const void* b) {
	/* A FAIRE */
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
static int fcfs(const void* a, const void* b) {
	/* A FAIRE */
}

void reorderInstance(Instance I,  DataStructure structtype, Order order) {
	/* A FAIRE */
}