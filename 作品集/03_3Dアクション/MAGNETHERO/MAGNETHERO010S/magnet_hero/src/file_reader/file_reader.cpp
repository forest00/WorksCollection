//�e�w�b�_�[
#include "file_reader.h"
//���̑��̃w�b�_�[
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "..\char_ptr\char_ptr.h"

//�^����ꂽ�������󔒂����䕶���̂Ƃ��A�^(0�ȊO)��Ԃ��A�����łȂ��ꍇ�́A�U(0)��Ԃ�
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

//n�����ڂ��_�u���N�H�[�e�[�V�����ł����true��Ԃ��A�������An-1�����ڂ�"\"�ł����false��Ԃ�
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

//������̒�����"//"��"\n"�ň͂܂ꂽ�����A�܂��́A"/*"��"*/"�ň͂܂ꂽ�������폜����
//�������A�_�u���N�H�[�e�[�V�����ň͂܂ꂽ�����͖�������
//��:"123/*456*/789//ABC\nDEF\"//000\n\""->"123789DEF\"//000\n\""
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
				//�s�R�����g�̏I���𔻒�
				bool EndOfLineComment = String[i] == '\n';
				//�͈̓R�����g�̏I���𔻒�
				bool EndOfRangeComment = String[i] == '*' && String[i + 1] == '/';
				if(EndOfLineComment || EndOfRangeComment)
				{
					//�͈̓R�����g�Ȃ烋�[�v�J�E���^�[��i�߂�
					if(EndOfRangeComment)
					{
						i++;
					}
					//�R�����g�̒��ł͂Ȃ��Ȃ�
					InComment = false;
					//�������Ȃ��Ƃ����Ȃ��̂ł��̃��[�v�͏I��
					continue;
				}
			}
			else
			{
				//�s�R�����g�̎n�܂�𔻒�
				bool StartOfLineComment = String[i] == '/' && String[i + 1] == '/';
				//�͈̓R�����g�̎n�܂�𔻒�
				bool StartOfRangeComment = String[i] == '/' && String[i + 1] == '*';
				if(StartOfLineComment || StartOfRangeComment)
				{
					//��x�ɓ�̕����𖳎�����̂Ń��[�v�J�E���^�[��i�߂Ă���
					i++;
					//�R�����g�̒��ɂȂ�
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

//������̒�����A�^�u����s�Ȃǂ̐��䕶��������
//�������A�_�u���N�H�[�e�[�V�����ň͂܂ꂽ�����͖�������
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

//������̒�����A�_�u���N�H�[�e�[�V����������
//�������A�ЂƂO�̕�����"\\"�ł���΁A�����Ȃ�
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

//������̒��Ɋ܂܂��R���}�̐��𒲂ׂ�
//�������A�_�u���N�H�[�e�[�V�����ň͂܂ꂽ�����͖�������
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

//������̐擪����C�ӂ̕����ƈ�v���镶����T��
//�������A�_�u���N�H�[�e�[�V�����ň͂܂ꂽ�����͖�������
//������Ȃ������ꍇ��null���Ԃ�
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

//�����񂩂�A�ϐ��ɕϊ�
extern void CreateVariableFromString(const char *String, size_t StringSize, datum_t *Converted)
{
	//null���n���ꂽ��S���[���ɂ��ďI��
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
	///�ȍ~��null����Ȃ��Ƃ��̏���
	//�R�s�[����
	char *CopyString = nullptr;
	size_t CopyStringSize = 0;
	clone_string(String, StringSize, &CopyString, &CopyStringSize);
	//�R�s�[�Ɏ��s������I��
	if(CopyString == nullptr)
	{
		return;
	}
	//�_�u���N�H�[�e�[�V����������
	DeleteDoubleQuotation(CopyString, CopyStringSize);
	//�������Ɖ��߂���
	Converted->_Int = (int)strtol(CopyString, nullptr, 10);
	//�P���x�����������Ɖ��߂���
	Converted->_Float = strtof(CopyString, nullptr);
	//�{���x�����������Ɖ��߂���
	Converted->_Double = strtod(CopyString, nullptr);
	//�ϗ��l���Ɖ��߂���
	Converted->_Bool = false;
	Converted->_Bool |= (strcmp(CopyString, "true") == 0);
	Converted->_Bool |= ((bool)(Converted->_Double));
	//������̃N���[�������
	clone_string(CopyString, CopyStringSize, &Converted->_String, &Converted->_StringSize);
	//�R�s�[�������̂����
	destroy_string(&CopyString);
}

//�����񂩂�A�R���}��؂�̕ϐ���ǂݍ���Ŕz������(�ǂݍ��ݕ���)
extern void CreateVariableArrayWithReadString(const char *String, size_t StringSize, datum_t *VariableArray, size_t VariableArraySize)
{
	//�R�s�[����
	char *CopyString = nullptr;
	clone_string(String, StringSize, &CopyString, nullptr);
	//�R�s�[�Ɏ��s������I��
	if(CopyString == nullptr)
	{
		return;
	}
	//�ЂƂЂƂϊ����Ă���
	{
		char *Current = CopyString;
		for(size_t i = 0; i < VariableArraySize; i++)
		{
			//�R���}��T��
			char *Comma = SearchStringChar(Current, ',');
			//�����Ō�����Ȃ���΍Ō�ɓ��B�����Ƃ������ƂɂȂ�̂ŁA��񂾂��ϊ����{���ďI��
			if(Comma == nullptr)
			{
				CreateVariableFromString(Current, (String + StringSize) - Current, &VariableArray[i]);
				break;
			}
			//���������̂�null�����ɂ���
			*Comma = '\0';
			//�ϊ�����
			CreateVariableFromString(Current, Comma - Current, &VariableArray[i]);
			//�����
			Current = &Comma[1];
		}
	}
	//�R�s�[�������̂����
	destroy_string(&CopyString);
}

//�����񂩂�A�R���}��؂�̕ϐ���ǂݍ���Ŕz������(��������)
extern void CreateVariableArrayFromString(const char *String, size_t StringSize, datum_t **VariableArray, size_t *VariableArraySize)
{
	//�R�s�[����
	char *CopyString = nullptr;
	size_t CopyStringSize = 0;
	clone_string(String, StringSize, &CopyString, &CopyStringSize);
	//�R�s�[�Ɏ��s������I��
	if(CopyString == nullptr)
	{
		return;
	}
	//�R�����g������
	DeleteComment(CopyString, CopyStringSize);
	//���䕶��������
	DeleteControl(CopyString, CopyStringSize);
	//�R���}�̐��𒲂ׂ�
	size_t CommaNum = 0;
	CountComma(CopyString, CopyStringSize, &CommaNum);
	//�������m��
	size_t ElementNum = CommaNum + 1;
	size_t ArrayBytes = sizeof(datum_t) * ElementNum;
	*VariableArray = (datum_t*)malloc(ArrayBytes);
	//�������m�ۂɎ��s������I��
	if(*VariableArray == nullptr)
	{
		return;
	}
	//�T�C�Y��������
	if(VariableArraySize)
	{
		*VariableArraySize = ElementNum;
	}
	//�ǂݍ���
	CreateVariableArrayWithReadString(CopyString, CopyStringSize, *VariableArray, ElementNum);
	//�R�s�[�������̂͂���Ȃ��Ȃ�̂ŉ��
	destroy_string(&CopyString);
}

//�t�@�C������A�R���}��؂�̕ϐ���ǂݍ���
extern void CreateVariableArrayFromFile(const char *FileName, datum_t **VariableArray, size_t *VariableArraySize)
{
	//�t�@�C�����̕�����ǂݍ���
	char *FileString = nullptr;
	size_t FileStringSize = 0;
	create_string_from_file(FileName, &FileString, &FileStringSize);
	//�t�@�C������ǂݍ��񂾕������g���Ĕz������
	CreateVariableArrayFromString(FileString, FileStringSize, VariableArray, VariableArraySize);
	//�������͂���Ȃ��Ȃ�̂ŉ��
	destroy_string(&FileString);
}

//�z����������
extern void DestroyVariableArray(datum_t **VariableArray, size_t *VariableArraySize)
{
	//null���n���ꂽ�牽�����Ȃ�
	if(VariableArray == nullptr)
	{
		return;
	}
	//�����񕔕������
	for(size_t i = 0; i < *VariableArraySize; i++)
	{
		destroy_string(&(*VariableArray)[i]._String);
	}
	//���
	free(*VariableArray);
	//null����
	*VariableArray = nullptr;
	//0����
	*VariableArraySize = 0;
}