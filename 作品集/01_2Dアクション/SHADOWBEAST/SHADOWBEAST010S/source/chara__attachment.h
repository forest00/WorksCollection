#pragma once


#include "flag.h"
#include "counter.h"

typedef struct{
	FLAG can;
    FLAG setup;
    COUNTER setupCounter;
    FLAG doing;
    COUNTER doingCounter;
    FLAG after;
    COUNTER afterCounter;
    FLAG success;
    COUNTER successCounter;
}ACTION,*P_ACTION;


typedef struct{
	double vx, vy;
	double grav;
    double walkSpeed;
    double jumpPow;
    ACTION attack;
    ACTION guard;
    ACTION jump;
	int idle[6];
	int walk[6];
}PLAYER1_ATT;


typedef struct{
    ACTION attack;
}GOBLIN_ATT;


typedef struct{
    P_FLAG instructor;
    void(*switching)(void);
}ATTACKER_NUM;


typedef struct{
    
}PARTICLE_ATT;



typedef union{
    PLAYER1_ATT pl;
    GOBLIN_ATT gob;
    PARTICLE_ATT ptcl;
}CHARA_ATT;

////////////////////////////////‚±‚±‚æ‚è‰º‚Í‚¢‚¶‚ç‚È‚¢

int actionInit(P_ACTION act);

int actionBulkInit(ACTION _array[] , int num);

int actionStart(P_ACTION act, long setup, long doing, long after, long success);

int actionMove(P_ACTION act);

