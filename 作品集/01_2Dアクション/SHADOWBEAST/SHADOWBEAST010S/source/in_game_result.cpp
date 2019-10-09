
#include "amgame.h"
#include "game_state.h"
#include "image_manager.h"

//���e���錅��
static constexpr int DIGIT_MAX = 3;
//�\�����������̐�
static constexpr int INFO_NUM = 3;


struct ResultInfoNumber
{
	int value;
	int offsetX;
	int offsetY;
	int drawRelativeX;
	int drawRelativeY;
	int drawSizeX;
	int drawSizeY;
	int GetDrawArea(int x, int y, int &left, int &top, int &right, int &bottom);
	int Draw(int x, int y, int alpha);
};

int ResultInfoNumber::GetDrawArea(int x, int y, int &left, int &top, int &right, int &bottom)
{
	x += offsetX;
	y += offsetY;
	left = x + drawRelativeX;
	top = y + drawRelativeY;
	right = left + drawSizeX;
	bottom = top + drawSizeY;
	return 0;
}

int ResultInfoNumber::Draw(int x, int y, int alpha)
{
//value��0�ȊO�̈ꌅ�̐��̐����łȂ���Ή������Ȃ�
	if (!(value >= 0 && value < 10))
	{
		return -1;
	}
	int l = 0, t = 0, r = 0, b = 0;
	GetDrawArea(x, y, l, t, r, b);
	SetDrawMode(AMDRAW_ALPHABLEND, alpha);
	SetDrawBright(1000, 1000, 1000);
	ImageFileId fileId = IMAGE_NUMBER_0;
	fileId = (ImageFileId)(fileId + value);
	int handle = imageMgrGetHandle(fileId, 0);
	DrawMemThExp(l, t, r, b, handle);
	return 0;
}



struct ResultInfoWnd
{
	ResultInfoNumber number[DIGIT_MAX];
	bool isActive;
	bool isDraw;
	int activeCounter;
	int appearDelay;
	int appearCountLimit;
	int offsetX;
	int offsetY;
	int drawRelativeX;
	int drawRelativeY;
	int drawSizeX;
	int drawSizeY;
	int alphaMax;
	int alpha;
	ImageFileId imageFileId;
	int GetDrawArea(int x, int y, int &left, int &top, int &right, int &bottom);
	int Update(void);
	int Draw(int x, int y);
};

int ResultInfoWnd::GetDrawArea(int x, int y, int &left, int &top, int &right, int &bottom)
{
	x += offsetX;
	y += offsetY;
	left = x + drawRelativeX;
	top = y + drawRelativeY;
	right = left + drawSizeX;
	bottom = top + drawSizeY;
	return 0;
}

//�P�̂̍X�V
int ResultInfoWnd::Update(void)
{
	if (!(isActive))
	{
		return -1;
	}
	int elapsed = activeCounter - appearDelay;
	if (elapsed >= 0)
	{
		isDraw = true;
	}
	if (isDraw)
	{
		double t = (double)(elapsed) / (double)(appearCountLimit);
		alpha = (int)(t * alphaMax);
	}
	activeCounter++;
	return 0;
}

//x,y�͉�ʂ̍���Ƃ���
int ResultInfoWnd::Draw(int x, int y)
{
	if (!(isActive && isDraw))
	{
		return -1;
	}
	int l = 0, t = 0, r = 0, b = 0;
	GetDrawArea(x, y, l, t, r, b);
	SetDrawMode(AMDRAW_NOBLEND, 0);
	SetDrawBright(1000, 1000, 1000);
	int handle = imageMgrGetHandle(imageFileId, 0);
	//�O�g�̕`��
	DrawMemThExp(l, t, r, b, handle);
	//�����̕`��
	for (int i = 0; i < DIGIT_MAX; i++)
	{
		number[i].Draw(l, t, alpha);
	}
	return 0;
}

static ResultInfoWnd windows[INFO_NUM];

//������
static int resultInfoInit()
{
	for each(auto &iter in windows)
	{
		iter = { 0 };
	}
	return 0;
}
//����
int resultInfoSetUp()
{
	resultInfoInit();
	constexpr int delayWidth = 48;
	for (int i = 0; i < INFO_NUM; i++)
	{
		ResultInfoWnd *wnd = &windows[i];
		//�}�W�b�N�i���o�[����
		//�l�X�ȃf�[�^�ƂȂ�
		wnd->isActive = true;
		wnd->alphaMax = 255;
		wnd->appearCountLimit = 32;
		wnd->appearDelay = i * delayWidth + 128;
		wnd->offsetX = DISP_CENTER_X;
		wnd->offsetY = 150 + i * 160;
		wnd->drawSizeX = 600;
		wnd->drawSizeY = 150;
		wnd->drawRelativeX = -(wnd->drawSizeX / 2);
		wnd->drawRelativeY = -(wnd->drawSizeY / 2);
	}
	windows[0].imageFileId = IMAGE_MSG_PLAYER_DEAD_COUNT;
	windows[1].imageFileId = IMAGE_MSG_ENEMY_DEAD_COUNT;
	windows[2].imageFileId = IMAGE_MSG_CASTLE_HP;
	//���l
	int value[INFO_NUM] = {
		getPlayer1DownCount() + getPlayer2DownCount(),
		getEnemyKillCount(),
		(int)(getTopwerDurability() * 100)
	};
	{
		//���ꂼ��̑��̏����Z�b�g
		for (int wi = 0; wi < INFO_NUM; wi++)
		{
//�ꌅ������
			int places[DIGIT_MAX] = { 0 };
			int digit = 1;
			for (int i = 1; i < DIGIT_MAX; i++)
			{
				digit *= 10;
			}
			for (int i = DIGIT_MAX - 1; i >= 0; i--)
			{
				int ans = value[wi] / digit;
				value[wi] -= ans * digit;
				digit /= 10;
				places[i] = ans;
			}
			//�����ꕶ�����Ƃɏ����Z�b�g
			for (int ni = 0; ni < DIGIT_MAX; ni++)
			{
				ResultInfoNumber *num = &windows[wi].number[ni];
				num->value = places[DIGIT_MAX - 1 - ni];
				//�}�W�b�N�i���o�[
				num->offsetX = windows[wi].drawSizeX - (DIGIT_MAX - ni) * 60 - 160;
				num->offsetY = 25;
				num->drawSizeX = 60;
				num->drawSizeY = 60;
				num->drawRelativeX = num->drawSizeX / 2;
				num->drawRelativeY = num->drawSizeY / 2;
			}
		}
	}
	return 0;
}
//�X�V
int resultInfoUpdate()
{
	for each(auto &iter in windows)
	{
		iter.Update();
	}
	return 0;
}
//�`��
int resultInfoDraw()
{
	for each(auto &iter in windows)
	{
		iter.Draw(0, 0);
	}
	return 0;
}
//����
int resultInfoDestroy()
{
	resultInfoInit();
	return 0;
}




