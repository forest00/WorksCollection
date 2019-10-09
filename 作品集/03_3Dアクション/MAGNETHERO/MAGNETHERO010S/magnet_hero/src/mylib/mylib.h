//雑多な関数を提供する場所です
//作成者　森　仁志
#pragma once
/*----------------------------------------------------------------------------------------------------------------------*/

// D3D9ライブラリ
#pragma comment( lib, "d3d9")
// D3DX9ライブラリ
#pragma comment( lib, "d3dx9")
// DirectSoundライブラリ
#pragma comment( lib, "dsound")
// DirectX GUIDライブラリ
#pragma comment( lib, "dxguid")
// マルチメディアライブラリ
#pragma comment( lib, "winmm")
// デジタルビデオライブラリ
#pragma comment( lib, "vfw32")
// 音声圧縮解凍ライブラリ
//#pragma comment( lib, "msacm32")

//Windowsインクルード
#include <windows.h>
//DirectXのライブラリインクルード
#include <d3d9.h>
#include <d3dx9.h>
//数学関数をインクルード
#define _USE_MATH_DEFINES
#include <math.h>
#undef _USE_MATH_DEFINES

//仮想の入力
#include "..\input\virtual_input.h"

// コードページの警告抑止
#pragma warning( disable : 4819 )

/*----------------------------------------------------------------------------------------------------------------------*/
//マクロ域

#define _mylib_safe_free(p)\
{\
	if(p)\
	{\
		free(p);\
		(p) = nullptr;\
	}\
}
#define _mylib_safe_release(p)\
{\
	if(p)\
	{\
		(p)->Release();\
		(p) = nullptr;\
	}\
}
#define _mylib_safe_delete(p)\
{\
	if(p)\
	{\
		delete (p);\
		(p) = nullptr;\
	}\
}
#define _mylib_texture_handle_max		0x1000
#define _mylib_error_box(message)		mylib_error_box(__FILE__,__LINE__,message,"エラー",MB_OK)
/*----------------------------------------------------------------------------------------------------------------------*/
//型定義域

//合成方法
enum mylib_blend_mode_t
{
	mbm_none,
	mbm_alpha,
	mbm_add,
	mbm_factor,
	mbm_max,
};
/*----------------------------------------------------------------------------------------------------------------------*/
//関数定義域

/*
*@fn mylib_init
*@brief このライブラリの初期化
*@param instance インスタンス
*@param full_opt フルスクリーンでウィンドウを作成するオプション
*        0...確認ダイアログを出す
*        1...フルスクリーン
*        2...ウィンドウモード
*@param x,y ウィンドウモードにするときの左上位置
*@param width ウィンドウモードにするときのウィンドウ幅
*@param height ウィンドウモードにするときのウィンドウ高さ
*@param window_title ウィンドウモードにする時のウィンドウタイトル
*@param icon_filename ウィンドウの左上と最小化したときに使うアイコンのファイル名です
*@return  0...成功
 *       -1...ユーザーが続行拒否した
 *       -2...初期できない
*@detail 他の関数を使える状態にします
*        Direct3Dのデバイスとかも管理します
*        この関数を呼ぶとウィンドウを一個作成して、DirectXライブラリの初期化を行います
*        関数が失敗した場合は即座にmylib_endを呼び出したうえでアプリケーションを終了しなくてはいけません
*/
extern int mylib_init(
	HINSTANCE instance,
	int full_opt,
	int x, int y,
	int width,
	int height,
	const char *window_title,
	const char *icon_filename
);

/*
*@fn mylib_init
*@brief このライブラリの終了処理
*@detail 呼び出すと他の関数が使えなくなります
*        初期化に失敗したときとゲームを終了するときに呼び出す必要があります
*/
extern void mylib_end();

/*
*@fn mylib_process_message
*@brief メッセージ処理をします
*@return ゲームを続行するべきならtrueを返します
*/
extern bool mylib_process_message();

/*
*@fn mylib_get_frame_count
*@brief 時間を知るための関数です
*@return メッセージ処理を行う関数を呼んだ回数を返します
*/
extern time_t mylib_get_frame_count();

