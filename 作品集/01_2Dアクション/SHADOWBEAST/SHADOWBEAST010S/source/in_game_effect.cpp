
//TODO:1P エフェクト
//TODO:2P エフェクト

#include "in_game_effect.h"
#include "game_state.h"
#include "chara__manager.h"
#include "chara_boss_stage1.h"
#include "chara_boss_stage2.h"
#include "image_manager.h"
#include "type_parameter.h"
#include "chara_player2_sub.h"
#include "chara__hit_rect.h"

//エフェクト空き領域探し関数
static pCharacter getBackEffect() {
	int found = charaFindNonActive(backEffects, IN_GAME_EFFECT_NUM);
	if (found < 0) {
		return nullptr;
	}
	return &backEffects[found];
}
static pCharacter getFrontEffect() {
	int found = charaFindNonActive(frontEffects, IN_GAME_EFFECT_NUM);
	if (found < 0) {
		return nullptr;
	}
	return &frontEffects[found];
}
//エフェクト更新関数
static int updateEasingEffect(pCharacter easingEfc) {
	EasingEffectAtt *att = &easingEfc->att.easingEfc;
	if (att->shouldAnim) {
		Anima *anim = &easingEfc->anim;
		anim->time = getGameFrame();
		{
			int index = animaGetValueStep(anim);
			int handle = imageMgrGetHandle(easingEfc->imageFileId, index);
			charaSetImageHandle(easingEfc, handle);
		}
	}
	int current = att->currentTime;
	int life = att->lifeTime;
	ParameterUpdate(&att->x, current, life);
	ParameterUpdate(&att->y, current, life);
	ParameterUpdate(&att->scale, current, life);
	ParameterUpdate(&att->angle, current, life);
	ParameterUpdate(&att->alpha, current, life);
	current++;
	if (current >= life) {
		charaClear(easingEfc);
	}
	att->currentTime = current;
	charaOffSet(easingEfc, att->offsetX + att->x.current, att->offsetY + att->y.current);
	charaScale(easingEfc, att->scale.current, att->scale.current);
	charaRot(easingEfc, att->angle.current);
	charaFlipX(easingEfc, att->flipFlag);
	int alpha = (int)(att->alpha.current);
	int red = att->red;
	int green = att->green;
	int blue = att->blue;
	charaColor(easingEfc, att->blend, alpha, red, green, blue);
	return 0;
}
static int updateWyvernFire(pCharacter efc) {
	FireEffectAtt *att = &efc->att.fireEfc;
	int current = att->currentTime;
	int life = att->lifeTime;
	if (current >= life) {
		charaClear(efc);
	}
	XYd v = att->velocity;
	efc->pos.x += v.x;
	efc->pos.y += v.y;
	v.x *= 0.99;
	v.y *= 0.99;
	att->velocity = v;
	att->currentTime++;
	ParameterUpdate(&att->alpha, current, life);
	int alpha = (int)(att->alpha.current);
	int red = att->red;
	int green = att->green;
	int blue = att->blue;
	charaColor(efc, att->blend, alpha, red, green, blue);
	return 0;
}
static int updateCloud(pCharacter easingEfc) {
	EasingEffectAtt *att = &easingEfc->att.easingEfc;
	int current = att->currentTime;
	int life = att->lifeTime;
	ParameterUpdate(&att->x, current, life);
	ParameterUpdate(&att->y, current, life);
	ParameterUpdate(&att->scale, current, life);
	ParameterUpdate(&att->angle, current, life);
	ParameterUpdate(&att->alpha, current, life);
	current++;
	if (current >= life) {
		charaClear(easingEfc);
	}
	att->currentTime = current;
	charaOffSet(easingEfc, att->offsetX + att->x.current, att->offsetY + att->y.current);
	charaScale(easingEfc, att->scale.current, att->scale.current);
	charaRot(easingEfc, att->angle.current);
	double alphaT = (double)current / (double)life;
	double alphaS = (1 - alphaT);
	int alpha = (int)((att->alpha.current) * alphaS);
	int red = att->red;
	int green = att->green;
	int blue = att->blue;
	charaColor(easingEfc, att->blend, alpha, red, green, blue);
	return 0;
}
//エフェクト準備関数
static int setUpEffect(pCharacter easingEfc, double x, double y, ImageFileId fileId) {
	AnimaData animData = { 0 };
	createAnimaData(&animData, fileId);
	animaSetup(&easingEfc->anim, getGameFrame(), &animData);
	charaSetImageHandle(easingEfc, imageMgrGetHandle(fileId, 0));
	charaSetDispSize(easingEfc, fileId);
	int w = easingEfc->draw.size.x;
	int h = easingEfc->draw.size.y;
	int w2 = w / 2, h2 = h / 2;
	charaHome(easingEfc, w2, h2);
	easingEfc->imageFileId = fileId;
	easingEfc->att.easingEfc.offsetX = x + w2;
	easingEfc->att.easingEfc.offsetY = y + h2;
	return 0;
}

