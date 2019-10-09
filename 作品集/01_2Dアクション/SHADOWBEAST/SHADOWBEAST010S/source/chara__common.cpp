#include <Windows.h>
#include "amgame.h"

#include "chara__common.h"
#include "chara__render.h"
#include "common.h"
#include "camera.h"

#include "forDebug.h"

//このファイル内のすべての関数は、
//失敗すると-1を返す


//バグ回避用ダミー関数
int dummyFunc(pCharacter)
{
	return 0;
}

//初期化用

int charaInit(pCharacter target)
{

	*target = { 0 };

	charaDispInfoInit(&target->draw);

	target->update = dummyFunc;

	return 0;
}

int  charaBulkInit(Character characters[], int num)
{
	for (int i = 0; i < num; i++)
	{
		charaInit(&characters[i]);
	}
	return 0;
}



//出現消滅管理用

int charaFindActive(Character characters[], int num)
{
	for (int i = 0; i < num; i++)
	{
		if (characters[i].isActive)
		{
			return i;
		}
	}
	return -1;
}

int charaFindNonActive(Character characters[], int num)
{
	for (int i = 0; i < num; i++)
	{
		if (!(characters[i].isActive))
		{
			return i;
		}
	}
	return -1;
}

int charaSet(Character characters[], int num)
{
	int found = charaFindNonActive(characters, num);
	if (found == -1)
	{
		return -1;
	}
	charaInit(&characters[found]);
	characters[found].isActive = true;
	return found;
}


int charaClear(pCharacter target)
{
	target->isActive = false;
	return 0;
}

int charaBulkClear(Character characters[], int num)
{
	for (int i = 0; i < num; i++)
	{
		charaClear(&characters[i]);
	}
	return 0;
}




//各種情報格納用

//キャラクターの表示用の大きさ情報をセットする
int charaSetDispSize(pCharacter target, int sizeX, int sizeY)
{
	target->draw.size.x = sizeX;
	target->draw.size.y = sizeY;
	return 0;
}
int charaSetDispSize(pCharacter target, ImageFileId id)
{
	int x = 0, y = 0;
	getImageSize(id, &x, &y);
	charaSetDispSize(target, x, y);
	return 0;
}
//注意 : この関数に限り、大きさだけでなく表示用の相対座標もセットする
int charaSetDispSize(pCharacter target, CharaHitAreaId id)
{
	const pBox ptr = (pBox)getBoxData(id);
	target->draw.relative = ptr->relative;
	target->draw.size = ptr->size;
	return 0;
}

int charaOffSet(pCharacter target, double x, double y)
{
	XYd coord = { x,y };
	target->beforePos = target->pos;
	target->pos = coord;
	return 0;
}

int charaColSet(pCharacter target, CharaHitAreaId id)
{
	target->hit = *(pBox)getBoxData(id);
	return 0;
}
int charaColSet(pCharacter target, int x, int y, int w, int h)
{
	pBox p = &target->hit;
	p->relative.x = x;
	p->relative.y = y;
	p->size.x = w;
	p->size.y = h;
	return 0;
}


int charaHome(pCharacter target, int cenX, int cenY)
{
	return charaDispInfoHome(&target->draw, cenX, cenY);
}

int charaScale(pCharacter target, double scaleX, double scaleY)
{
	return charaDispInfoScale(&target->draw, scaleX, scaleY);
}


int charaRot(pCharacter target, double angle)
{
	return charaDispInfoRot(&target->draw, angle);
}

int charaColor(pCharacter target, int blend, int alpha,
	int red, int green, int blue)
{
	return charaDispInfoColor(&target->draw, blend, alpha, red, green, blue);
}

int charaSetImageHandle(pCharacter target, int handle)
{
	return charaDispInfoHandle(&target->draw, handle);
}


int charaFlipX(pCharacter target, bool flagX)
{
	return charaDispInfoFlip(&target->draw, flagX, 0);
}


int charaFuncSet(pCharacter target, int(*update)(pCharacter))
{
	target->update = update;
	return 0;
}


//各種情報取得用

bool getCharaExist(const pCharacter target)
{
	return target->isActive;
}

int getCharaOffSet(const pCharacter target, double *x, double *y)
{
	*x = target->pos.x;
	*y = target->pos.y;
	return 0;
}

