#include "application_MyClass.h"
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


JNIEXPORT void JNICALL Java_application_MyClass_getSchedule
  (JNIEnv * env, jobject obj, jstring inFile, jstring outFile, jint structType, jint order, jint backfilling){
    char *inFileNamechar = (char*)(*env)->GetStringUTFChars(env, inFile, NULL);
    char *outFileNamechar = (char*)(*env)->GetStringUTFChars(env, outFile, NULL);

    Instance I = readInstance(inFileNamechar);
    Order ordre;
    DataStructure typeStruct;
    switch(order){
        case 1:ordre=SPT;break;
        case 2:ordre=LPT;break;
        case 3:ordre=WSPT;break;
        case 4:ordre=FCFS;break;
        default:ordre=SPT;break;
    }

   switch(structType){
        case 1:typeStruct=OL;break;
        case 2:typeStruct=BST;break;
        case 3:typeStruct=EBST;break;
        default:typeStruct=OL;break;
    }

    reorderInstance(I, typeStruct, ordre);
    Schedule *SEBST = newSchedule(typeStruct, backfilling);
    computeSchedule(SEBST, I);
    //viewSchedule(SEBST);
    saveSchedule(SEBST, outFileNamechar);
//    printf("Makespan=%ld\n", makespan(SEBST));
//    printf("SumWjCj=%ld\n", SumWjCj(SEBST));
//    printf("SumWjFj=%ld\n", SumWjFj(SEBST));
//    printf("SumWjTj=%ld\n", SumWjTj(SEBST));
    freeSchedule(SEBST);


  }