static EasingEffectAtt baseBlackGrainAtt = {
	0,0,							//offset
	{EASE_OUT_QUAD,0,0,0},			//x
	{EASE_OUT_QUAD,0,0,-300},		//y
	{EASE_OUT_QUAD,1,1,0},			//scale
	{EASE_OUT_QUAD,0,0,0},			//angle
	{EASE_OUT_QUAD,255,255,0},		//alpha
	255,255,255,					//red,green,blue
	0,								//currentTime
	128,							//lifeTime
	AMDRAW_ALPHABLEND,				//blend
	false,							//shouldAnim
};
static int setBlackGrain(pCharacter easingEfc, double x, double y) {
	easingEfc->att.easingEfc = baseBlackGrainAtt;
	setUpEffect(easingEfc, x, y, IMAGE_BLACK_GRAIN);
	easingEfc->att.easingEfc.scale.start += rand0to1();
	charaFuncSet(easingEfc, updateEasingEffect);
	return 0;
}

static EasingEffectAtt baseDarkBallAtt = {
	0,0,									//offset
{EASE_OUT_QUAD,0,0,0},						//x
{EASE_OUT_QUAD,0,0,-32},					//y
{EASE_OUT_QUAD,1,2,3},						//scale
{EASE_OUT_QUAD,0,0,0},						//angle
{EASE_OUT_QUAD,128,128,0},					//alpha
255,255,255,								//red,green,blue
0,											//currentTime
32,											//lifeTime
AMDRAW_ADDBLEND,							//blend
false,										//shouldAnim
};
static int setDarkBall(pCharacter efc, double x, double y) {
	efc->att.easingEfc = baseDarkBallAtt;
	setUpEffect(efc, x, y, IMAGE_DARKBALL);
	charaFuncSet(efc, updateEasingEffect);
	return 0;
}

static EasingEffectAtt baseFireSpreadAtt = {
	0,0,								//offset
{EASE_OUT_CUBIC,-80,-80,-80},			//x
{EASE_OUT_CUBIC,-120,-120,-120},		//y
{EASE_OUT_CUBIC,1,0,3},					//scale
{EASE_OUT_CUBIC,0,0,0},					//angle
{EASE_OUT_CUBIC,128,128,128},			//alpha
255,255,255,							//red,green,blue
0,										//currentTime
16,										//lifeTime
AMDRAW_ADDBLEND,						//blend
false,									//shouldAnim
};
static int setFireSpread(pCharacter efc, double x, double y) {
	efc->att.easingEfc = baseFireSpreadAtt;
	setUpEffect(efc, x, y, IMAGE_FIRESPREAD);
	charaFuncSet(efc, updateEasingEffect);
	return 0;
}

