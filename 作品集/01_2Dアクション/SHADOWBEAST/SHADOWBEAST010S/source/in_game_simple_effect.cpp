
#include "in_game_simple_effect.h"
#include "chara__manager.h"
#include "anima.h"
#include "game_state.h"

int updateSimpleEffect(pCharacter efc){
	Anima *anim = &efc->anim;
	anim->time = getGameFrame();
	int index = animaGetValueStep(anim);
	int handle = imageMgrGetHandle(efc->imageFileId, index);
	charaSetImageHandle(efc, handle);
	if(animaGetCount(anim) >= 1){
		charaClear(efc);
		return -1;
	}
	if(efc->draw.alpha == 0){
		efc->draw.alpha = 1;
	} else if(efc->draw.alpha == 1){
		efc->draw.alpha = 255;
	}
	return 0;
}

int setSimpleEffect(SimpleEffectType type, double x, double y, Direction dir, ImageFileId fileId){
	int found = charaFindNonActive(simpleEffects,IN_GAME_SIMPLE_EFFECT_NUM);
	if(found < 0){
		return -1;
	}
	pCharacter efc = &simpleEffects[found];
	efc->isActive = true;
	efc->imageFileId = fileId;
	efc->pos = {x,y};
	efc->beforePos = efc->beforePos;
	switch(type){
		case SIMPLE_EFFECT_NONE:
			break;
		case SIMPLE_EFFECT_BLOOD:
		{
			if(dir == RIGHT){
				charaFlipX(efc, true);
				efc->pos.x += -60;
				efc->pos.y += -120;
			} else{
				efc->pos.x += 0;
				efc->pos.y += -120;
			}
			charaSetDispSize(efc, DISP_BLOOD_EFFECT);
			charaFuncSet(efc, updateSimpleEffect);
			AnimaData data = {0};
			createAnimaData(&data, IMAGE_BLOOD_ANIM);
			animaSetup(&efc->anim, getGameFrame(), &data);
			efc->draw.alpha = 0;
		}
			break;
		case SIMPLE_EFFECT_SMOKE:
			break;
		case SIMPLE_EFFECT_GOBLIN_DEAD:
			break;
		default:
			break;
	}
	return 0;
}
