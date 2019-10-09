/*
** ゲーム本体
*/



// アプリの初期化
// 起動時に1回だけ実行される
void AppInit();

// アプリの解放
// 終了時に1回だけ実行される
void AppRelease();



//
// フレーム処理。1フレームに1回ずつ実行される
//

// フレーム処理：入力
void FrameInput();

// フレーム処理：計算
void FrameProcess();

// フレーム処理：描画
void FrameDraw();

