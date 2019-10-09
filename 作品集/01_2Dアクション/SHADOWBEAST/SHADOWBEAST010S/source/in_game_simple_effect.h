#pragma once

#include "image_manager.h"
#include "common.h"

enum SimpleEffectType{
	SIMPLE_EFFECT_NONE,
	SIMPLE_EFFECT_BLOOD,
	SIMPLE_EFFECT_SMOKE,
	SIMPLE_EFFECT_GOBLIN_DEAD,
};

int setSimpleEffect(SimpleEffectType, double x, double y, Direction, ImageFileId);
