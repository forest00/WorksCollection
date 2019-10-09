
#include "game_state.h"
#include "chara_player1.h"
#include "chara__manager.h"
#include "chara__hit_rect.h"
#include "chara_attacker.h"
#include "image_manager.h"

#define AHEAD_ANIM_LIMIT 30
#define STOP_START_FRAME 8
#define STOP_END_FRAME 20
#define AHEAD_GO_SPEED (400.0 / STOP_START_FRAME)
#define AHEAD_BACK_SPEED (400.0 / (AHEAD_ANIM_LIMIT - STOP_END_FRAME))

static unsigned int getFrame(){
	return getGameFrame();
}


static int update1pChain(pCharacter chain){
	//プレイヤー速度取得
	double plMoveX = 0.0, plMoveY = 0.0;
	getPlayer1FactMove(&plMoveX, &plMoveY);
	Player1SubAtt *att = &chain->att.pl1Sub;
	Anima *anim = &chain->anim;
	//鎖部分更新↓
	anim->time = getFrame();
	int animFrame = animaGetFrame(anim);
	int animLimit = anim->limit;
	{
		att->isMiss = animFrame >= animLimit / 2;
		att->isSuccess = false;
		charaFlipX(chain, chain->dir == LEFT);
		if(animFrame >= animLimit){
			chain->isActive = false;
			clearAttacker(att->aheadIndex);
		}
	}
	//位置更新↓
	chain->pos.x += plMoveX;
	chain->pos.y += plMoveY;
	//位置更新↑
	//鎖部分更新↑

	//先端部分更新↓
	{
		int index = att->aheadIndex;
		pCharacter hitting = enemies - 1;
		if(att->aheadTarget == nullptr){
			do{
				hitting = checkAttackerHitCharacters(index, hitting + 1, ENEMY_NUM);
				if(hitting != nullptr){
					XYd _pos = {0};
					if(animFrame < STOP_END_FRAME){
						int __elapsed = anim->time - anim->started;
						int __w = anim->duration - __elapsed - __elapsed;
						anim->started -= __w;
						animFrame = animaGetFrame(anim);
						att->aheadTarget = (void*)hitting;
					}
				} else{
					break;
				}
			} while(true);
		}


		//位置更新↓
		pXYd pos = &att->aheadPos;
		double speed = (att->aheadSpeed);
		double vx = 0;
		if(chain->dir == LEFT){
			speed *= -1;
		}
		if(animFrame >= (att->aheadStopStartFrame)){
			att->aheadInProgress = false;
		}
		if(animFrame >= (att->aheadStopEndFrame)){
			att->aheadRegression = true;
			att->aheadSpeed = AHEAD_BACK_SPEED;
		}
		if(att->aheadInProgress){
			vx = speed;
		}
		if(att->aheadRegression){
			vx = -speed;
		}
		pos->x += vx;
		if(att->aheadTarget != nullptr){
			pCharacter _aheadTarget = (pCharacter)(att->aheadTarget);
			XYd _pos = {0,0};
			getCharaOffSet(_aheadTarget, &_pos.x, &_pos.y);
			_pos.x += vx;
			charaOffSet(_aheadTarget, _pos.x, _pos.y);
		}
		pos->x += plMoveX;
		pos->y += plMoveY;
		attackerOffset(index, pos->x, pos->y);
		//位置更新↑
		att->aheadPos = *pos;
	}
	//先端部分更新↑

	//アニメーション更新↓
	{
		int index = animaGetValueStep(anim);
		int handle = imageMgrGetHandle(chain->imageFileId, index);
		charaSetImageHandle(chain, handle);
	}
	//アニメーション更新↑


	return 0;
}




int player1SubAtkSet(double x, double y, Direction dir){
	int nonActive = charaFindNonActive(player1Sub, PLAYER1_SUB_NUM);
	if(nonActive < 0){
		return -1;
	}
	ImageFileId fileId = IMAGE_CHAIN_1P;
	pCharacter chain = &player1Sub[nonActive];
	chain->isActive = true;
	charaOffSet(chain, x, y);
	charaSetDispSize(chain, DISP_CHAIN);
	charaColSet(chain, HIT_CHAIN);
	chain->dir = dir;
	chain->imageFileId = fileId;
	Player1SubAtt *att = &chain->att.pl1Sub;
	att->isMiss = false;
	att->isSuccess = false;
	double xResist = 0;
	if(dir == LEFT){
		xResist = 1.0;
	}
	att->aheadPos.x = x + (chain->hit.relative.x) + (chain->hit.size.x) * xResist;
	att->aheadPos.y = y + (chain->hit.relative.y) + (chain->hit.size.y) / 2;
	att->aheadIndex = setAttacker(att->aheadPos.x, att->aheadPos.y, HIT_CHAIN_AHEAD);
	att->aheadInProgress = true;
	att->aheadRegression = false;
	att->aheadSpeed = AHEAD_GO_SPEED;
	att->aheadStopStartFrame = STOP_START_FRAME;
	att->aheadStopEndFrame = STOP_END_FRAME;
	att->aheadTarget = nullptr;
	AnimaData anim = {0};
	createAnimaData(&anim, fileId);
	animaSetup(&(chain->anim), getFrame(), &anim);
	charaFuncSet(chain, update1pChain);
	chain->update(chain);
	return 0;
}



bool isPlayer1SubAtkActive(int index){
	if(index < 0 && index >= PLAYER1_SUB_NUM){
		return false;
	}
	return player1Sub[index].isActive;
}

bool isPlayer1SubAtkMiss(int index){
	if(index < 0 && index >= PLAYER1_SUB_NUM){
		return false;
	}
	return player1Sub[index].att.pl1Sub.isMiss;
}

bool isPlayer1SubAtkSuccess(int index){
	if(index < 0 && index >= PLAYER1_SUB_NUM){
		return false;
	}
	return player1Sub[index].att.pl1Sub.isSuccess;
}

