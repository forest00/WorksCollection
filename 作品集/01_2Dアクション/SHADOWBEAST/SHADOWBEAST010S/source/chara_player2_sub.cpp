
#include <math.h>
#include "chara__manager.h"
#include "chara__hit_rect.h"
#include "chara_enemy.h"
#include "in_game_effect.h"

static const double BULLET_RANGE = 128;
static Direction bulletDirectionForEffect = LEFT;

static int updatePlayer2SubAtk(pCharacter bullet){
	Player2SubAtt *att = &bullet->att.pl2Sub; 
	//バレット移動↓
	att->time++;
	if(att->time <= att->delay){
		return -1;
	}
	double x = 0, y = 0;
	getCharaOffSet(bullet, &x, &y);
	XYd velocity = att->velocity;
	double d = hypot(velocity.x, velocity.y);
	att->movementAmount += d;
	//範囲外なら消す↓
	if(att->movementAmount > BULLET_RANGE){
		charaClear(bullet);
	}
	//範囲外なら消す↑
	x += velocity.x;
	y += velocity.y;
	charaOffSet(bullet, x, y);
	//バレット移動↑
	//バレット当たり判定↓
	int hitting = charaBulkHit(enemies, ENEMY_NUM, bullet);
	if(hitting >= 0){
		pCharacter enemy = &enemies[hitting];
		XYd _velocity = {22,-10};
		if(att->velocity.x < 0){
			_velocity.x *= -1;
		}
		enemyBlow(enemy, _velocity.x, _velocity.y);
	}
	//バレット当たり判定↑
	return 0;
}

int player2SubAtkSet(double x, double y, double vx, double vy, int delay){
	int found = charaFindNonActive(player2Sub, PLAYER2_SUB_NUM);
	if(found < 0){
		return -1;
	}
	pCharacter actor = &player2Sub[found];
	Player2SubAtt *att = &actor->att.pl2Sub;
	int __x__ = 0;
	if(vx > 0){
		actor->dir = RIGHT;
		__x__ = 120;
	} else{
		actor->dir = LEFT;
		__x__ = -110;
	}
	bulletDirectionForEffect = actor->dir;
	inGameEffctSet(IN_GAME_EFFECT_2P_SUB_ATK_BULLET, x - 350 + __x__, y - 200 - 110, EFFECT_BACK);
	charaInit(actor);
	actor->isActive = true;
	charaOffSet(actor, x, y);
	charaColSet(actor, HIT_PLAYER2_SUB_A);
	charaFuncSet(actor, updatePlayer2SubAtk);
	att->time = 0;
	att->delay = delay;
	att->velocity.x = vx;
	att->velocity.y = vy;
	att->movementAmount = 0;
	return 0;
}

int getPlayer2SubAtkDirectionForEffect(){
	return (int)bulletDirectionForEffect;
}