static EasingEffectAtt baseBoss1DamageAtt = {
	0,0,								//offset
{EASE_OUT_SINE,0,0,0},					//x
{EASE_OUT_SINE,0,0,0},					//y
{EASE_OUT_SINE,1,1,1.1},				//scale
{EASE_OUT_SINE,0,0,0},					//angle
{EASE_OUT_SINE,128,128,0},				//alpha
256,8,0,								//red,green,blue
0,										//currentTime
6,										//lifeTime
AMDRAW_ADDBLEND,						//blend
false,									//shouldAnim
};
static int setBoss1Damage(pCharacter efc, double x, double y) {
	efc->att.easingEfc = baseBoss1DamageAtt;
	ImageFileId id = IMAGE_NONE;
	int animIndex = 0;
	bool flipFlagX = false;
	getBoss1FileIdForEffect(&id, &animIndex, &flipFlagX);
	setUpEffect(efc, x, y, id);
	efc->draw.flipFlagX = flipFlagX;
	charaSetImageHandle(efc, imageMgrGetHandle(id, animIndex));
	charaFuncSet(efc, updateEasingEffect);
	return 0;
}

static FireEffectAtt baseWyvernRemainFireAtt = {
	{0.0,-0.5},					//velocity
	{EASE_LINEAR,128,128,0},	//alpha
	64,16,4,					//red, green, blue
	0,							//currentTime
	32,							//lifeTime
	AMDRAW_ADDBLEND,			//blend
};
static int setWyvernRemainFire(pCharacter efc, double x, double y) {
	int handle = imageMgrGetHandle(IMAGE_FIREBALL, 0);
	charaSetImageHandle(efc, handle);
	FireEffectAtt *att = &efc->att.fireEfc;
	XYd *v = &att->velocity;
	*att = baseWyvernRemainFireAtt;
	double randW = 1.5;
	v->x += (rand0to1() * 2 - 1) * randW;
	v->y += (rand0to1() * 2 - 1) * randW;
	charaOffSet(efc, x, y);
	charaSetDispSize(efc, HIT_WYVERN_FIRE_A);
	int w = efc->draw.size.x;
	int h = efc->draw.size.y;
	int w2 = w / 2, h2 = h / 2;
	charaHome(efc, w2, h2);
	charaScale(efc, 3.0, 3.0);
	charaColor(efc, att->blend, (int)(att->alpha.current),
		att->red, att->green, att->blue);
	charaFuncSet(efc, updateWyvernFire);
	return 0;
}

static FireEffectAtt baseWyvernNotLeftFireAtt = {
	{0.0,-0.5},					//velocity
	{EASE_LINEAR,128,128,0},	//alpha
	196,64,32,					//red, green, blue
	0,							//currentTime
	4,							//lifeTime
	AMDRAW_ADDBLEND,			//blend
};
static int setWyvernNotLeftFire(pCharacter efc, double x, double y) {
	setWyvernRemainFire(efc, x, y);
	efc->att.fireEfc = baseWyvernNotLeftFireAtt;
	return 0;
}

static EasingEffectAtt baseCloudAtt = {
	0,0,							//offset
{EASE_LINEAR,0,0,50},				//x
{EASE_LINEAR,0,0,0},				//y
{EASE_LINEAR,1,1,1.1},				//scale
{EASE_LINEAR,0,0,0},				//angle
{EASE_LINEAR,0,0,20},				//alpha
256,256,256,						//red,green,blue
0,									//currentTime
128,								//lifeTime
AMDRAW_ADDBLEND,					//blend
false,								//shouldAnim
};
static int setCloud(pCharacter efc, double x, double y) {
	efc->att.easingEfc = baseCloudAtt;
	ImageFileId fileId = IMAGE_NONE;
	switch (rand() % 3) {
	case 0:
		fileId = IMAGE_CLOUD_A;
		break;
	case 1:
		fileId = IMAGE_CLOUD_B;
		efc->att.easingEfc.x.stop += 100;
		break;
	case 2:
		fileId = IMAGE_CLOUD_C;
		efc->att.easingEfc.x.stop += 200;
		break;
	default:
		break;
	}
	setUpEffect(efc, x, y, fileId);
	efc->draw.size.x *= 3;
	efc->draw.size.y *= 2;
	efc->draw.relative.x = -(efc->draw.size.x) / 2;
	efc->draw.relative.y = -(efc->draw.size.y) / 2;
	charaFuncSet(efc, updateCloud);
	efc->update(efc);
	return 0;
}