/*
*@fn mylib_get_instance
*@brief 初期化時に渡されたHINSTANCEをそのまま返します
*/
extern HINSTANCE mylib_get_instance();

/*
*@fn mylib_get_window
*@brief mylibが生成したウィンドウのハンドルを返します
*/
extern HWND mylib_get_window();

/*
*@fn mylib_get_d3d
*@brief 初期化済みのD3Dオブジェクトを返します
*/
extern IDirect3D9 *mylib_get_d3d();

/*
*@fn mylib_get_d3d_device
*@brief 初期化済みのD3Dデバイスを返します
*/
extern IDirect3DDevice9 *mylib_get_d3d_device();

/*
*@fn mylib_is_full_mode
*@return フルスクリーンモードで実行中ならtrue
*/
extern bool mylib_is_full_mode();

/*
*@fn mylib_input_update
*@brief 入力の更新
*/
extern void mylib_input_update();

extern const virtual_input_t *mylib_get_virtual_input_repeat();
extern const virtual_input_t *mylib_get_virtual_input_trigger();
extern const virtual_input_t *mylib_get_virtual_input_release();

/*
*@fn mylib_get_aspect
*@brief 画面のアスペクト比を取得
*/
extern float mylib_get_aspect();

/*
*@fn mylib_calc_view
*@brief セットされたカメラの情報からビュー行列を計算する
*@param view 計算結果を格納する
*@param eye カメラの位置
*@param at カメラが見ている位置
*@param up カメラの上方向
*@detail 求めたビュー行列を、mylib_set_viewを使って格納しなければいけません
*/
extern void mylib_calc_view(D3DXMATRIX *view, const D3DXVECTOR3 *eye, const D3DXVECTOR3 *at, const D3DXVECTOR3 *up);

/*
*@fn mylib_set_view
*@brief ビュー行列を設定する
*/
extern void mylib_set_view(const D3DXMATRIX *view);

/*
*@fn mylib_get_view
*@brief ビュー行列を取得する
*/
extern void mylib_get_view(D3DXMATRIX *view);

/*
*@fn mylib_calc_projection
*@brief セットされたカメラの情報からプロジェクション行列を計算する
*@param projection 計算結果を格納する
*@param fov_y 視野
*@param near_clip 近クリップ面
*@param far_clip 遠クリップ面
*@detail 求めたプロジェクション行列を、mylib_set_projectionを使って格納しなければいけません
*/
extern void mylib_calc_projection(D3DXMATRIX *projection, float fov_y, float near_clip, float far_clip);

/*
*@fn mylib_set_projection
*@brief プロジェクション行列を設定する
*/
extern void mylib_set_projection(const D3DXMATRIX *projection);

/*
*@fn mylib_get_projection
*@brief プロジェクション行列を取得する
*/
extern void mylib_get_projection(D3DXMATRIX *projection);

/*
*@fn mylib_clear
*@brief クリアします
*/
extern void mylib_clear(int red, int green, int blue);

/*
*@fn mylib_begin_scene
*@brief シーンを開始します
*@detail 成功すると0を、失敗すると0以外の値を返します
*/
extern int mylib_begin_scene();

/*
*@fn mylib_end_scene
*@brief シーンを終了します
*/
extern void mylib_end_scene();

/*
*@fn mylib_present
*@brief プレゼントします
*/
extern void mylib_present();

/*
*@fn mylib_create_texture_can_render_target
*@brief レンダーターゲットに指定可能なテクスチャを作成します
*@param texture 作成したテクスチャを受け取る
*@param width 作成するテクスチャの幅
*@param height 作成するテクスチャの高さ
*@detail todo:詳細説明を書く
*/
extern void mylib_create_texture_can_render_target(IDirect3DTexture9 **texture, int width, int height);

/*
*@fn mylib_create_depth_stencil
*@brief 深度ステンシルバッファを作成します
*@param surface 作成したサーフェスを受け取る
*@param width 作成するサーフェスの幅
*@param height 作成するサーフェスの高さ
*@detail todo:詳細説明を書く
*/
extern void mylib_create_depth_stencil(IDirect3DSurface9 **surface, int width, int height);

