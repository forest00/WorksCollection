#pragma once


int loadPlayer1DataFromFile();

int setPlayer1(int x, int y);

int player1Blow(double vx, double vy);

int player1HitDamage(int damage);

int player1HealHp(int recoveryAmount);

int getPlayer1X();

int getPlayer1FactMove(double *moveX, double *moveY);

bool isPlayer1Dead();

int player1Revive();

