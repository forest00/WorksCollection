

#include "game_state.h"
#include "chara__manager.h"

static WaterAtt waterAttBase = {0.0,0.0};

int updateBossWater(pCharacter wt){
	WaterAtt *att = &wt->att.water;
	att;
	Anima *anim = &wt->anim;
	anim->time = getGameFrame();

	int index = animaGetValueStep(anim);
	int handle = imageMgrGetHandle(wt->imageFileId, index);
	charaSetImageHandle(wt, handle);
	return 0;
}


int setBossWater(double x, double y, double vx, double vy){
	vx, vy;
	int nonActive = charaFindNonActive(thunder, THUNDER_NUM);
	if(nonActive < 0){
		return -1;
	}
	pCharacter actor = &thunder[nonActive];
	actor->isActive = true;
	actor->pos = {x,y};
	actor->imageFileId = IMAGE_NONE;
	charaSetDispSize(actor, HIT_WATER);
	charaFuncSet(actor, updateBossWater);
	AnimaData animData = {0};
	createAnimaData(&animData, actor->imageFileId);
	animaSetup(&actor->anim, getGameFrame(), &animData);
	WaterAtt *att = &actor->att.water;
	*att = waterAttBase;
	actor->update(actor);
	return 0;
}