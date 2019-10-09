#pragma once

typedef struct{
    bool before;
    bool now;
}FLAG, *P_FLAG;



void flagInit(P_FLAG target);

void flagBulkInit(FLAG _array[] , int num);

void flagMove(P_FLAG target);

void flagBulkMove(FLAG _array[], int num);

void flagSet(P_FLAG target , bool flag);

bool flagOn(const P_FLAG target);

bool flagOffToOn(const P_FLAG target);

bool flagOff(const P_FLAG target);

bool flagOnToOff(const P_FLAG target);


