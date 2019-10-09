#pragma once



typedef struct {
    unsigned int count;
    long goal;
}COUNTER,*P_COUNTER;

int counterInit(P_COUNTER target);

long getCountGoal(const P_COUNTER target);

int countUpStart(P_COUNTER target, long goal);

int countDownStart(P_COUNTER target , long goal);

int countUp(P_COUNTER target);

int countDown(P_COUNTER target);

bool countEnd(const P_COUNTER target);

bool countZero(const P_COUNTER target);
