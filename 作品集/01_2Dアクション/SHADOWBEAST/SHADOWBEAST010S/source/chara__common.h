#pragma once


#include "chara__render.h"
#include "chara__common.h"
#include "chara__hit_rect.h"
#include "anima.h"
#include "type_parameter.h"
#include "common.h"
#include <Windows.h>

//define↓


#define HIT_DIR_UP		(1<<0)
#define HIT_DIR_DOWN	(1<<1)
#define HIT_DIR_LEFT	(1<<2)
#define HIT_DIR_RIGHT	(1<<3)

//define↑



typedef enum{
	ENEMY_NONE,
	ENEMY_GOBLIN,
	ENEMY_WYVERN,
	ENEMY_SNAKE,
	BOSS_TITAN,
	BOSS_SEASNAKE,
	ENEMY_TYPE_MAX,
}EnemyType;


typedef struct{
	bool locked;
}AttackerAtt;

typedef struct{
	double gravity;					///重力
	double walkSpeedBase;			///基本移動速度
	int maxHp;						///最大hp
	double jumpPowBase;				///基本ジャンプ力
	int attackBase;					///基本攻撃力
	double avoidPowBase;			///基本回避力
	int stopLimit;					///ダメージを受けて停止する時間
	int stiffnessTimeLimit;			///攻撃後硬直時間
	//ここより下はゼロで初期化される
	XYd velocity;					///現在速度
	int hp;							///現在hp
	double walkSpeed;				///移動速度
	double jumpPow;					///ジャンプ力
	int attack;						///攻撃力
	double avoidPow;				///回避力
	int state;						///現在の状態
	int beforeState;				///ひとつ前の状態
	unsigned int stateChanged;		///状態が切り替わった瞬間
	bool nextAttackFlag;			///次の攻撃に移行するフラグ
	unsigned int nextAttackCounter;	///フラグを切り替えるためのカウンター
	Parameter avoidPosX;			///回避位置
	Direction avoidDir;				///回避の方向
	double knockBackX;				///ノックバック
	double knockBackY;
	XYd stopCoord;					///停止用　位置を保存しとく
	int stopCount;					///停止用　時間を保存しとく
	bool invincibleFlag;			///無敵フラグ
	int invincibleCount;			///無敵カウンター
	int invincibleLimit;			///無敵カウンターがリミットを超えたら無敵解除
	int attackerMainAtk1;
	int attackerMainAtk2;
	int attackerMainAtk3;
	int stiffnessCount;				///攻撃後硬直カウント
	int subAtkIndex;				///サブアタック
	bool reading;					///看板読んでる
	bool alreadySubAttack;			///すでにサブアタック
	bool alreadyResurrect;			///すでに復活
	bool alreadyExpandShield;		///すでに盾
	int shieldHandle;				///盾の管理用ハンドル
	int confusionStarHandle;		///混乱星管理用ハンドル
	int inGuardTotalHitCount;		///ガード中にダメージを受けた回数
	int inGuardTotalHitDamage;		///ガード中に受けたダメージの総量
	int inGuardGuardCount;			///ガード時専用無敵制御用カウンタ
	int quakeCount;					///正の数の時、揺れる
}Player1Att;

typedef struct{
	bool isMiss;				///アニメーションが半分再生されれば失敗となる
	bool isSuccess;				///敵に当たれば成功である
	int aheadIndex;				///先端の情報
	XYd aheadPos;				///先端の座標
	double aheadSpeed;			///先端の移動速度
	int aheadStopStartFrame;	///停止するコマ
	int aheadStopEndFrame;		///停止解除するコマ
	bool aheadInProgress;		///進行中
	bool aheadRegression;		///退行中
	void *aheadTarget;
}Player1SubAtt;

