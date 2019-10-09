//親ヘッダー
#include "file_reader.h"
//その他のヘッダー
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "..\char_ptr\char_ptr.h"

//与えられた文字が空白か制御文字のとき、真(0以外)を返し、そうでない場合は、偽(0)を返す
extern bool IsSpaceOrControl(char c)
{
	if(c == ' ')
	{
		return true;
	}
	if(c >= -1 && c <= 255)
	{
		if(iscntrl(c))
		{
			return true;
		}
	}
	return false;
}

//n文字目がダブルクォーテーションであればtrueを返す、ただし、n-1文字目が"\"であればfalseを返す
extern bool IsDoubleQuotation(const char *String, size_t n)
{
	if(String == nullptr)
	{
		return false;
	}
	if(String[n] == '\"')
	{
		if(n == 0)
		{
			return true;
		}
		if(String[n - 1] != '\\')
		{
			return true;
		}
	}
	return false;
}

//文字列の中から"//"と"\n"で囲まれた部分、または、"/*"と"*/"で囲まれた部分を削除する
//ただし、ダブルクォーテーションで囲まれた文字は無視する
//例:"123/*456*/789//ABC\nDEF\"//000\n\""->"123789DEF\"//000\n\""
extern void DeleteComment(char *String, size_t StringSize)
{
	size_t DestinationCount = 0;
	bool InString = false;
	bool InComment = false;
	bool ShouldIgnore = false;
	for(size_t i = 0; i < StringSize - 1 && String[i] != '\0'; i++)
	{
		if(InComment == false)
		{
			if(IsDoubleQuotation(String, i))
			{
				InString = InString ? false : true;
			}
		}
		if(InString == false)
		{
			if(InComment)
			{
				//行コメントの終わりを判定
				bool EndOfLineComment = String[i] == '\n';
				//範囲コメントの終わりを判定
				bool EndOfRangeComment = String[i] == '*' && String[i + 1] == '/';
				if(EndOfLineComment || EndOfRangeComment)
				{
					//範囲コメントならループカウンターを進める
					if(EndOfRangeComment)
					{
						i++;
					}
					//コメントの中ではなくなる
					InComment = false;
					//無視しないといけないのでこのループは終了
					continue;
				}
			}
			else
			{
				//行コメントの始まりを判定
				bool StartOfLineComment = String[i] == '/' && String[i + 1] == '/';
				//範囲コメントの始まりを判定
				bool StartOfRangeComment = String[i] == '/' && String[i + 1] == '*';
				if(StartOfLineComment || StartOfRangeComment)
				{
					//一度に二つの文字を無視するのでループカウンターを進めておく
					i++;
					//コメントの中になる
					InComment = true;
				}
			}
		}
		ShouldIgnore = (InString == false) && InComment;
		if(ShouldIgnore == false)
		{
			String[DestinationCount++] = String[i];
		}
	}
	String[DestinationCount] = '\0';
}

//文字列の中から、タブや改行などの制御文字を消す
//ただし、ダブルクォーテーションで囲まれた文字は無視する
extern void DeleteControl(char *String, size_t StringSize)
{
	size_t DestinationCount = 0;
	bool InString = false;
	bool ShouldIgnore = false;
	for(size_t i = 0; i < StringSize - 1 && String[i] != '\0'; i++)
	{
		if(IsDoubleQuotation(String, i))
		{
			InString = InString ? false : true;
		}
		ShouldIgnore = (InString == false) && IsSpaceOrControl(String[i]);
		if(ShouldIgnore == false)
		{
			String[DestinationCount++] = String[i];
		}
	}
	String[DestinationCount] = '\0';
}

//文字列の中から、ダブルクォーテーションを消す
//ただし、ひとつ前の文字が"\\"であれば、消さない
extern void DeleteDoubleQuotation(char *String, size_t StringSize)
{
	size_t DestinationCount = 0;
	for(size_t i = 0; i < StringSize - 1 && String[i] != '\0'; i++)
	{
		if(String[i] == '\\')
		{
			if(String[i + 1] == '\"')
			{
				continue;
			}
		}
		if(IsDoubleQuotation(String, i))
		{
			continue;
		}
		String[DestinationCount++] = String[i];
	}
	String[DestinationCount] = '\0';
}

//文字列の中に含まれるコンマの数を調べる
//ただし、ダブルクォーテーションで囲まれた文字は無視する
extern void CountComma(const char *String, size_t StringSize, size_t *CommaNum)
{
	size_t Count = 0;
	bool InString = false;
	for(size_t i = 0; i < StringSize - 1 && String[i] != '\0'; i++)
	{
		if(IsDoubleQuotation(String, i))
		{
			InString = InString ? false : true;
		}
		if(InString == false && String[i] == ',')
		{
			Count++;
		}
	}
	*CommaNum = Count;
}

//文字列の先頭から任意の文字と一致する文字を探す
//ただし、ダブルクォーテーションで囲まれた文字は無視する
//見つからなかった場合はnullが返る
extern char *SearchStringChar(char *String, char Target)
{
	bool InString = false;
	for(size_t i = 0; String[i] != '\0'; i++)
	{
		if(IsDoubleQuotation(String, i))
		{
			InString = InString ? false : true;
		}
		if(InString == false && String[i] == Target)
		{
			return &String[i];
		}
	}
	return nullptr;
}

