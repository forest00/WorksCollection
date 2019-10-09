

#include "chara_block.h"
#include "game_state.h"
#include "image_manager.h"
#include "chara__manager.h"
#include "stage_data_reader.h"


#include <stdlib.h>


static int lSetBlock(int x, int y, int w, int h, int hitDir){
	if(getSelectedStage() >= 3){
		int hBlock = -1;
		int found = charaFindNonActive(block, BLOCK_NUM);
		if(found < 0){
			return -1;
		}
		double xX = (double)WORLD_WIDTH / DISP_W;
		double yY = (double)WORLD_HEIGHT / DISP_H;
		pCharacter blk = &block[found];
		charaInit(blk);
		blk->isActive = true;
		charaOffSet(blk, x, y);
		charaColSet(blk, 0, 0, w, h);
		blk->hit.relative.x = (int)(blk->hit.relative.x * xX);
		blk->hit.relative.y = (int)(blk->hit.relative.y * yY);
		blk->hit.size.x = (int)(blk->hit.size.x * xX);
		blk->hit.size.y = (int)(blk->hit.size.y * yY);
		charaSetImageHandle(blk, hBlock);
		blk->draw.relative = {-x,-y};
		blk->draw.relative.x += 0;
		blk->draw.relative.y += 60;
		blk->draw.size = {WORLD_WIDTH,WORLD_HEIGHT};
		blk->att.blkHitDir = hitDir;
		return 0;
	} else{
		int hBlock = -1;
		ImageFileId fileId = IMAGE_BLOCK_STAGE1;
		hBlock = imageMgrGetHandle(fileId, 0);
		int found = charaFindNonActive(block, BLOCK_NUM);
		if(found < 0){
			return -1;
		}
		pCharacter blk = &block[found];
		charaInit(blk);
		blk->isActive = true;
		charaOffSet(blk, x, y);
		charaColSet(blk, 0, 0, w, h);
		charaSetImageHandle(blk, hBlock);
		charaSetDispSize(blk, w, h);
		blk->att.blkHitDir = hitDir;
		return 0;
	}
}

int setBlock(int x, int y, int w, int h){
	return lSetBlock(x, y, w, h, HIT_DIR_UP | HIT_DIR_DOWN | HIT_DIR_LEFT | HIT_DIR_RIGHT);
}
int setBlockU(int x, int y, int w, int h){
	return lSetBlock(x, y, w, h, HIT_DIR_UP);
}

int charaBlockingByBlock(void *ptr){
	pCharacter chara = (pCharacter)ptr;
	int hitInfo = 0;
	for(int i = 0; i < BLOCK_NUM; i++){
		pCharacter p = &block[i];
		if(charaHit(chara, p)){
			hitInfo |= charaCorrect(chara, p, p->att.blkHitDir);
		}
	}
	return hitInfo;
}

bool isCharaStandingOnBlock(void *ptr){
	pCharacter chara = (pCharacter)ptr;
	int cl = (int)(chara->pos.x) + chara->hit.relative.x;
	int ct = (int)(chara->pos.y) + chara->hit.relative.y;
	int cr = cl + chara->hit.size.x;
	int cb = ct + chara->hit.size.y;
	for(int i = 0; i < BLOCK_NUM; i++){
		pCharacter p = &block[i];
		int l = (int)(p->pos.x) + p->hit.relative.x;
		int t = (int)(p->pos.y) + p->hit.relative.y;
		int r = l + p->hit.size.x;
		int b = t + p->hit.size.y;
		if(isHitBox(l, t, r, b, cl, ct, cr, cb + 1)){
			if(!isHitBox(l, t, r, b, cl, ct, cr, cb - 1)){
				return true;
			}
		}
	}
	return false;
}