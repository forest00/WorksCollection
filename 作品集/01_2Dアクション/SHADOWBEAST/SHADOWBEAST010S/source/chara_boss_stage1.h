#pragma once

#include "chara__common.h"

int boss1Set(double x, double y);
int boss1Blow(pCharacter boss, double vx, double vy);
int boss1HitDamage(pCharacter boss, int damage);

int getBoss1FileIdForEffect(ImageFileId *id, int *animIndex, bool *flipFlag);
