
#include <time.h>
#include <Windows.h>
#include "amgame.h"

#include "game.h"
#include "game_state.h"
#include "input_manager.h"
#include "sound_manager.h"
#include "option.h"
#include "init_graph_no_sys.h"

//
//#include "image_manager.h"
//



//
// WinMain(). プログラム起動関数
//
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
) {
	//warning対策
	(void)nCmdShow;
	(void)lpCmdLine;
	(void)hPrevInstance;
	//warning対策ここまで

	//
	// AMライブラリの初期化
	//

	//SetGlobalMode(AMMODE_WINDOWED);	// これをコメントアウトするとフルスクリーンで動く
	SetGraphMode(DISP_W, DISP_H, 32);///
	if(!InitGraphNoSys(hInstance)){	// ライブラリ初期化失敗
		return -1;
	}

	SetDrawMode(AMDRAW_BILINEAR, 0);


	SetAMDebugMode(AMDEBUG_REGISTER_OFF);
	SetAMDebugMode(AMDEBUG_DISPLAY_OFF);
	SetAMDebugMode(AMDEBUG_KEY_OFF);
	SetAMDebugMode(AMDEBUG_FILE_OFF);

	InitSound(0);

	soundMgrLoad();

	AppInit();///

	srand((unsigned int)time(nullptr));
	
	int							hTitle;							// タイトル

	hTitle = LoadTexture(AM_TITLE_NAME);
	if (hTitle < 0) {
		if (gg_bFullMode) {
			DrawString(0, 0, "Amgamew.sysがありません！！", GetColor(255, 0, 128));
			Flip();
			GetCh();
		} else {
			MessageBox(gg_hWindow, "Amgamew.sysがありません。", "Error", MB_OK);
		}
		EndGraph();
		return NULL;
	}

	int							i, j;
	int							nTitleWidth, nTitleHeight;
	int							nTitleX, nTitleY;
	float						fYLoc;

	GetPictureSize(hTitle, &nTitleWidth, &nTitleHeight);
	nTitleX = (gg_siDispSize.cx - nTitleWidth) / 2;
	nTitleY = (gg_siDispSize.cy - nTitleHeight) / 2;

	AmVertex		avVertices[480 * 2];			// 頂点

													// 頂点情報の初期化（ラスタスクロール用）
	for (i = 0; i < 480; i++) {
		SetColorVertex(&avVertices[i * 2], 0.0f, (gg_siDispSize.cy - 480.0f) / 2.0f + i, 0.0f,
			0.5f, i * nTitleHeight / 482.0f + 0.5f,
			0xff, 0xff, 0xff, 0xff);
		SetColorVertex(&avVertices[i * 2 + 1], 640.0f, (gg_siDispSize.cy - 480.0f) / 2.0f + i, 0.0f,
			(float)nTitleWidth - 1.0f, i * nTitleHeight / 482.0f + 0.5f,
			0xff, 0xff, 0xff, 0xff);
	}
	SetDrawMode(AMDRAW_BILINEAR, 0);

	for (i = 0; i <= 1000; i += 20) {
		AM_ClearBackBuffer(255, 255, 255);
		fYLoc = (800.0f - i) * (800.0f - i) * nTitleHeight * 0.2f / (200.0f * 200.0f) - nTitleHeight * 0.2f;
		for (j = 0; j < 480; j++) {
			avVertices[j * 2].x = nTitleX - (j - 240.0f) * (j - 240.0f) / (240.0f * 240.0f) * 1000.0f;
			avVertices[j * 2 + 1].x = nTitleX + nTitleWidth + (j - 240.0f) * (j - 240.0f) / (240.0f * 240.0f) * 1000.0f;
			avVertices[j * 2].v = -fYLoc + j * nTitleHeight / 482.0f + 0.5f;
			if (avVertices[j * 2].v > nTitleHeight - 1.0f) avVertices[j * 2].v = nTitleHeight - 1.0f;
			avVertices[j * 2 + 1].v = avVertices[j * 2].v;
		}
		Draw3DLines(avVertices, 480 * 2, hTitle);	// 線描画
		Flip();
	}

	WaitByTimer(500);

	for (i = 0; i <= 1000; i += 30) {
		AM_ClearBackBuffer(255, 255, 255);
		for (j = 0; j < 480; j++) {
			avVertices[j * 2].x = nTitleX - (j - 240.0f) * (j - 240.0f) / (240.0f * 240.0f)
				* (1000 - i) * 700.0f / 1000.0f;
			avVertices[j * 2 + 1].x = nTitleX + nTitleWidth + (j - 240.0f) * (j - 240.0f) / (240.0f * 240.0f)
				* (1000 - i) * 700.0f / 1000.0f;
			avVertices[j * 2].v = (j - 240) * (nTitleHeight / 482.0f * (1000.0f - i) + i) / 1000.0f + nTitleHeight / 2 + 0.5f;
			if (avVertices[j * 2].v > nTitleHeight - 1.0f) avVertices[j * 2].v = nTitleHeight - 1.0f;
			avVertices[j * 2 + 1].v = avVertices[j * 2].v;
		}
		Draw3DLines(avVertices, 480 * 2, hTitle);	// 線描画
		Flip();
	}

	AM_ClearBackBuffer(255, 255, 255);
	SetDrawMode(AMDRAW_NEAREST, 0);
	DrawMem(nTitleX, nTitleY, hTitle);
	Flip();
	if (!KBHit()) WaitByTimer(2000);
	else GetCh();

	for (i = 1000; i >= 0; i -= 20) {
		AM_ClearBackBuffer(i * 255 / 1000, i * 255 / 1000, i * 255 / 1000);
		SetDrawBright(i, i, i);
		DrawMem(nTitleX, nTitleY, hTitle);
		Flip();
	}
	
	while(AM_ProcessEvent()){

		FrameInput();///

		FrameProcess();///

		FrameDraw();///

	}

	AppRelease();///

	EndSound();

	EndGraph();

	return 0;
}





