//	
//	冬期制作「Magnet Hero」
//	
//	プログラムのエントリーポイントが定義されているファイルです
//	
//	作成者	森	仁志
//	

#include "framework\framework.h"

//エントリーポイントです
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//参照しないパラメーター
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
	//ゲームの初期化処理
	if (frame_init(hInstance) == 0)
	{
		//ゲームを続けるべきか
		while (frame_continue())
		{
			//ゲームの更新処理
			frame_update();
			//ゲームの描画処理
			frame_draw();
		}
	}
	//ゲームの終了処理
	frame_end();
	//リザルトを返す
	return S_OK;
}
