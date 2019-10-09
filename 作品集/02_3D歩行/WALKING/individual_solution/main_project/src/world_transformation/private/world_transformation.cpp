
//�e�w�b�_�[
#include "..\public\world_transformation.h"

//���[���h�ϊ����s��
extern void world_transformation(matrix4x4 *world, const vector3_t *position, const vector3_t *angle, const vector3_t *scale)
{
	matrix4x4 matrix_scale;				//�X�P�[�����O�s��
	matrix4x4 matrix_rotation;			//��]�s��
	matrix4x4 matrix_translation;		//���s�ړ��s��

										//�s���������
	D3DXMatrixIdentity(world);
	D3DXMatrixTranslation(&matrix_translation, position->x, position->y, position->z);
	D3DXMatrixRotationYawPitchRoll(&matrix_rotation, angle->y, angle->x, angle->z);
	D3DXMatrixScaling(&matrix_scale, scale->x, scale->y, scale->z);

	//���[���h�ϊ�
#if 1
	//���[���h�s�񁁃X�P�[�����O�~��]�~�ړ�
	*world = matrix_scale * matrix_rotation * matrix_translation;
#else
	D3DXMatrixMultiply(world, world, &matrix_scale);
	D3DXMatrixMultiply(world, world, &matrix_rotation);
	D3DXMatrixMultiply(world, world, &matrix_translation);
#endif
}