typedef struct{
	double gravity;					///重力
	double walkSpeedBase;			///基本移動速度
	int maxHp;						///最大hp
	double jumpPowBase;				///基本ジャンプ力
	int attackBase;					///基本攻撃力
	double avoidPowBase;			///基本回避力
	int stopLimit;					///ダメージを受けて停止する時間
	int stiffnessTimeLimit;			///攻撃後硬直時間
	int subAtkCoolTime;				///サブアタックのクールタイム
	//ここより下はゼロで初期化される
	XYd velocity;					///現在速度
	int hp;							///現在hp
	int afterResurrection;			///復活するたびにリセットされるカウンター
	double walkSpeed;				///移動速度
	double jumpPow;					///ジャンプ力
	int attack;						///攻撃力
	double avoidPow;				///回避力
	int state;						///現在の状態
	int beforeState;				///ひとつ前の状態
	unsigned int stateChanged;		///状態が切り替わった瞬間
	Parameter avoidPosX;			///回避位置
	Direction avoidDir;				///回避の方向
	double knockBackX;				///ノックバック
	double knockBackY;
	XYd stopCoord;					///停止用　位置を保存しとく
	int stopCount;					///停止用　時間を保存しとく
	bool invincibleFlag;			///無敵フラグ
	int invincibleCount;			///無敵カウンター
	int invincibleLimit;			///無敵カウンターがリミットを超えたら無敵解除
	int stiffnessCount;				///攻撃後硬直カウント
	bool alreadyMainAtk;			///すでにメインアタック
	bool alreadySubAtk;				///すでにサブアタック
	bool alreadyRecoveryAtk;		///すでにリカバリアタック
	int subAtkCoolCounter;			///サブアタックのクールタイム　カウンター
	bool alreadyResurrect;			///すでに復活
}Player2Att;

typedef struct{
	int vx, vy;
	int attack;
	int attackerIndex;
}Player2MainAtt;

typedef struct{
	int time;					///現在時間
	int delay;					///遅延時間
	int attack;					///攻撃力
	XYd velocity;				///現在速度
	double movementAmount;		///移動量
}Player2SubAtt;

typedef struct{
	EnemyType enemyType;					///識別用(すべての敵に持たせる)
	double gravity;					///重力
	double walkSpeed;				///移動速度
	int hp;							///HP
	int attack;
	int stopLimit;					///ダメージを受けて停止する時間
	int stiffnessTimeLimit;			///攻撃後硬直時間
	double resistSword;				///剣耐性
	double resistGun;				///銃耐性
	//以降は0で初期化される
	int state;						///現在の状態
	XYd velocity;					///現在速度
	int stopCount;					///停止用　時間を保存しとく
	XYd stopCoord;					///停止用　位置を保存しとく
	double knockBackX;				///ノックバック
	double knockBackY;				///
	int stiffnessCount;				///攻撃後硬直カウント
	//パンチの変数↓
	int atkBeforeFrame;
	int atkRectIndex;
	//パンチの変数↑
	bool isDead;					///ゲーム内で死んでいる
	int deadCounter;				///死んだ瞬間からの経過時間
}GoblinAtt;

typedef struct{
	EnemyType enemyType;			///識別用(すべての敵に持たせる)
	double gravity;					///重力
	double walkSpeed;				///移動速度
	int hp;							///HP
	int attack;
	int stopLimit;					///ダメージを受けて停止する時間
	int stiffnessTimeLimit;			///攻撃後硬直時間
	double resistSword;				///剣耐性
	double resistGun;				///銃耐性
	//以降は0で初期化される
	int state;						///現在の状態
	XYd velocity;					///現在速度
	XYd stopCoord;					///停止用　位置を保存しとく
	int stopCount;					///停止用　時間を保存しとく
	double knockBackX;				///ノックバック
	double knockBackY;				///ノックバック
	Direction atkDir;
	int stiffnessCount;				///攻撃後硬直カウント
	bool isHittingDamage;			///ダメージ受け中
	bool isDead;					///ゲーム内で死んでいる
	int deadCounter;				///死んだ瞬間からの経過時間
}WyvernAtt;

typedef struct {
	bool used;			///使用中フラグ
	int time;			///経過時間
	int delay;			///遅延時間
	Direction dir;		///向き
	XYd velocity;		///速度
	int attack;			///攻撃力
	void *parrentPtr;
}WyvernFireAtt;