/*
*@fn mylib_set_render_target_surface
*@brief レンダーターゲットを切り替えます
*@param surface ターゲットに指定するサーフェス
*/
extern void mylib_set_render_target_surface(IDirect3DSurface9 *surface);

/*
*@fn mylib_set_render_target_texture
*@brief レンダーターゲットをテクスチャに切り替えます
*@param texture ターゲットに指定するテクスチャ
*@detail 実際のレンダーターゲットはテクスチャから取得されるサーフェスになります
*/
extern void mylib_set_render_target_texture(IDirect3DTexture9 *texture);

/*
*@fn mylib_set_depth_stencil
*@brief 深度ステンシルバッファを切り替えます
*@param surface ターゲットに指定するサーフェス
*/
extern void mylib_set_depth_stencil(IDirect3DSurface9 *surface);

/*
*@fn mylib_set_render_target_manage
*@brief レンダーターゲットをライブラリ管理下のものに切り替えます
*@param target ターゲットを指定します
*@detail 1を指定するとテクスチャをターゲットに設定し、0だとバックバッファをターゲットにします
*/
extern void mylib_set_render_target_manage(int target);

/*
*@fn mylib_get_render_target
*@brief レンダーターゲットを取得します
*@param surface 受け取るサーフェス
*/
extern void mylib_get_render_target(IDirect3DSurface9 **surface);

/*
*@fn mylib_get_depth_stencil
*@brief 深度ステンシルバッファを取得します
*@param surface 受け取るサーフェス
*/
extern void mylib_get_depth_stencil(IDirect3DSurface9 **surface);

/*
*@fn mylib_get_rendered_texture
*@brief レンダーターゲットとして使っているテクスチャを取得します
*@detail この関数の返り値はmylib_change_render_target関数に1を渡した時に設定されるテクスチャと同等です
*/
extern IDirect3DTexture9 *mylib_get_rendered_texture();

/*
*@fn mylib_set_fvf
*@brief Direct3Dのデバイスに対して、FVFの設定を行います
*@param fvf フォーマット
*/
extern void mylib_set_fvf(DWORD fvf);

/*
*@fn mylib_set_blend_mode
*@brief Direct3Dのデバイスに対して、ブレンドモードの指定を行います
*@param mode ブレンドモード
*/
extern void mylib_set_blend_mode(mylib_blend_mode_t mode);

/*
*@fn mylib_get_blend_mode
*@brief 現在のブレンドモードを取得します
*@param mode 格納する変数
*/
extern void mylib_get_blend_mode(mylib_blend_mode_t *mode);

/*
*@fn mylib_set_z_write
*@brief Direct3Dのデバイスに対して、Zバッファへの書き込みをするか指定します
*@param value この値がtrueならZバッファへの書き込みをします
*/
extern void mylib_set_z_write(bool value);

/*
*@fn mylib_get_z_write
*@brief Zバッファへの書き込みを行うか調べます
*@param value 結果が返されます。trueならZバッファへの書き込みをします
*/
extern void mylib_get_z_write(bool *value);

/*
*@fn mylib_set_z_check
*@brief Direct3Dのデバイスに対して、Zバッファへテストを行うか指定します
*@param value この値がtrueならZバッファへテストを行います
*/
extern void mylib_set_z_check(bool value);

/*
*@fn mylib_get_z_check
*@brief Zバッファへテストを行うか取得します
*@param value 結果が返されます。trueならZバッファへテストを行います
*/
extern void mylib_get_z_check(bool *value);

/*
*@fn mylib_set_cull_mode
*@brief カリングの設定をする
*/
extern void mylib_set_cull_mode(D3DCULL mode);

/*
*@fn mylib_get_cull_mode
*@brief 現在のカリングの設定を取得する
*/
extern void mylib_get_cull_mode(D3DCULL *mode);

/*
*@fn mylib_set_lighting
*@brief ライティングの設定を指定する
*/
extern void mylib_set_lighting(bool value);

