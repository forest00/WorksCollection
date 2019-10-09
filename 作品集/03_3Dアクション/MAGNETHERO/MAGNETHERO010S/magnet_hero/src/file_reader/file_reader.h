#pragma once
//�e�L�X�g�t�@�C���������ǂݎ��֐��������ꂽ�w�b�_�[�t�@�C���ł�
//�쐬�ҁ@�X�@�m�u
//�X�V����
//2019/01/02...�t�@�C�����쐬

//�t�@�C������ǂݎ��ϐ�
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

//�^����ꂽ�������󔒂����䕶���̂Ƃ��A�^��Ԃ��A�����łȂ��ꍇ�́A�U��Ԃ�
extern bool IsSpaceOrControl(char c);

//n�����ڂ��_�u���N�H�[�e�[�V�����ł����true��Ԃ��A�������An-1�����ڂ�"\"�ł����false��Ԃ�
extern bool IsDoubleQuotation(const char *String, size_t n);

//������̒�����"//"��"\n"�ň͂܂ꂽ�����A�܂��́A"/*"��"*/"�ň͂܂ꂽ�������폜����
//�������A�_�u���N�H�[�e�[�V�����ň͂܂ꂽ�����͖�������
//��:"123/*456*/789//ABC\nDEF\"//000\n\""->"123789DEF\"//000\n\""
extern void DeleteComment(char *String, size_t StringSize);

//������̒�����A�^�u����s�Ȃǂ̐��䕶��������
//�������A�_�u���N�H�[�e�[�V�����ň͂܂ꂽ�����͖�������
extern void DeleteControl(char *String, size_t StringSize);

//������̒�����A�_�u���N�H�[�e�[�V����������
//�������A�ЂƂO�̕�����"\\"�ł���΁A�����Ȃ�
extern void DeleteDoubleQuotation(char *String, size_t StringSize);

//������̒��Ɋ܂܂��R���}�̐��𒲂ׂ�
//�������A�_�u���N�H�[�e�[�V�����ň͂܂ꂽ�����͖�������
extern void CountComma(const char *String, size_t StringSize, size_t *CommaNum);

//������̐擪����C�ӂ̕����ƈ�v���镶����T��
//�������A�_�u���N�H�[�e�[�V�����ň͂܂ꂽ�����͖�������
//������Ȃ������ꍇ��null���Ԃ�
extern char *SearchStringChar(char *String, char Target);

//�����񂩂�A�ϐ��ɕϊ�
extern void CreateVariableFromString(const char *String, size_t StringSize, datum_t *Converted);

//�����񂩂�A�R���}��؂�̕ϐ���ǂݍ���Ŕz������(�ǂݍ��ݕ���)
extern void CreateVariableArrayWithReadString(const char *String, size_t StringSize, datum_t *VariableArray, size_t VariableArraySize);

//�����񂩂�A�R���}��؂�̕ϐ���ǂݍ���Ŕz������(��������)
extern void CreateVariableArrayFromString(const char *String, size_t StringSize, datum_t **VariableArray, size_t *VariableArraySize);

//�t�@�C������A�R���}��؂�̕ϐ���ǂݍ���Ŕz������
extern void CreateVariableArrayFromFile(const char *FileName, datum_t **VariableArray, size_t *VariableArraySize);

//�z����������
extern void DestroyVariableArray(datum_t **VariableArray, size_t *VariableArraySize);