typedef struct{
	EnemyType enemyType;			///識別用(すべての敵に持たせる)
	double gravity;					///重力
	double walkSpeed;				///移動速度
	int hp;							///HP
	int attack;
	int stopLimit;					///ダメージを受けて停止する時間
	int stiffnessTimeLimit;			///攻撃後硬直時間
	double resistSword;				///剣耐性
	double resistGun;				///銃耐性
	//以降は0で初期化される
	int state;						///現在の状態
	XYd velocity;					///現在速度
	XYd stopCoord;					///停止用　位置を保存しとく
	int stopCount;					///停止用　時間を保存しとく
	double knockBackX;				///ノックバック
	double knockBackY;				///ノックバック
	int stiffnessCount;				///攻撃後硬直カウント
	int atkRectIndex;				///攻撃判定管理用：矩形インデックス
	int atkBeforeFrame;				///攻撃判定管理用：1F前のアニメーションコマ数
	bool isDead;					///ゲーム内で死んでいる
	int deadCounter;				///死んだ瞬間からの経過時間
}SnakeAtt;

typedef struct{
	int currentTime;	//現在時間
	int lifeTime;		//寿命
	bool toOpen;		//開き状態フラグ
	EnemyType enemy;	//エネミータイプ
}EnemyHoleAtt;

typedef struct{
	EnemyType enemyType;			///識別用(すべての敵に持たせる)
	double gravity;					///重力
	double walkSpeed;				///移動速度
	int hp;							///HP
	int attack;
	int stopLimit;					///ダメージを受けて停止する時間
	int attackWaitLimit;			///攻撃前待ち
	int stiffnessTimeLimit1;		///攻撃後硬直時間(振り下ろし)
	int stiffnessTimeLimit2;		///攻撃後硬直時間(薙ぎ払い)
	int stiffnessTimeLimit3;		///攻撃後硬直時間(突進)
	double resistSword;				///剣耐性
	double resistGun;				///銃耐性
	//以降は0で初期化される
	int state;						///現在の状態
	XYd velocity;					///現在速度
	XYd stopCoord;					///停止用　位置を保存しとく
	int stopCount;					///停止用　時間を保存しとく
	double knockBackX;				///ノックバック
	double knockBackY;				///ノックバック
	int atkControlller;				///どの攻撃をするかフラグ
	int atk1RectIndex;				///攻撃矩形出現管理用変数(振り下ろし)
	Parameter atk1X, atk1Y;			///攻撃用矩形座標管理用変数
	int atk2RectIndexR;				///攻撃矩形出現管理用変数(薙ぎ払い左)
	int atk2RectIndexL;				///攻撃矩形出現管理用変数(薙ぎ払い右)
	int atk3RectIndex;				///攻撃矩形出現管理用変数(突進)
	int damegeCount;				///ダメージ受けた後のカウンター
	int attackWaitCount;			///攻撃前カウント
	int stiffnessCount;				///攻撃後硬直カウント
	int quakeCount;					///正の数の時、揺れる
	bool isDead;					///ゲーム内で死んでいる
	int deadCounter;				///死んだ瞬間からの経過時間
}TitanAtt;