/*
*@fn mylib_get_lighting
*@brief ライティングの設定を取得する
*/
extern void mylib_get_lighting(bool *value);

/*
*@fn mylib_shader_open
*@brief シェーダーファイルを開く
*@return 0...成功、-1...失敗
*/
extern int mylib_shader_open(const char* filename, ID3DXEffect **shader);

/*
*@fn mylib_shader_error_output
*@brief シェーダーファイルを開いたときのエラー情報を出力する
*/
extern void mylib_shader_error_message(ID3DXBuffer *compile_errors);

/*
*@fn mylib_load_texture
*@brief 画像を読み込む
*@param filename パスを含めたファイル名です
*@param handle 読み込んだ画像のハンドルを受け取る
*@param image_info 読み込んだ画像の詳細情報、nullを指定すると受け取らない
*@param error_code エラー情報を受け取る、nullを指定すると受け取らない
*@detail この関数で読み込んだ画像はライブラリの中で管理されます
*        読み込み失敗時にはhandleに-1が格納され
*        error_codeに格納される値の一覧です↓
*         0 読み込みできた
*        -1 ファイルが見つからない
*        -2 読み込みに失敗した
*/
extern void mylib_load_texture(const char *filename, int *handle, D3DXIMAGE_INFO *image_info, int *error_code);

/*
*@fn mylib_get_texture
*@brief 読み込んだ画像を取得する
*@param handle 画像のハンドル
*@param image_info 画像の情報を受け取る、nullを指定すると受け取らない
*@return テクスチャ
*/
extern IDirect3DTexture9 *mylib_get_texture(int handle, D3DXIMAGE_INFO *image_info);

/*
*@fn mylib_destroy_texture
*@brief 読み込んだ画像を全部破棄
*/
extern void mylib_destroy_texture();

//マウスカーソルを隠す関数です
extern void mylib_hide_mouse_cursor();

//マウスカーソルを指定した矩形の中に閉じ込める関数です
extern void mylib_confine_mouse_cursor(RECT *region);

//指定されたファイルが存在するならtrueを返す
extern bool mylib_file_is_exist(const char *file_name);

//フレーム単位から秒単位に変換する
extern float mylib_frame_to_second(time_t frame, float fps);

//ワールド変換を行う
extern void mylib_world_transform(D3DXMATRIX *world, const D3DXVECTOR3 *position, const D3DXVECTOR3 *rotation, const D3DXVECTOR3 *scale);

/*
*@fn mylib_make_billboard_matrix
*@brief 指定の行列の回転を打ち消す行列を作成
*@param out 計算結果を受け取ります
*@param in 計算のもとになる行列です
*/
extern void mylib_make_billboard_matrix(D3DXMATRIX *out, const D3DXMATRIX *in);

/*
*@fn mylib_get_vpv
*@brief ワールド座標からスクリーン座標への変換をする行列を取得します
*@param vpv 結果を受け取る変数
*@detail この関数はビュー行列とプロジェクション行列とビューポート行列を掛け算した結果を返します
*/
extern void mylib_get_vpv(D3DXMATRIX *vpv);

/*
*@fn mylib_to_world_pos
*@brief スクリーン座標に対応するワールド座標を取得します
*@param world_pos 結果を受け取る変数
*@param screen_pos ワールド座標
*/
extern void mylib_to_world_pos(D3DXVECTOR3 *world_pos, const D3DXVECTOR3 *screen_pos);

/*
*@fn mylib_to_screen_pos
*@brief ワールド座標に対応するスクリーン座標を取得します
*@param world_pos 結果を受け取る変数
*@param screen_pos スクリーン座標(左上(-1, 1),右下(1, -1))
*/
extern void mylib_to_screen_pos(D3DXVECTOR3 *screen_pos, const D3DXVECTOR3 *world_pos);

/*
*@fn mylib_sprintf_1024
*@brief printfと同じ書式で1024文字以下の文字列を作成します
*@param format 書式
*@param ... いろいろ
*@return 作成した文字列
*/
extern const char *mylib_sprintf_1024(const char *format, ...);

