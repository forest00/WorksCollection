//	
//	アニメーション付きメッシュの読み込みと破棄、描画とアニメ再生を担当するファイルです
//	
//	ここはほぼ全てコピペです
//	
#pragma once
//必要なヘッダーファイルのインクルード
#include <windows.h>
#include <d3dx9.h>
#include <stdio.h>
#include "anim_data_loader.h"

//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
//// シンボル定義及びマクロ
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define THING_AMOUNT 1


//派生フレーム構造体 (それぞれのメッシュ用の最終ワールド行列を追加する）
struct MYFRAME : public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
};
//派生メッシュコンテナー構造体(
//コンテナーがテクスチャを複数持てるようにポインターのポインターを追加する）
struct MYMESHCONTAINER : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;
	DWORD dwWeight;//重みの個数（重みとは頂点への影響。）
	DWORD dwBoneNum;//ボーンの数
	LPD3DXBUFFER pBoneBuffer;//ボーン・テーブル
	D3DXMATRIX** ppBoneMatrix;//全てのボーンのワールド行列の先頭ポインター
	D3DXMATRIX* pBoneOffsetMatrices;//フレームとしてのボーンのワールド行列のポインター
};
//Xファイル内のアニメーション階層を読み下してくれるクラスを派生させる。
//ID3DXAllocateHierarchyは派生すること想定して設計されている。
class MY_HIERARCHY : public ID3DXAllocateHierarchy
{
public:
	MY_HIERARCHY() {}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR, D3DXFRAME **);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCTSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*,
		CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD *, LPD3DXSKININFO, LPD3DXMESHCONTAINER *);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
};

//ボーンの行列を確保(?)
extern HRESULT AllocateBoneMatrix(D3DXFRAME *pFrameRoot, LPD3DXMESHCONTAINER);

//全てのボーンに対して行列を確保(?)
//スキンメッシュに対して使うらしいです
extern HRESULT AllocateAllBoneMatrices(D3DXFRAME *pFrameRoot, D3DXFRAME *pFrame);

//フレーム内のそれぞれのメッシュをレンダリングする
extern VOID RenderMeshContainer(LPDIRECT3DDEVICE9, MYMESHCONTAINER*, MYFRAME*);

//フレームをレンダリングする。
extern VOID UpdateFrameMatrices(LPD3DXFRAME, const D3DXMATRIX*);

//フレーム内のメッシュ毎にワールド変換行列を更新する
extern VOID RenderFrame(LPDIRECT3DDEVICE9, LPD3DXFRAME);

//有効なヒエラルキーのポインタを返す
///ID3DXAllocateHierarchyのポインタに暗黙変換可能
//ID3DXAllocateHierarchyのポインタに変換済みのものを返します
//ヒエラルキーが必要な場所で使ってね
extern ID3DXAllocateHierarchy *GetValidHierarchy();

//階層内、全てのDirectXオブジェクトをリリースする
extern void FreeAnim(D3DXFRAME *pFrame, ID3DXAllocateHierarchy *cHierarchy);

///以上コピペ
///以下自作


// 物体を表すパラメーター
struct thing_parameter_t
{
	D3DXVECTOR3 _position;		//位置
	D3DXVECTOR3 _angle;			//角度
	D3DXVECTOR3 _scale;			//拡大倍率
};

//アニメーションが終わった時に繰り返し再生するか
enum anim_repeat_flag_t
{
	anim_repeat_on,				//繰り返す
	anim_repeat_off,			//止める(繰り返さない)
};

//アニメーションのパラメーター
struct anim_parameter_t
{
	anim_model_kind_t _anim_kind;					//どのアニメーションデータを参照しているか
	const anim_data_t *_data;						//参照するアニメーションデータ
	ID3DXAnimationController *_controller;			//アニメーションコントローラー
	bool _playing;									//アニメーションしているか
	float _total_time;								//アニメーション合計時間
	float _last_changed_time;						//最後に切り替わった瞬間
	int _playing_index;								//再生中のアニメの番号
	anim_repeat_flag_t _repeat_flag;				//アニメーションが終わった時に繰り返し再生するか
};


//アニメーションの初期化
void anim_init(anim_parameter_t *anim);

//アニメーションの準備
void anim_setup(anim_parameter_t *anim, anim_model_kind_t anim_kind);

//アニメーションをやめる
void anim_end(anim_parameter_t *anim);

//アニメーションを再生する
/*
*@param anim						対象のパラメーター構造体
*@param change						何番のアニメーションにするか
*@param do_at_the_end				アニメーションが終わった時に繰り返し再生するか
*@return 0...成功 -1...アニメーションコントローラーが存在しない -2...changeが範囲外である
*/
int anim_play(anim_parameter_t *anim, int change, anim_repeat_flag_t repeat_flag);

//アニメーションを進める
/*
*@param anim						対象のパラメーター構造体
*@param advance_time				進める時間
*@param *duration					アニメの一回分の再生時間を格納する
*@param *duration_nextafter0		アニメが再生され切らないぎりぎりの時間を格納する
*@param *anim_count					アニメがループされた回数を格納する
*@param *anim_position				処理後のアニメの位置を格納する
*@param *anim_prev_position			処理前のアニメの位置を格納する
*@param *anim_is_ended				アニメが再生され切らないぎりぎりの時間に達しているかを格納する(anim_position >= duration_nextafter0)
*@return 無し
*/
void anim_advance(
	anim_parameter_t *anim,
	float advance_time,
	float *duration,
	float *duration_nextafter0,
	size_t *anim_count,
	float *anim_position,
	float *anim_prev_position,
	bool *anim_is_ended
);

//アニメーションする物体を描画
/*
*@param *device			Direct3Dのデバイス
*@param *world			ワールド変換済みの行列
*@param *anim			初期化済みのアニメーションパラメーター
*@return 無し
*/
void anim_thing_draw(IDirect3DDevice9 *device, const D3DXMATRIX *world, const anim_parameter_t *anim);


