#pragma once


int loadPlayer2DataFromFile();

int setPlayer2(int x, int y);

int player2Blow(double vx, double vy);

int player2HitDamage(int damage);

int player2HealHp(int recoveryAmount);

int getPlayer2X();

int getPlayer2FactMove(double *moveX, double *moveY);

bool isPlayer2Resurrection();

bool isPlayer2Dead();

int player2Revive();
