

#include "counter.h"



int counterInit(P_COUNTER target){
    target->count = 0;
    target->goal = 0;
    return 0;
}

long getCountGoal(const P_COUNTER target){
    return target->goal;
}

int countUpStart(P_COUNTER target , long goal){
    target->count = 0;
    target->goal = goal;
    return 0;
}

int countDownStart(P_COUNTER target, long goal){
    target->count = goal;
    target->goal = goal;
    return 0;
}

int countUp(P_COUNTER target){
    target->count++;
    return (int)target->count;
}

int countDown(P_COUNTER target){
    target->count--;
    return (int)target->count;
}

bool countEnd(const P_COUNTER target){
    return target->count == target->goal;
}

bool countZero(const P_COUNTER target){
    return target->count == 0;
}