typedef struct{
	EnemyType enemyType;			///識別用(すべての敵に持たせる)
	double garavity;				///重力
	double walkSpeed;				///移動速度
	int hp;							///HP
	int attackBite;					///噛みつきのダメージ
	int attackThunder;				///雷のダメージ
	int attackWater;				///水のダメージ
	int damageStopTime;				///ダメージを受けて停止する時間
	int bitePreStiffnessTime;		///噛みつきアニメーション前準備時間
	int bitePostStiffnessTime;		///噛みつきアニメーション後硬直時間
	int biteAtkStartFrame;			///噛みつき攻撃開始コマ(当たり判定開始)
	int biteAtkEndFrame;			///噛みつき攻撃終了コマ(当たり判定終了)
	int biteAnimDuration;			///噛みつき攻撃アニメーション時間
	int thunderPreStiffnessTime;	///雷アニメーション前準備時間
	int thunderPostStiffnessTime;	///雷アニメーション後硬直時間
	int thunderAtkStartFrame;		///雷攻撃開始コマ(当たり判定開始)
	int thunderAtkEndFrame;			///雷攻撃終了コマ(当たり判定終了)
	int thunderAnimDuration;		///雷攻撃アニメーション時間
	int waterPreStiffnessTime;		///水アニメーション前準備時間
	int waterPostStiffnessTime;		///水アニメーション後硬直時間
	int waterAtkStartFrame;			///水攻撃開始コマ(当たり判定開始)
	int waterAtkEndFrame;			///水攻撃終了コマ(当たり判定終了)
	int waterAnimDuration;			///水攻撃アニメーション時間
	double resistSword;				///剣耐性
	double resistGun;				///銃耐性
	//以降は0で初期化される
	int state;						///現在の状態
	XYd velocity;					///現在速度
	int damageStopCounter;			///ダメージ硬直用カウンター
	XYd stopCoord;					///ダメージを受けた時点での位置
	int stiffnessCounter;			///硬直カウンター
	int biteAtkIndex;				///攻撃矩形出現管理用変数(噛みつき)
	double knockBackX;				///ノックバック
	double knockBackY;				///ノックバック
	int atkControlller;				///どの攻撃をするかフラグ
	int animStopCounter;			///アニメ停止カウンター
	bool alreadyThunder;			///すでに雷
	int thunderOffsetX[3];			///どの位置に雷を落とすか(X座標オフセット)　注意：マジックナンバー
	bool alreadyPlaySound;			///すでに音再生
	int quakeCount;					///正の数の時、揺れる
	bool isDead;					///ゲーム内で死んでいる
	int deadCounter;				///死んだ瞬間からの経過時間
}SeaSnakeAtt;

typedef struct{
	int duration;			//継続時間
	int elapsed;			//経過時間
	int delay;				//遅延
	int atkStartTime;		//攻撃開始
	int atkEndTime;			//攻撃終わり
	int attack;				//攻撃力
}ThunderAtt;

typedef struct{
	XYd velocity;
	int attack;
}WaterAtt;

typedef struct{
	int maxHp;						///最大hp
	int hp;							///hp
	bool aliveFlag;						///生存フラグ
	bool invincibleFlag;			///無敵フラグ
	int invincibleCount;			///無敵カウンター
	int invincibleLimit;			///無敵カウンターがリミットを超えたら無敵解除
}CastleAtt;

typedef struct{
	XYd velocity;					///現在速度
}ItemAtt;

typedef struct{
	double offsetX;			///座標オフセットx
	double offsetY;			///座標オフセットy
	Parameter x, y;			///座標
	Parameter scale;		///拡大倍率
	Parameter angle;		///角度
	Parameter alpha;		///透明度
	int red, green, blue;	///赤緑青成分
	int currentTime;		///現在時間
	int lifeTime;			///寿命
	int blend;				///ブレンド方法
	bool shouldAnim;		///アニメーションすべきか
	bool flipFlag;			///反転フラグ
}EasingEffectAtt;

typedef struct{
	XYd velocity;			///現在速度
	Parameter alpha;		///透明度
	int red, green, blue;	///赤緑青成分
	int currentTime;		///現在時間
	int lifeTime;			///寿命
	int blend;				///ブレンド方法
}FireEffectAtt;

typedef union{
	AttackerAtt atk;
	Player1Att pl1;
	Player1SubAtt pl1Sub;
	Player2Att pl2;
	Player2MainAtt pl2Main;
	Player2SubAtt pl2Sub;
	GoblinAtt gob;
	WyvernAtt wyv;
	WyvernFireAtt wyvFire;
	SnakeAtt snk;
	EnemyHoleAtt hole;
	TitanAtt titan;
	SeaSnakeAtt seaSnk;
	ThunderAtt thunder;
	WaterAtt water;
	CastleAtt cstl;
	EasingEffectAtt easingEfc;
	FireEffectAtt fireEfc;
	ItemAtt item;
	int blkHitDir;
}CharaAtt;




