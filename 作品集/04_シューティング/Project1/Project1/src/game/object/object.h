#pragma once
#include <stdlib.h>
#include "object_kind.h"
#include "primitive/vector2d.h"
#include "primitive/keyboard.h"
#include "looks.h"

#define ObjectCapacity	100

struct Object
{
	ObjectKind kind;		//種類
	Vector2D pos;			//座標
	LooksID looks_id;		//見た目
	void *kind_info;		//種類ごとの追加情報
};

//オブジェクト初期化
extern int InitObject();

//オブジェクト終了
extern int EndObject();

//オブジェクトを取得
extern Object *GetObject(int index);

//オブジェクトリストの先頭を取得
extern Object *BeginObject();

//オブジェクトリストの末尾の次の要素を取得
extern Object *OutObject();

//オブジェクトリストの次の要素を取得
//末尾判定にはOutObjectを使う
extern Object *NextObject(Object *obj);

//オブジェクトを出す
extern Object *SetObject(ObjectKind kind, Vector2D pos, LooksID looks_id, void *kind_info);

//オブジェクトを消す
extern int ClearObject(Object *obj);

//オブジェクト全消し
extern void ClearAllObject();

//オブジェクト更新
extern void UpdateObject(const Keyboard *key);

//オブジェクト描画
extern void DrawObject();

//オブジェクトと矩形が衝突しているかチェック
extern bool IsHitObjectRect(const Object *obj, Rect hit_rect);

//自分と衝突しているオブジェクトを取得
extern Object *HitObject(const Object *obj, Object *offset, bool ignore_self);

//オブジェクトの種類ごとの数を得る
extern size_t GetObjectKindNum(ObjectKind kind);

//オブジェクトの数を得る
extern size_t GetObjectNum();