int getCharaHitArea(const pCharacter target, double *left, double *top, double *right, double *bottom)
{
	*left = target->pos.x + target->hit.relative.x;
	*top = target->pos.y + target->hit.relative.y;
	*right = *left + target->hit.size.x;
	*bottom = *top + target->hit.size.y;
	return 0;
}

int getCharaHitAreaCenter(const pCharacter target, double *centerX, double *centerY)
{
	double x = target->pos.x + target->hit.relative.x;
	double y = target->pos.y + target->hit.relative.y;
	double w = target->hit.size.x;
	double h = target->hit.size.y;
	*centerX = x + w / 2;
	*centerY = y + h / 2;
	return 0;
}


bool getCharaFlipFlag(const pCharacter target)
{
	return target->draw.flipFlagX;
}





//当たり判定用


int createBoxForHit(const pCharacter chara, pRect out)
{
	int x = (int)(chara->pos.x) + chara->hit.relative.x;
	int y = (int)(chara->pos.y) + chara->hit.relative.y;
	out->left = x;
	out->top = y;
	out->right = x + chara->hit.size.x;
	out->bottom = y + chara->hit.size.y;
	return 0;
}
int createBoxForHit(const pCharacter chara, CharaHitAreaId id, pRect out)
{
	pBox temp = (pBox)getBoxData(id);
	int x = (int)(chara->pos.x) + temp->relative.x;
	int y = (int)(chara->pos.y) + temp->relative.y;
	*out = { x,y,x + temp->size.x,y + temp->size.y };
	return 0;
}

bool charaHit(pCharacter target, pCharacter opponent)
{
	Rect box0, box1;
	createBoxForHit(target, &box0);
	createBoxForHit(opponent, &box1);
	return isHitBox(&box0, &box1);
}

bool charaHitBox(pCharacter target, int left, int top, int right, int bottom)
{
	Rect box0, box1;
	createBoxForHit(target, &box0);
	box1 = { left, top, right, bottom };
	return isHitBox(&box0, &box1);
}
bool charaHitBox(pCharacter target, int x, int y, const pBox in)
{
	Rect box0, box1;
	createBoxForHit(target, &box0);
	BoxToRect(x, y, in, &box1);
	return isHitBox(&box0, &box1);
}
bool charaHitBox(pCharacter target, int x, int y, const Box *in)
{
	Rect box0, box1;
	createBoxForHit(target, &box0);
	BoxToRect(x, y, (const pBox)in, &box1);
	return isHitBox(&box0, &box1);
}
bool charaHitBox(pCharacter target, int x, int y, CharaHitAreaId id)
{
	Rect box0, box1;
	createBoxForHit(target, &box0);
	BoxToRect(x, y, (pBox)getBoxData(id), &box1);
	return isHitBox(&box0, &box1);
}
bool charaHitBox(pCharacter target, const pRect in)
{
	Rect box0;
	createBoxForHit(target, &box0);
	return isHitBox(&box0, in);
}
bool charaHitBox(pCharacter target, const Rect *in)
{
	Rect box0;
	createBoxForHit(target, &box0);
	return isHitBox(&box0, in);
}

int charaBulkHit(Character characters[], int num, pCharacter target)
{
	for (int i = 0; i < num; i++)
	{
		if (charaHit(target, &characters[i]))
		{
			return i;
		}
	}
	return -1;
}

int charaBulkHit(Character characters[], int num, pCharacter target, CharaHitAreaId id)
{
	Rect rect, box;
	createBoxForHit(target, id, &rect);
	for (int i = 0; i < num; i++)
	{
		if (!characters[i].isActive)
		{
			continue;
		}
		createBoxForHit(characters + i, &box);
		if (isHitBox(&rect, &box))
		{
			return i;
		}
	}
	return -1;
}

int charaBulkHit(Character characters[], int num, int x, int y, CharaHitAreaId id)
{
	Rect rect, box;
	pBox temp = (pBox)getBoxData(id);
	x += temp->relative.x;
	y += temp->relative.y;
	rect = { x,y,x + temp->size.x,y + temp->size.y };
	for (int i = 0; i < num; i++)
	{
		if (!characters[i].isActive)
		{
			continue;
		}
		createBoxForHit(characters + i, &box);
		if (isHitBox(&rect, &box))
		{
			return i;
		}
	}
	return -1;
}


