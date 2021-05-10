#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "utilities.h"
#include "list.h"
#include "olist.h"
#include "bstree.h"
#include "instance.h"
#include "schedule.h"

int main(int argv, char *argc[]) {
 Instance I = readInstance("./data/exemple");
 reorderInstance(I, EBST, LPT);
 //viewInstance(I);
    // int data1=30;
    // int* ptrData1=&data1;
    //
    // int data2=15;
    // int* ptrData2=&data2;
    //
    // int data3=12;
    // int* ptrData3=&data3;
    //
    // int data4=17;
    // int* ptrData4=&data4;
    //
    // BSTree *T= newBSTree(&compareInt,&viewInt,&viewInt,&freeInt,&freeInt);
    // BSTreeInsert(T,ptrData1,ptrData1);
    //
    // BSTreeInsert(T,ptrData2,ptrData2);
    //
    // BSTreeInsert(T,ptrData3,ptrData3);
    //
    // BSTreeInsert(T,ptrData4,ptrData4);


    //viewBSTree(T);
    //T->viewData(T->root->left->right->data);
    // printf("\n");
    // T->viewData(findSuccessor(T,T->root->left)->data);

	// Schedule *SOL = newSchedule(OL, 1);
	// computeSchedule(SOL, I);
	// viewSchedule(SOL);
	// saveSchedule(SOL, "./data/output_OL");
	// printf("Makespan=%ld\n", makespan(SOL));
	// printf("SumWjCj=%ld\n", SumWjCj(SOL));
	// printf("SumWjFj=%ld\n", SumWjFj(SOL));
	// printf("SumWjTj=%ld\n", SumWjTj(SOL));
	// freeSchedule(SOL);

	// Schedule *SBST = newSchedule(BST, 1);
	// computeSchedule(SBST, I);
	// viewSchedule(SBST);
	// saveSchedule(SBST, "./data/output_BST");
	// printf("Makespan=%ld\n", makespan(SBST));
	// printf("SumWjCj=%ld\n", SumWjCj(SBST));
	// printf("SumWjFj=%ld\n", SumWjFj(SBST));
	// printf("SumWjTj=%ld\n", SumWjTj(SBST));
	// freeSchedule(SBST);

	Schedule *SEBST = newSchedule(EBST, 1);
	computeSchedule(SEBST, I);
	viewSchedule(SEBST);
	saveSchedule(SEBST, "./data/output_EBST");
	printf("Makespan=%ld\n", makespan(SEBST));
	printf("SumWjCj=%ld\n", SumWjCj(SEBST));
	printf("SumWjFj=%ld\n", SumWjFj(SEBST));
	printf("SumWjTj=%ld\n", SumWjTj(SEBST));
	freeSchedule(SEBST);

	 freeInstance(I, 1);

	return EXIT_SUCCESS;
}
