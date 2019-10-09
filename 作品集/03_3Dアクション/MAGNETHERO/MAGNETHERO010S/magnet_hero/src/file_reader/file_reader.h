#pragma once
//テキストファイルから情報を読み取る関数が書かれたヘッダーファイルです
//作成者　森　仁志
//更新履歴
//2019/01/02...ファイルを作成

//ファイルから読み取る変数
struct datum_t
{
	double _Double;
	float _Float;
	int _Int;
	char *_String;
	size_t _StringSize;
	bool _Bool;
	char _padding[7];
};

//与えられた文字が空白か制御文字のとき、真を返し、そうでない場合は、偽を返す
extern bool IsSpaceOrControl(char c);

//n文字目がダブルクォーテーションであればtrueを返す、ただし、n-1文字目が"\"であればfalseを返す
extern bool IsDoubleQuotation(const char *String, size_t n);

//文字列の中から"//"と"\n"で囲まれた部分、または、"/*"と"*/"で囲まれた部分を削除する
//ただし、ダブルクォーテーションで囲まれた文字は無視する
//例:"123/*456*/789//ABC\nDEF\"//000\n\""->"123789DEF\"//000\n\""
extern void DeleteComment(char *String, size_t StringSize);

//文字列の中から、タブや改行などの制御文字を消す
//ただし、ダブルクォーテーションで囲まれた文字は無視する
extern void DeleteControl(char *String, size_t StringSize);

//文字列の中から、ダブルクォーテーションを消す
//ただし、ひとつ前の文字が"\\"であれば、消さない
extern void DeleteDoubleQuotation(char *String, size_t StringSize);

//文字列の中に含まれるコンマの数を調べる
//ただし、ダブルクォーテーションで囲まれた文字は無視する
extern void CountComma(const char *String, size_t StringSize, size_t *CommaNum);

//文字列の先頭から任意の文字と一致する文字を探す
//ただし、ダブルクォーテーションで囲まれた文字は無視する
//見つからなかった場合はnullが返る
extern char *SearchStringChar(char *String, char Target);

//文字列から、変数に変換
extern void CreateVariableFromString(const char *String, size_t StringSize, datum_t *Converted);

//文字列から、コンマ区切りの変数を読み込んで配列を作る(読み込み部分)
extern void CreateVariableArrayWithReadString(const char *String, size_t StringSize, datum_t *VariableArray, size_t VariableArraySize);

//文字列から、コンマ区切りの変数を読み込んで配列を作る(準備部分)
extern void CreateVariableArrayFromString(const char *String, size_t StringSize, datum_t **VariableArray, size_t *VariableArraySize);

//ファイルから、コンマ区切りの変数を読み込んで配列を作る
extern void CreateVariableArrayFromFile(const char *FileName, datum_t **VariableArray, size_t *VariableArraySize);

//配列を解放する
extern void DestroyVariableArray(datum_t **VariableArray, size_t *VariableArraySize);