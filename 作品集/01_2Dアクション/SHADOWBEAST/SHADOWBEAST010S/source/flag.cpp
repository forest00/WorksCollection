
#include <stdlib.h>
#include "flag.h"

void flagInit(P_FLAG target){
    target->before = false;
    target->now = false;
}

void flagBulkInit(FLAG _array[] , int num){
    for(int i = 0; i < num; i++){
        flagInit(&_array[i]);
    }
}

void flagMove(P_FLAG target){
    target->before = target->now;
}

void flagBulkMove(FLAG _array[] , int num){
    for(int i = 0; i < num; i++){
        flagMove(&_array[i]);
    }
}

void flagSet(P_FLAG target , bool flag){
    target->now = flag;
}

bool flagOn(const P_FLAG target){
    return target->now == true;
}

bool flagOffToOn(const P_FLAG target){
    return target->now == true && target->before == false;
}

bool flagOff(const P_FLAG target){
    return target->now == false;
}

bool flagOnToOff(const P_FLAG target){
    return target->now == false && target->before == true;
}