/*
*@fn mylib_now_sec
*@brief 現在秒
*/
extern float mylib_now_sec();

/*
*@fn mylib_wait_sec
*@param 待ちたい時間(秒)
*@brief 指定時間だけ全ての処理を止める
*/
extern void mylib_wait_sec(float sec);

/*
*@fn mylib_draw_string
*@brief 文字を描画します
*@param x 描画する位置(左端)
*@param y 描画する位置(上端)
*@param string 描画する文字列
*@param color 描画する色
*/
extern void mylib_draw_string(int x, int y, const char *string, D3DCOLOR color);

/*
*@fn mylib_draw_ray
*@brief レイの描画
*@param start			レイの開始地点
*@param vector			レイの終了地点までのベクトル
*@param color			色
*@return 無し
*/
extern void mylib_draw_ray(const D3DXVECTOR3 *start_position, const D3DXVECTOR3 *vector, DWORD color);

/*
*@fn mylib_draw_sphere
*@brief 球の描画
*@param position		球の中心位置
*@param radius			球の半径
*@param color			色
*@return 無し
*/
extern void mylib_draw_sphere(const D3DXVECTOR3 *position, float radius, DWORD color);

/*
*@fn mylib_draw_board
*@brief 3D空間上にポリゴンを二枚描画する関数です
*@param world ワールド行列
*@param uv0,color0 左上頂点の情報です
*@param uv1,color1 右上頂点の情報です
*@param uv2,color2 左下頂点の情報です
*@param uv3,color3 右下頂点の情報です
*@param *texture ポリゴンに貼り付けるテクスチャです
*@param blend_mode ブレンド方法を指定します(0...アルファブレンディング, 1...加算ブレンディング)
*@detail この関数は3次元空間上に四角形のポリゴンを配置するのに適しています
*/
extern void mylib_draw_board_3d(
	const D3DXMATRIX *world,
	const D3DXVECTOR2 *uv0, D3DCOLOR color0,
	const D3DXVECTOR2 *uv1, D3DCOLOR color1,
	const D3DXVECTOR2 *uv2, D3DCOLOR color2,
	const D3DXVECTOR2 *uv3, D3DCOLOR color3,
	IDirect3DTexture9 *texture, mylib_blend_mode_t blend_mode
);

/*
*@fn mylib_draw_board
*@brief スクリーン上にポリゴンを二枚描画する関数です
*@param position0,uv0,color0 左上頂点の情報です
*@param position1,uv1,color1 右上頂点の情報です
*@param position2,uv2,color2 左下頂点の情報です
*@param position3,uv3,color3 右下頂点の情報です
*@param *texture ポリゴンに貼り付けるテクスチャです
*@param blend_mode ブレンド方法を指定します
*@detail この関数は渡された頂点がそのままスクリーン座標とみなされるので、2次元の描画に適しています(多分)
*/
extern void mylib_draw_board_2d(
	const D3DXVECTOR3 *position0, const D3DXVECTOR2 *uv0, D3DCOLOR color0,
	const D3DXVECTOR3 *position1, const D3DXVECTOR2 *uv1, D3DCOLOR color1,
	const D3DXVECTOR3 *position2, const D3DXVECTOR2 *uv2, D3DCOLOR color2,
	const D3DXVECTOR3 *position3, const D3DXVECTOR2 *uv3, D3DCOLOR color3,
	IDirect3DTexture9 *texture, mylib_blend_mode_t blend_mode
);

/*
*@fn mylib_draw_board_post
*@brief なみなみポストエフェクトをかけて3D空間上にポリゴンを二枚描画する関数です
*@param mirage_texture 蜃気楼テクスチャ
*@param position0,uv0,color0 左上頂点の情報です
*@param position1,uv1,color1 右上頂点の情報です
*@param position2,uv2,color2 左下頂点の情報です
*@param position3,uv3,color3 右下頂点の情報です
*@param *texture ポリゴンに貼り付けるテクスチャです
*/
extern void mylib_draw_board_2d_wave(IDirect3DTexture9 *mirage_texture,
	const D3DXVECTOR3 *position0, const D3DXVECTOR2 *uv0, D3DCOLOR color0,
	const D3DXVECTOR3 *position1, const D3DXVECTOR2 *uv1, D3DCOLOR color1,
	const D3DXVECTOR3 *position2, const D3DXVECTOR2 *uv2, D3DCOLOR color2,
	const D3DXVECTOR3 *position3, const D3DXVECTOR2 *uv3, D3DCOLOR color3,
	IDirect3DTexture9 *texture
);

