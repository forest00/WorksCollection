#pragma once

#include "..\..\common_def\public\common_def.h"


//ワールド変換を行う
extern void world_transformation(matrix4x4 *world, const vector3_t *position, const vector3_t *angle, const vector3_t *scale);