//Character型の構造体を用意


typedef struct tagCharacter {
	//アクティブフラグ(アクティブ時:1)
	bool isActive;
	//移動用
	XYd beforePos;
	XYd pos;
	Direction dir;
	//当たり判定用
	Box hit;
	//画像決定用
	ImageFileId imageFileId;
	//アニメーション用
	Anima anim;
	//描画用
	DrawInfo draw;
	//実際の動作用
	int(*update)(tagCharacter*);
	//その他、足りない情報
	CharaAtt att;
}Character, *pCharacter;






//初期化用


/*
キャラ単体を初期化する
メンバー全てに対して、初期状態にふさわしい値を代入する
@param target 初期化する対象のキャラ
@return
0...成功
1...失敗
*/
int  charaInit(pCharacter target);

/*
キャラの配列を初期化する
配列の全要素に対して、初期状態にふさわしい値を代入する
@param characters 初期化したい配列の先頭アドレス 
@param num 配列の要素数
@return
0...成功
1...失敗
*/
int  charaBulkInit(Character characters[], int num);






//出現消滅管理用

/*
キャラの配列の全要素の中から、アクティブな要素を探す(先頭から)
@param characters 対象の配列
@param num 配列の要素数
@return アクティブな要素の要素番号
*/
int charaFindActive(Character characters[], int num);

/*
キャラの配列の全要素の中から、アクティブでない要素を探す(先頭から)
@param characters 対象の配列
@param num 配列の要素数
@return アクティブでない要素の要素番号
*/
int charaFindNonActive(Character characters[], int num);

/*
キャラ配列の中から、アクティブでない要素を探し、それを初期化し、要素番号を返す
@param characters 対象の配列
@param num 配列の要素数
@return
関数が成功した場合、操作を行った要素のインデックス
失敗した場合、-1
*/
int charaSet(Character characters[], int num);

/*
指定したキャラをアクティブでなくす
@param target 操作対象のキャラ
@return
0...成功
1...失敗
*/
int charaClear(pCharacter target);

/*
指定したキャラ配列の全要素をアクティブでなくす
@param characters 対象の配列
@param num 配列の要素数
@return
0...成功
1...失敗
*/
int charaBulkClear(Character characters[], int num);




//各種情報格納用

/*
キャラの表示用のサイズを指定する
@param target 対象のキャラ
@param sizeX x方向のサイズ
@param sizeY y方向のサイズ
@return
0...成功
1...失敗
*/
int charaSetDispSize(pCharacter target, int sizeX, int sizeY);
/*
キャラの表示用のサイズを指定する
@param target 対象のキャラ
@param ImageFileId 画像データベース上の位置
@return
0...成功
1...失敗
*/
int charaSetDispSize(pCharacter target, ImageFileId);
/*
キャラの表示用のサイズを指定する
@param target 対象のキャラ
@param CharaHitAreaId 矩形当たり判定用データベース上の位置
@return
0...成功
1...失敗
*/
int charaSetDispSize(pCharacter target, CharaHitAreaId);

/*
キャラのワールド座標を指定する
変更前の座標を保存しているので、移動などをしたい場合はこの関数を使うこと
@param target 対象のキャラ
@param x ワールド座標x
@param y ワールド座標y
@return
0...成功
1...失敗
*/
int charaOffSet(pCharacter target, double x, double y);

/*
キャラの当たり判定領域をデータベースから指定する
@param target 対象のキャラ
@param id データベース上の位置
@return
0...成功
1...失敗
*/
int charaColSet(pCharacter target, CharaHitAreaId id);

/*
キャラの当たり判定領域を直接指定する
@param target 対象のキャラ
@param x 当たり判定オフセットx
@param y 当たり判定オフセットy
@param w 当たり判定横幅
@param h 当たり判定縦幅
@return
0...成功
1...失敗
*/
int charaColSet(pCharacter target, int x, int y, int w, int h);

