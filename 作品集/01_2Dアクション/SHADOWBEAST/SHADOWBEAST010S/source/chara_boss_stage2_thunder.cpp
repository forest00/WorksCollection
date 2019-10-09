
#include <math.h>
#include "game_state.h"
#include "chara_block.h"
#include "image_manager.h"
#include "chara__manager.h"
#include "chara_player1.h"
#include "chara_player2.h"
#include "in_game_effect.h"
#include "stage_data_reader.h"

static constexpr int ANIM_STEP = 5;
static constexpr int ANIM_DURATION = 9 * ANIM_STEP;
static constexpr int HIT_RELATIVE_TIME = 5 * ANIM_STEP;



int updateBossThunder(pCharacter th) {
	ThunderAtt *att = &th->att.thunder;
	Anima *anim = &th->anim;
	att->elapsed++;
	if ((att->elapsed) <= (att->delay)) {
		charaSetImageHandle(th, -1);
		//エフェクトを出す↓
		{
#if 0
			double _x = 0.0, _y = 0.0;
			_x = th->pos.x + th->hit.relative.x;
			_y = th->pos.y + th->hit.relative.y;
			int diff = att->delay - att->elapsed;
			int diffV = 200;
			if (diff <= diffV) {
				double t = (double)(diffV - diff) / (double)(diffV);
				int iMax = (int)(t * 4);
				for (int i = 0; i < iMax; i++) {
					int _x_ = (int)(_x + rand0to1() * th->hit.size.x);
					int _y_ = (int)(_y + rand0to1() * th->hit.size.y);
					inGameEffctSet(IN_GAME_EFFECT_THUNDER_SMALL, _x_, _y_, (EffectFrontOrBack)(rand() % 2));
				}
			}
#endif
			inGameEffctSet(IN_GAME_EFFECT_THUNDER_SUGGESTION, th->pos.x,th->pos.y, (EffectFrontOrBack)(rand() % 2));
		}
		//エフェクトを出す↑
		//アニメーション準備↓
		if ((att->elapsed) == (att->delay)) {
			AnimaData data = { 0 };
			createAnimaData(&data, th->imageFileId);
			data.duration = ANIM_DURATION;
			animaSetup(anim, getGameFrame(), &data);
		}
		//アニメーション準備↑
		return -1;
	}
	//アニメーション↓
	anim->time = getGameFrame();
	int index = animaGetValueStep(anim);
	int handle = imageMgrGetHandle(th->imageFileId, index);
	charaSetImageHandle(th, handle);
	//アニメーション↑
	//自分とプレイヤーの衝突↓
	if (att->elapsed >= att->delay + HIT_RELATIVE_TIME) {
		int hitIndex = -1;
		pCharacter hitting = nullptr;
		//自分と1P
		hitIndex = charaBulkHit(player1, PLAYER1_NUM, th);
		if (hitIndex >= 0) {
			hitting = &player1[hitIndex];
			player1Blow(0, 0);
			player1HitDamage(att->attack);
		}
		//自分と2P
		hitIndex = charaBulkHit(player2, PLAYER2_NUM, th);
		if (hitIndex >= 0) {
			hitting = &player2[hitIndex];
			player2Blow(0, 0);
			player2HitDamage(att->attack);
		}
	}
	//自分とプレイヤーの衝突↑
	//遅延 + 継続時間 + アニメ時間 以上経過したら消える↓
	if (att->elapsed > att->duration + att->delay) {
		th->isActive = false;
	}
	//遅延 + 継続時間 + アニメ時間 以上経過したら消える↑
	return 0;
}


int setBossThunder(double x, double y, int attack, int delay) {
	int nonActive = charaFindNonActive(thunder, THUNDER_NUM);
	if (nonActive < 0) {
		return -1;
	}
	pCharacter actor = &thunder[nonActive];
	charaInit(actor);
	actor->isActive = true;
	actor->pos = { x,y };
	actor->imageFileId = IMAGE_THUNDER_ANIM;
	charaSetDispSize(actor, DISP_THUNDER_BIG);
	charaColSet(actor, HIT_THUNDER_BIG);
	charaFuncSet(actor, updateBossThunder);
	ThunderAtt *att = &actor->att.thunder;
	att->elapsed = -1;
	att->delay = delay;
	att->atkStartTime = 46;
	att->atkEndTime = 48;
	att->duration = ANIM_DURATION;
	att->attack = attack;
	actor->update(actor);
	return 0;
}


