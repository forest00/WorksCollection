
//�e�w�b�_�[
#include "..\public\sound.h"
//���̑��̃w�b�_�[
#include <assert.h>
#include "..\public\sound_index.h"
#include "..\private\dslib.h"
#include "..\..\my_3d_lib\public\my_3d_lib.h"
#include "..\..\file_path_provider\public\file_path_provider.h"
#include "..\..\calc\public\calc.h"
#include "..\..\common_def\public\common_def.h"



//�y�ȃt�@�C�����ƍĐ����@�̃f�[�^�x�[�X�\����
struct load_data_t
{
	char *file_name;
	DWORD flag;
};

//�y�ȃt�@�C����ǂݍ��ނƂ��ɕK�v�ȃf�[�^
static load_data_t g_load_database[(int)(sound_index__max)] = {};

//���݃X�g���[���Đ�����Ă���y�Ȃ�index
static sound_index_t g_playing_stream_music = sound_index__none;

//�y�Ȃ̃n���h��
static int g_handles[(int)(sound_index__max)];

//�t�@�C���������������`�F�b�N
static bool is_correct_file_name(const char *file_name, size_t chars_num)
{
	size_t length = strnlen_s(file_name, chars_num);
	//�����������Ȃ�����
	if (length < 4)
	{
		return false;
	}
	//���̍s�ɏ�����Ă���̂̓R�����g�ł���
	if (file_name[0] == '/' && file_name[1] == '/')
	{
		return false;
	}
	//�g���q��".wav"����Ȃ�
	if (file_name[length - 4] != '.' ||
		file_name[length - 3] != 'w' ||
		file_name[length - 2] != 'a' ||
		file_name[length - 1] != 'v')
	{
		return false;
	}
	return true;
}

//�y�ȃt�@�C�������t�@�C�����猈�肷��
static void filename_difintion(const char *file_name)
{
	//�t�@�C�����J��
	FILE *f = nullptr;
	fopen_s(&f, file_name, "rb");
	if (f != nullptr)
	{
		//�ŏ��̈��null
		int i = 0;
		g_load_database[i].file_name = nullptr;
		i++;
		while (!feof(f))
		{
			//�Ƃ肠�����̈�m��
			if (g_load_database[i].file_name == nullptr)
			{
				g_load_database[i].file_name = (char*)malloc(sizeof(char) * FILENAME_MAX);
				assert(g_load_database[i].file_name != nullptr);
			}
			//�t�@�C������ǂ�
			fgets(g_load_database[i].file_name, FILENAME_MAX, f);
			change_target_to_null(g_load_database[i].file_name, FILENAME_MAX, "\r\n");
			{
				char *found = strchr(g_load_database[i].file_name, (int)(','));
				if (found != nullptr)
				{
					int flag = 0;
					sscanf_s(found, ",%d", &flag);
					if (flag)
					{
						g_load_database[i].flag = DSLF_FLG_STREAM;
					}
					else
					{
						g_load_database[i].flag = DSLF_FLG_STATIC;
					}
				}
			}
			change_target_to_null(g_load_database[i].file_name, FILENAME_MAX, " ,");

			//�ǂݍ��񂾂��̂��`�F�b�N
			//�������t�@�C�����łȂ���΁A�ǂݍ��ݒ���
			if (is_correct_file_name(g_load_database[i].file_name, FILENAME_MAX) == false)
			{
				continue;
			}
			//�ǂݍ��񂾂��̂͐������t�@�C�����ł��邽�߁A���̂܂܎��֐i��
			i++;
			if (i >= (int)(sound_index__max))
			{
				break;
			}
		}
		fclose(f);
	}
}

//�S�y�Ȃ�ǂݍ���
static void load_all()
{
	static bool yet = true;
	if (yet)
	{
		yet = false;
		filename_difintion(wav_file_path());
		g_handles[(int)(sound_index__none)] = -1;
		for (int i = (int)(sound_index__none) + 1; i < (int)(sound_index__max); i++)
		{
			g_handles[i] = DSoundLoadFile((LPSTR)g_load_database[i].file_name, -1, g_load_database[i].flag);
		}
	}
}

//���
static void release()
{
	for (int i = 0; i < (int)(sound_index__max); i++)
	{
		safe_free(g_load_database[i].file_name);
	}
}

//�X�g���[���Đ�����A�������A���łɍĐ�����Ă���΁A�������Ȃ�
static void play_stream(sound_index_t index)
{
	//�Ō�ɃX�g���[���Đ����ꂽ���̂��o���Ă���
	const auto &last_play = g_playing_stream_music;
	if (last_play != index)
	{
		DSoundStop(g_handles[(int)(last_play)]);
		DSoundPlay(g_handles[(int)(index)], true);
	}
	g_playing_stream_music = index;
}

















































//������
extern int sound_manager_init(HWND window_handle)
{
	if (DSoundInit(window_handle, nullptr) == FALSE)
	{
		return -1;
	}
	load_all();
	return 0;
}
//�I��
extern void sound_manager_end()
{
	release();
	DSoundRelease();
}

//�y�Ȃ��Đ�
extern void sound_play(sound_index_t index)
{
#ifdef SOUND_OFF
	//����炳�Ȃ�
	return;
#endif
	int _index = (int)(index);
	switch (g_load_database[_index].flag)
	{
	case DSLF_FLG_DEFAULT:
	case DSLF_FLG_STATIC:
		//���ʂ̍Đ�(����)
		DSoundIsStop(g_handles[_index]);
		DSoundStop(g_handles[_index]);
		DSoundPlay(g_handles[_index], false);
		break;
	case DSLF_FLG_STREAM:
		//�X�g���[���Đ�
		play_stream(index);
		break;
	default:
		break;
	}
}




