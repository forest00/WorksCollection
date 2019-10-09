



#include "type_parameter.h"
#include "chara__common.h"
#include "chara__manager.h"
#include "chara_attacker.h"
#include "camera.h"
#include "common.h"
#include "game_state.h"


int setAttacker(double x, double y, int hitX, int hitY, int hitW, int hitH){
	int found = charaFindNonActive(attackers, ATTACKER_NUM);
	if(found < 0){
		return -1;
	}
	Character *actor = &attackers[found];
	charaInit(actor);
	actor->isActive = true;
	actor->pos.x = x;
	actor->pos.y = y;
	actor->hit.relative.x = hitX;
	actor->hit.relative.y = hitY;
	actor->hit.size.x = hitW;
	actor->hit.size.y = hitH;
	actor->att.atk.locked = false;
	actor->draw.handle = -1;
	return found;
}
int setAttacker(double x, double y, const pBox hit){
	return setAttacker(x, y, hit->relative.x, hit->relative.y, hit->size.x, hit->size.y);
}
int setAttacker(double x, double y, CharaHitAreaId id){
	return setAttacker(x, y, (pBox)getBoxData(id));
}

int attackerLock(int index){
	if(index < 0 || index >= ATTACKER_NUM){
		return -1;
	}
	attackers[index].att.atk.locked = true;
	return 0;
}

int attackerUnLock(int index){
	if(index < 0 || index >= ATTACKER_NUM){
		return -1;
	}
	attackers[index].att.atk.locked = false;
	return 0;
}

int attackerOffset(int index, double x, double y){
	if(index < 0 || index >= ATTACKER_NUM){
		return -1;
	}
	Character *target = &attackers[index];
	target->pos.x = x;
	target->pos.y = y;
	return index;
}


int getAttackerCoord(int index, double *x, double *y){
	if(index < 0 || index >= ATTACKER_NUM){
		return -1;
	}
	*x = attackers[index].pos.x;
	*y = attackers[index].pos.y;
	return index;
}

bool checkAttackerHitRect(int index, pRect rect){
	if(index < 0 || index >= ATTACKER_NUM){
		return false;
	}
	return isHitBox((int)attackers[index].pos.x, (int)attackers[index].pos.y,
					&attackers[index].hit, rect);
}

bool checkAttackerHitChara(int index, pCharacter chara){
	if(index < 0 || index >= ATTACKER_NUM){
		return false;
	}
	Character *tester = &attackers[index];
	if(!tester->isActive){
		return false;
	}
	int left = (int)(tester->pos.x) + tester->hit.relative.x;
	int top = (int)(tester->pos.y) + tester->hit.relative.x;
	int right = left + tester->hit.size.x;
	int bottom = top + tester->hit.size.y;
	const Rect testArea = {left,top,right,bottom};
	if(charaHitBox(chara, &testArea)){
		return true;
	}
	return false;
}

pCharacter checkAttackerHitCharacters(int index, pCharacter _array, int num){
	if(index < 0 || index >= ATTACKER_NUM){
		return nullptr;
	}
	Character *tester = &attackers[index];
	if(!tester->isActive){
		return nullptr;
	}
	int left = (int)(tester->pos.x) + tester->hit.relative.x;
	int top = (int)(tester->pos.y) + tester->hit.relative.y;
	int right = left + tester->hit.size.x;
	int bottom = top + tester->hit.size.y;
	const Rect testArea = {left,top,right,bottom};
	for(int i = 0; i < num; i++){
		if(_array[i].isActive){
			if(charaHitBox(&_array[i], &testArea)){
				return &_array[i];
			}
		}
	}
	return nullptr;
}

int clearAttacker(int index){
	if(index < 0 || index >= ATTACKER_NUM){
		return -1;
	}
	if(attackers[index].att.atk.locked){
		return -1;
	}
	attackers[index].isActive = false;
	return 0;
}