/*
キャラの移動・回転・拡大縮小の原点を指定する
@param target 対象のキャラ
@param cenX 中心位置x
@param cenY 中心位置y
@return
0...成功
1...失敗
*/
int charaHome(pCharacter target, int cenX, int cenY);

/*
キャラを拡大・縮小させる
@param target 対象のキャラ
@param scaleX 拡大倍率x
@param scaleY 拡大倍率y
@return
0...成功
1...失敗
*/
int charaScale(pCharacter target, double scaleX, double scaleY);

/*
キャラを回転させる
@param target 対象のキャラ
@param angle 回転角度(ラジアン)
@return
0...成功
1...失敗
*/
int charaRot(pCharacter target, double angle);

/*
キャラのブレンド方法および表示色を指定する
@param target 対象のキャラ
@param blend ブレンド方法(amganeにより定義されている)
@param alpha 透明度
@param red 赤成分(0~255)
@param green 緑成分(0~255)
@param blue 青成分(0~255)
@return
0...成功
1...失敗
*/
int charaColor(pCharacter target, int blend, int alpha,
			   int red, int green, int blue);

/*
キャラの表示用画像ハンドルを指定する
@param target 対象のキャラ
@param handle 画像ハンドル
@return
0...成功
1...失敗
*/
int charaSetImageHandle(pCharacter target, int handle);

/*
キャラを横反転させるかどうかフラグを設定
@param target 対象のキャラ
@param flagX フラグ(trueなら横反転)
@return
0...成功
1...失敗
*/
int charaFlipX(pCharacter target, bool flagX);

/*
キャラを更新する関数を指定
@param target 対象のキャラ
@param update キャラ更新時に呼ばれるコールバック関数
@return
0...成功
1...失敗
*/
int charaFuncSet(pCharacter target, int(*update)(pCharacter));

//各種情報取得用

bool getCharaExist(const pCharacter target);

int getCharaOffSet(const pCharacter target, double *x, double *y);

int getCharaHitArea(const pCharacter target, double *left, double *top, double *right, double *bottom);

int getCharaHitAreaCenter(const pCharacter target, double *centerX, double *centerY);

bool getCharaFlipFlag(const pCharacter target);







//当たり判定用

int createBoxForHit(const pCharacter chara, pRect out);
int createBoxForHit(const pCharacter chara, CharaHitAreaId id, pRect out);

bool charaHit(pCharacter target, pCharacter opponent);

bool charaHitBox(pCharacter target, int left, int top, int right, int bottom);
bool charaHitBox(pCharacter target, int x, int y, const pBox in);
bool charaHitBox(pCharacter target, int x, int y, const Box *in);
bool charaHitBox(pCharacter target, int x, int y, CharaHitAreaId id);
bool charaHitBox(pCharacter target, const pRect in);
bool charaHitBox(pCharacter target, const Rect *in);

int charaBulkHit(Character characters[], int num, pCharacter target);
int charaBulkHit(Character characters[], int num, pCharacter target, CharaHitAreaId id);
int charaBulkHit(Character characters[], int num, int x, int y, CharaHitAreaId id);

int charaCorrect(pCharacter chara, pCharacter subject, int bFlag);

bool charaLanding(int hitInfo);


//実際の動作用

/*
キャラ配列中のアクティブな要素に対して、更新処理を行う
@param characters 対象の配列
@param num 配列の要素数
@return
0...成功
1...失敗
*/
int charaBulkUpdate(Character characters[], int num);




//描画用

/*
キャラ単体を描画する
@param target 対象のキャラ
@return
0...成功
1...失敗
*/
int charaDraw(pCharacter target);

/*
キャラ配列中のアクティブな要素を全て描画する
@param characters 対象の配列
@param num 配列の要素数
@return
0...成功
1...失敗
*/
int charaBulkDraw(Character characters[], int num);


