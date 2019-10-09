
//親ヘッダー
#include "..\public\world_transformation.h"

//ワールド変換を行う
extern void world_transformation(matrix4x4 *world, const vector3_t *position, const vector3_t *angle, const vector3_t *scale)
{
	matrix4x4 matrix_scale;				//スケーリング行列
	matrix4x4 matrix_rotation;			//回転行列
	matrix4x4 matrix_translation;		//平行移動行列

										//行列を初期化
	D3DXMatrixIdentity(world);
	D3DXMatrixTranslation(&matrix_translation, position->x, position->y, position->z);
	D3DXMatrixRotationYawPitchRoll(&matrix_rotation, angle->y, angle->x, angle->z);
	D3DXMatrixScaling(&matrix_scale, scale->x, scale->y, scale->z);

	//ワールド変換
#if 1
	//ワールド行列＝スケーリング×回転×移動
	*world = matrix_scale * matrix_rotation * matrix_translation;
#else
	D3DXMatrixMultiply(world, world, &matrix_scale);
	D3DXMatrixMultiply(world, world, &matrix_rotation);
	D3DXMatrixMultiply(world, world, &matrix_translation);
#endif
}

