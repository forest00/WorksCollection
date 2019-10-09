#pragma once

#include "chara__common.h"
#include "chara__hit_rect.h"

int setAttacker(double x, double y, int hitX, int hitY, int hitW, int hitH);
int setAttacker(double x, double y, const pBox hit);
int setAttacker(double x, double y, CharaHitAreaId id);

int attackerLock(int index);

int attackerUnLock(int index);

int attackerOffset(int index, double x, double y);

int getAttackerCoord(int index, double *x, double *y);

bool checkAttackerHitRect(int index, pRect rect);

bool checkAttackerHitChara(int index, pCharacter chara);

pCharacter checkAttackerHitCharacters(int index, pCharacter _array, int num);

int clearAttacker(int index);

