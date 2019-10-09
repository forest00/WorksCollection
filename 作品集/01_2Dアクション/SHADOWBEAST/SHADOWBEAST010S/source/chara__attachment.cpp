
#include "chara__attachment.h"

int actionInit(P_ACTION act){
    flagInit(&act->can);
    flagInit(&act->setup);
    counterInit(&act->setupCounter);
    flagInit(&act->doing);
    counterInit(&act->doingCounter);
    flagInit(&act->after);
    counterInit(&act->afterCounter);
    flagInit(&act->success);
    flagSet(&act->can , true);
    return 0;
}

int actionBulkInit(ACTION _array[] , int num){
    for(int i = 0; i < num; i++){
        actionInit(&_array[i]);
    }
    return 0;
}

int actionStart(P_ACTION act, long setup, long doing, long after, long success){
    countUpStart(&act->setupCounter, setup);
    countUpStart(&act->doingCounter, doing);
    countUpStart(&act->afterCounter, after);
    countUpStart(&act->successCounter, success);
    return 0;
}


int actionMove(P_ACTION act){
    flagMove(&act->can);
    flagMove(&act->setup);
    flagMove(&act->doing);
    flagMove(&act->after);
    flagMove(&act->success);
    return 0;
}





