//文字列から、変数に変換
extern void CreateVariableFromString(const char *String, size_t StringSize, datum_t *Converted)
{
	//nullが渡されたら全部ゼロにして終了
	if(String == nullptr)
	{
		Converted-> _Bool = false;
		Converted->_Int = 0;
		Converted->_Float = 0.0f;
		Converted->_Double = 0.0;
		Converted->_String = nullptr;
		Converted->_StringSize = 0;
		return;
	}
	///以降はnullじゃないときの処理
	//コピーする
	char *CopyString = nullptr;
	size_t CopyStringSize = 0;
	clone_string(String, StringSize, &CopyString, &CopyStringSize);
	//コピーに失敗したら終了
	if(CopyString == nullptr)
	{
		return;
	}
	//ダブルクォーテーションを消す
	DeleteDoubleQuotation(CopyString, CopyStringSize);
	//整数だと解釈する
	Converted->_Int = (int)strtol(CopyString, nullptr, 10);
	//単精度浮動小数だと解釈する
	Converted->_Float = strtof(CopyString, nullptr);
	//倍精度浮動小数だと解釈する
	Converted->_Double = strtod(CopyString, nullptr);
	//倫理値だと解釈する
	Converted->_Bool = false;
	Converted->_Bool |= (strcmp(CopyString, "true") == 0);
	Converted->_Bool |= ((bool)(Converted->_Double));
	//文字列のクローンを作る
	clone_string(CopyString, CopyStringSize, &Converted->_String, &Converted->_StringSize);
	//コピーしたものを解放
	destroy_string(&CopyString);
}

//文字列から、コンマ区切りの変数を読み込んで配列を作る(読み込み部分)
extern void CreateVariableArrayWithReadString(const char *String, size_t StringSize, datum_t *VariableArray, size_t VariableArraySize)
{
	//コピーする
	char *CopyString = nullptr;
	clone_string(String, StringSize, &CopyString, nullptr);
	//コピーに失敗したら終了
	if(CopyString == nullptr)
	{
		return;
	}
	//ひとつひとつ変換していく
	{
		char *Current = CopyString;
		for(size_t i = 0; i < VariableArraySize; i++)
		{
			//コンマを探す
			char *Comma = SearchStringChar(Current, ',');
			//ここで見つからなければ最後に到達したということになるので、一回だけ変換を施して終了
			if(Comma == nullptr)
			{
				CreateVariableFromString(Current, (String + StringSize) - Current, &VariableArray[i]);
				break;
			}
			//見つかったのでnull文字にする
			*Comma = '\0';
			//変換する
			CreateVariableFromString(Current, Comma - Current, &VariableArray[i]);
			//一個次へ
			Current = &Comma[1];
		}
	}
	//コピーしたものを解放
	destroy_string(&CopyString);
}

//文字列から、コンマ区切りの変数を読み込んで配列を作る(準備部分)
extern void CreateVariableArrayFromString(const char *String, size_t StringSize, datum_t **VariableArray, size_t *VariableArraySize)
{
	//コピーする
	char *CopyString = nullptr;
	size_t CopyStringSize = 0;
	clone_string(String, StringSize, &CopyString, &CopyStringSize);
	//コピーに失敗したら終了
	if(CopyString == nullptr)
	{
		return;
	}
	//コメントを消す
	DeleteComment(CopyString, CopyStringSize);
	//制御文字を消す
	DeleteControl(CopyString, CopyStringSize);
	//コンマの数を調べる
	size_t CommaNum = 0;
	CountComma(CopyString, CopyStringSize, &CommaNum);
	//メモリ確保
	size_t ElementNum = CommaNum + 1;
	size_t ArrayBytes = sizeof(datum_t) * ElementNum;
	*VariableArray = (datum_t*)malloc(ArrayBytes);
	//メモリ確保に失敗したら終了
	if(*VariableArray == nullptr)
	{
		return;
	}
	//サイズを教える
	if(VariableArraySize)
	{
		*VariableArraySize = ElementNum;
	}
	//読み込む
	CreateVariableArrayWithReadString(CopyString, CopyStringSize, *VariableArray, ElementNum);
	//コピーしたものはいらなくなるので解放
	destroy_string(&CopyString);
}

//ファイルから、コンマ区切りの変数を読み込む
extern void CreateVariableArrayFromFile(const char *FileName, datum_t **VariableArray, size_t *VariableArraySize)
{
	//ファイル内の文字を読み込む
	char *FileString = nullptr;
	size_t FileStringSize = 0;
	create_string_from_file(FileName, &FileString, &FileStringSize);
	//ファイルから読み込んだ文字を使って配列を作る
	CreateVariableArrayFromString(FileString, FileStringSize, VariableArray, VariableArraySize);
	//文字情報はいらなくなるので解放
	destroy_string(&FileString);
}

//配列を解放する
extern void DestroyVariableArray(datum_t **VariableArray, size_t *VariableArraySize)
{
	//nullが渡されたら何もしない
	if(VariableArray == nullptr)
	{
		return;
	}
	//文字列部分を解放
	for(size_t i = 0; i < *VariableArraySize; i++)
	{
		destroy_string(&(*VariableArray)[i]._String);
	}
	//解放
	free(*VariableArray);
	//nullを代入
	*VariableArray = nullptr;
	//0を代入
	*VariableArraySize = 0;
}