static EasingEffectAtt base2PSubAtkBulletAtt = {
0,0,									//offset
{EASE_LINEAR,0,0,0},					//x
{EASE_LINEAR,0,0,0},					//y
{EASE_NONE,1,1,1},						//scale
{EASE_NONE,0,0,0},						//angle
{EASE_NONE,255,255,255},				//alpha
255,255,255,							//red,green,blue
0,										//currentTime
9 * 2,									//lifeTime
AMDRAW_ALPHABLEND,						//blend
true,									//shouldAnim
};
static int set2PsubAtkBullet(pCharacter efc, double x, double y) {
	efc->att.easingEfc = base2PSubAtkBulletAtt;
	efc->dir = (Direction)getPlayer2SubAtkDirectionForEffect();
	efc->att.easingEfc.flipFlag = (efc->dir == LEFT);
	setUpEffect(efc, x, y, IMAGE_2P_SUB_ATK_BULLET_EFFECT);
	efc->att.easingEfc.shouldAnim = true;
	charaFuncSet(efc, updateEasingEffect);
	return 0;
}

static EasingEffectAtt baseThunderSmallAtt = {
	0,0,								//offset
{EASE_LINEAR,0,0,0},					//x
{EASE_LINEAR,0,0,0},					//y
{EASE_LINEAR,1,1,1},					//scale
{EASE_LINEAR,0,0,0},					//angle
{EASE_LINEAR,32,32,0},					//alpha
255,255,255,							//red,green,blue
0,										//currentTime
32,										//lifeTime
AMDRAW_ALPHABLEND,						//blend
false,									//shouldAnim
};
static int setThunderSuggestion(pCharacter efc, double x, double y) {
	efc->att.easingEfc = baseThunderSmallAtt;
	ImageFileId fileId = IMAGE_THUNDER_SUGGESTION;
	AnimaData animData = {0};
	createAnimaData(&animData, fileId);
	animaSetup(&efc->anim, getGameFrame(), &animData);
	charaSetImageHandle(efc, imageMgrGetHandle(fileId, 0));
	charaSetDispSize(efc, fileId);
	efc->imageFileId = fileId;
	efc->att.easingEfc.offsetX = x;
	efc->att.easingEfc.offsetY = y;
	charaSetDispSize(efc, DISP_THUNDER_SUGGESTION);
	charaFuncSet(efc, updateEasingEffect);
	efc->update(efc);
	return 0;
}

///エフェクトを出す関数
extern int inGameEffctSet(InGameEffectType effectType, double x, double y, EffectFrontOrBack which) {
	pCharacter efc = nullptr;
	if (which == EFFECT_FRONT) {
		efc = getFrontEffect();
	}
	else if (which == EFFECT_BACK) {
		efc = getBackEffect();
	}
	if (efc == nullptr) {
		return -1;
	}
	charaInit(efc);
	efc->isActive = true;
	switch (effectType) {
	case IN_GAME_EFFECT_NONE:
		break;
	case IN_GAME_EFFECT_DARK_BALL:
		setDarkBall(efc, x, y);
		break;
	case IN_GAME_EFFECT_FIRE_SPREAD:
		setFireSpread(efc, x, y);
		break;
	case IN_GAME_EFFECT_BOSS1_DAMAGE:
		setBoss1Damage(efc, x, y);
		break;
		break;
	case IN_GAME_EFFECT_ENEMY_DEAD:
		break;
	case IN_GAME_EFFECT_BLACK_GRAIN:
		setBlackGrain(efc, x, y);
		break;
	case IN_GAME_EFFECT_WYVERN_REMAIN_FIRE:
		setWyvernRemainFire(efc, x, y);
		break;
	case IN_GAME_EFFECT_WYVERN_NOT_LEFT_FIRE:
		setWyvernNotLeftFire(efc, x, y);
		break;
	case IN_GAME_EFFECT_CLOUD:
		setCloud(efc, x, y);
		break;
	case IN_GAME_EFFECT_2P_SUB_ATK_BULLET:
		set2PsubAtkBullet(efc, x, y);
		break;
	case IN_GAME_EFFECT_THUNDER_SUGGESTION:
		setThunderSuggestion(efc, x, y);
		break;
	default:
		break;
	}
	return 0;
}