int update2PKiller(pCharacter th) {
	ThunderAtt *att = &th->att.thunder;
	Anima *anim = &th->anim;
	att->elapsed++;
	if ((att->elapsed) <= (att->delay)) {
		charaSetImageHandle(th, -1);
		//エフェクトを出す↓
		{
			double _x = 0.0, _y = 0.0;
			_x = th->pos.x + th->hit.relative.x;
			_y = th->pos.y + th->hit.relative.y;
			int diff = att->delay - att->elapsed;
			int diffV = 200;
			if (diff <= diffV) {
				double t = (double)(diffV - diff) / (double)(diffV);
				int iMax = (int)(t * 4);
				for (int i = 0; i < iMax; i++) {
					int _x_ = (int)(_x + rand0to1() * th->hit.size.x);
					int _y_ = (int)(_y + rand0to1() * th->hit.size.y);
					inGameEffctSet(IN_GAME_EFFECT_THUNDER_SUGGESTION, _x_, _y_, (EffectFrontOrBack)(rand() % 2));
				}
			}
		}
		//エフェクトを出す↑
		//アニメーション準備↓
		if ((att->elapsed) == (att->delay)) {
			AnimaData data = { 0 };
			createAnimaData(&data, th->imageFileId);
			data.duration = ANIM_DURATION;
			animaSetup(anim, getGameFrame(), &data);
		}
		//アニメーション準備↑
		return -1;
	}
	//アニメーション↓
	anim->time = getGameFrame();
	int index = animaGetValueStep(anim);
	int handle = imageMgrGetHandle(th->imageFileId, index);
	charaSetImageHandle(th, handle);
	//アニメーション↑
	//自分とプレイヤーの衝突↓
	if (att->elapsed >= att->delay + HIT_RELATIVE_TIME) {
		int hitIndex = -1;
		pCharacter hitting = nullptr;
		//自分と2P
		hitIndex = charaBulkHit(player2, PLAYER2_NUM, th);
		if (hitIndex >= 0) {
			hitting = &player2[hitIndex];
			player2Blow(0, 0);
			player2HitDamage(att->attack);
		}
	}
	//自分とプレイヤーの衝突↑
	//遅延 + 継続時間 + アニメ時間 以上経過したら消える↓
	if (att->elapsed > att->duration + att->delay) {
		th->draw.alpha = 0;
	}
	//遅延 + 継続時間 + アニメ時間 以上経過したら消える↑
	//遅延 + 継続時間 + アニメ時間 以上経過したら消える↓
	if (att->elapsed - 80 > att->duration + att->delay) {
		th->isActive = 0;
	}
	//遅延 + 継続時間 + アニメ時間 以上経過したら消える↑
	return 0;
}

int set2PKiller() {
	int nonActive = charaFindNonActive(thunder, THUNDER_NUM);
	if (nonActive < 0) {
		return -1;
	}
	pCharacter actor = &thunder[nonActive];
	charaInit(actor);
	actor->isActive = true;
	actor->pos = { (double)getPlayer2X(),(double)GROUND_Y };
	actor->imageFileId = IMAGE_THUNDER_ANIM;
	charaSetDispSize(actor, DISP_THUNDER_BIG);
	charaColSet(actor, HIT_THUNDER_BIG);
	actor->hit.relative.x -= 3000;
	actor->hit.size.x += 6000;
	charaFuncSet(actor, update2PKiller);
	ThunderAtt *att = &actor->att.thunder;
	att->elapsed = -1;
	att->delay = 0;
	att->atkStartTime = 46;
	att->atkEndTime = 48;
	att->duration = ANIM_DURATION;
	att->attack = 999999;
	actor->update(actor);
	return 0;
}