int charaCorrect(pCharacter chara, pCharacter subject, int bFlag)
{
	double resist = 1.0;
	if ((bFlag & 15) == 15)
	{
		resist = 0.0;
	}
	int hitInfo = 0;
	int x0 = int(chara->beforePos.x) + chara->hit.relative.x;
	int y0 = int(chara->beforePos.y) + chara->hit.relative.y;
	int w0 = chara->hit.size.x;
	int h0 = chara->hit.size.y;
	int x1 = int(subject->pos.x) + subject->hit.relative.x;
	int y1 = int(subject->pos.y) + subject->hit.relative.y;
	int w1 = subject->hit.size.x;
	int h1 = subject->hit.size.y;
	double halfW0 = w0 / 2.0;
	double halfH0 = h0 / 2.0;
	double midX0 = x0 + halfW0;
	double midY0 = y0 + halfH0;
	double halfW1 = w1 / 2.0;
	double halfH1 = h1 / 2.0;
	double midX1 = x1 + halfW1;
	double midY1 = y1 + halfH1;
	double s_ = (midX0 - midX1) / (halfW0 + halfW1);
	double t_ = (midY0 - midY1) / (halfH0 + halfH1);
	double abs_t = fabs(t_);
	double abs_s = fabs(s_);
	if (abs_t > abs_s)
	{
		if (t_ <= -resist)
		{
			if (bFlag & HIT_DIR_UP)
			{
				chara->pos.y = y1 - chara->hit.size.y - chara->hit.relative.y;
				hitInfo |= HIT_DIR_UP;
			}
		}
		else if (t_ >= resist)
		{
			if ((bFlag & HIT_DIR_DOWN))
			{
				chara->pos.y = subject->pos.y + subject->hit.relative.y + subject->hit.size.y - chara->hit.relative.y;
				hitInfo |= HIT_DIR_DOWN;
			}
		}
		if (abs_t < 1.0)
		{
			hitInfo |= 16;
		}
	}
	else
	{
		if (s_ <= -resist)
		{
			if ((bFlag & HIT_DIR_LEFT))
			{
				chara->pos.x = x1 - chara->hit.size.x - chara->hit.relative.x;
				hitInfo |= HIT_DIR_LEFT;
			}
		}
		else if (s_ >= resist)
		{
			if (bFlag & HIT_DIR_RIGHT)
			{
				chara->pos.x = subject->pos.x + subject->hit.relative.x + subject->hit.size.x - chara->hit.relative.x;
				hitInfo |= HIT_DIR_RIGHT;
			}
		}
		if (abs_s < 1.0)
		{
			hitInfo |= 16;
		}
	}
	return hitInfo;
}

bool charaLanding(int hitInfo)
{
	return hitInfo & HIT_DIR_UP;
}



//実際の動作用
int charaBulkUpdate(Character characters[], int num)
{
	pCharacter chara;
	for (int i = 0; i < num; i++)
	{
		chara = &characters[i];
		if (chara->isActive)
		{
			chara->update(chara);
			if (chara->pos.y > 2000)
			{
				charaClear(chara);
			}
		}
	}
	return 0;
}





//描画用

int charaDraw(pCharacter target)
{
	if (!(target->isActive))
	{
		return -1;
	}
	pDrawInfo info = &target->draw;
	charaDispInfoOffSet(info, (int)target->pos.x, (int)target->pos.y);
	charaDispInfoRender(info);
#ifdef _DBGOPT_DRAW_RECT_
	//デバッグ用
	Rect box;
	createBoxForHit(target, &box);
	double x0 = box.left;
	double y0 = box.top;
	double x1 = box.right;
	double y1 = box.bottom;

	correctByCamera(&x0, &y0);
	correctByCamera(&x1, &y1);

	box.left = (int)x0;
	box.top = (int)y0;
	box.right = (int)x1;
	box.bottom = (int)y1;
	if (x0 >= x1 || y0 >= y1)
	{
		return 0;
	}
	drawBox(&box, GetColor(0, 255, 0));
	box.left--;
	box.top--;
	box.right++;
	box.bottom++;
	drawBox(&box, GetColor(0, 0, 0));
#endif // _DBGOPT_DRAW_RECT_
	return 0;
}

int charaBulkDraw(Character characters[], int num)
{
	for (int i = 0; i < num; i++)
	{
		pCharacter chara = &characters[i];
		charaDraw(chara);
	}
	return 0;
}