/*
*@fn mylib_draw_point_sprite
*@brief ポイントスプライトを描画する関数です
*@param pos 位置
*@param rot 回転角度
*@param scale 拡大倍率
*@param color 色
*@param uv0 左上頂点のUV座標
*@param uv1 右上頂点のUV座標
*@param uv2 左下頂点のUV座標
*@param uv3 右下頂点のUV座標
*@param texture テクスチャ
*@param blend_mode ブレンド方法
*/
extern void mylib_draw_point_sprite(
	const D3DXVECTOR2 *pos,
	float rot,
	const D3DXVECTOR2 *scale,
	D3DCOLOR color,
	const D3DXVECTOR2 *uv0,
	const D3DXVECTOR2 *uv1,
	const D3DXVECTOR2 *uv2,
	const D3DXVECTOR2 *uv3,
	IDirect3DTexture9 *texture,
	mylib_blend_mode_t blend_mode
);

/*
*@fn
*@brief ビルボードを描画する
*@param pos 位置
*@param rot 回転角度
*@param scale 拡大倍率
*@param color 色
*@param uv0 左上頂点のUV座標
*@param uv1 右上頂点のUV座標
*@param uv2 左下頂点のUV座標
*@param uv3 右下頂点のUV座標
*@param texture テクスチャ
*@param blend_mode ブレンド方法
*/
extern void mylib_draw_billboard(
	const D3DXVECTOR3 *pos,
	const float rot,
	const D3DXVECTOR2 *scale,
	D3DCOLOR color,
	const D3DXVECTOR2 *uv0,
	const D3DXVECTOR2 *uv1,
	const D3DXVECTOR2 *uv2,
	const D3DXVECTOR2 *uv3,
	IDirect3DTexture9 *texture,
	mylib_blend_mode_t blend_mode
);

/*
*@fn
*@brief ビルボードを描画する(その2)
*@param pos 位置
*@param rot 回転角度
*@param scale 拡大倍率
*@param color 色
*@param uv0 左上頂点のUV座標
*@param uv1 右上頂点のUV座標
*@param uv2 左下頂点のUV座標
*@param uv3 右下頂点のUV座標
*@param texture テクスチャ
*@detail ブレンドモードが指定不可能
         きらきらの光を描画するために存在する
*/
extern void mylib_draw_billboard2(
	const D3DXVECTOR3 *pos,
	const float rot,
	const D3DXVECTOR2 *scale,
	D3DCOLOR color,
	const D3DXVECTOR2 *uv0,
	const D3DXVECTOR2 *uv1,
	const D3DXVECTOR2 *uv2,
	const D3DXVECTOR2 *uv3,
	IDirect3DTexture9 *texture
);

/*
*@fn mylib_error_box
*@brief エラーが発生したときに使う専用のメッセージボックスです
*@param file ファイル名
*@param line 行番号
*@param message エラーメッセージ
*@param title タイトル
*@param mb_mode モード
*/
extern void mylib_error_box(const char *file, int line, const char *message, const char *title, UINT mb_mode);

/*
*@fn mylib_register_debug_info
*@brief printfと同じ書式でデバッグ用文字列を作成し、それをデバッグ文字列リストに登録します
*@param format 書式
*@param ... いろいろ
*@detail デバッグ文字列リストに登録された文字列は、mylib_output_debug_string()を呼と描画できます
*/
extern void mylib_register_debug_string(const char *format, ...);

/*
*@fnmylib_output_debug_string
*@brief デバッグ文字列リストに登録された文字列をすべて描画します
*/
extern void mylib_output_debug_string();
/*----------------------------------------------------------------------------------------------------------------------*/
