
#include <stdio.h>
#include "image_manager.h"
#include "chara_player1.h"
#include "chara_player2.h"
#include "chara__hit_rect.h"
#include "chara__manager.h"
#include "chara_block.h"
#include "in_game_effect.h"
#include "stage_data_reader.h"

#define FILE_NAME_MAX				FILENAME_MAX
#define FILE_LINE_MAX				99
#define FILE_PATH					"_data\\item_data\\"
#define FILE_NAME_HPHEALITEM		"hp_heal_item_data.txt"

typedef struct{
	double gravity;
	int healPoint;
}HpHealItemData;
HpHealItemData hpHealItemData;

int loadItemDataFromFile(){



	FILE *in = nullptr;
	char fileName[FILE_NAME_MAX] = "\0";
	char comment[FILE_LINE_MAX] = "\0";
	int opened = -1;
	strncpy_s(fileName, FILE_PATH, FILE_NAME_MAX);
	strcat_s(fileName, FILE_NAME_MAX, FILE_NAME_HPHEALITEM);
	opened = fopen_s(&in, fileName, "r");
	if(opened){
		exit(0);
	} else{
		fscanf_s(in, "%lf", &hpHealItemData.gravity);
		fgets(comment, FILE_LINE_MAX, in);
		fscanf_s(in, "%d", &hpHealItemData.healPoint);
		fgets(comment, FILE_LINE_MAX, in);
		fclose(in);
	}

	return 0;

}

static int hpHealItemUpdate(pCharacter item){
	ItemAtt *att = &item->att.item;
	XYd velocity = att->velocity;
	double x = 0, y = 0;
	
	getCharaOffSet(item, &x, &y);
	x += att->velocity.x;
	y += att->velocity.y;
	charaOffSet(item, x, y);
	velocity.y += hpHealItemData.gravity;
	int hitInfo = charaBlockingByBlock((void*)item);
	if(charaLanding(hitInfo)){
		//着地
		velocity = {0,0};
	}
	att->velocity = velocity;
	//1pとの当たり判定
	if(charaHit(item, player1)){
		player1HealHp(hpHealItemData.healPoint);
		charaClear(item);
	}
	//2pとの当たり判定
	if(charaHit(item, player2)){
		player2HealHp(hpHealItemData.healPoint);
		charaClear(item);
	}
	return 0;
}

int hpHealItemSet(int x, int y, double vx, double vy){
	int found = charaFindNonActive(hpHealItem, HP_HEAL_ITEM_NUM);
	if(found < 0 || found >= HP_HEAL_ITEM_NUM){
		return -1;
	}
	pCharacter item = &hpHealItem[found];
	ItemAtt *att = &item->att.item;
	ImageFileId fileId = IMAGE_HP_HEAL_ITEM;
	charaInit(item);
	item->isActive = true;
	charaSetDispSize(item, DISP_HP_HEAL_ITEM);
	charaSetImageHandle(item, imageMgrGetHandle(fileId, 0));
	int w = item->draw.size.x;
	int h = item->draw.size.y;
	charaOffSet(item, x - w / 2, y - h / 2);
	charaColSet(item, HIT_HP_HEAL_ITEM);
	charaFuncSet(item, hpHealItemUpdate);
	att->velocity.x = vx;
	att->velocity.y = vy;
	return 0;
}

static int fogUpdate(pCharacter){
	//雲のエフェクトを出す
	for(int i = 0; i < 12; i++){
		EffectFrontOrBack which = EFFECT_BACK;
		if(i >= 4){
			which = EFFECT_FRONT;
		}
		inGameEffctSet(IN_GAME_EFFECT_CLOUD,
					   rand() % WORLD_WIDTH, rand() % WORLD_HEIGHT,
					   which);
	}
	return 0;
}

int fogSet(){
	int nonActive = charaFindNonActive(fogGenerator, FOG_GENERATOR_NUM);
	if(nonActive < 0){
		return -1;
	}
	pCharacter fog = &fogGenerator[nonActive];
	fog->isActive = true;
	charaFuncSet(fog, fogUpdate);
	return 0;
}

int fogDispel(){
	int active = charaFindActive(fogGenerator, FOG_GENERATOR_NUM);
	if(active < 0){
		return -1;
	}
	fogGenerator[active].isActive = false;
	return 0;
}
