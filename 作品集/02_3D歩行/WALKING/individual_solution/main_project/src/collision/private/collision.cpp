
#include "..\public\collision.h"

extern vector3_t aabb_segment(const aabb_t *aabb)
{
	vector3_t segment = aabb->max - aabb->min;
	return segment;
}

extern bool aabb_is_hit(const aabb_t *aabb1, const aabb_t *aabb2)
{
	bool x_hit = (aabb1->max.x < aabb2->min.x) && (aabb2->max.x < aabb1->min.x);
	bool y_hit = (aabb1->max.y < aabb2->min.y) && (aabb2->max.y < aabb1->min.y);
	bool z_hit = (aabb1->max.z < aabb2->min.z) && (aabb2->max.z < aabb1->min.z);
	return x_hit && y_hit && z_hit;
}

extern bool sphere_is_hit(const sphere_t *sphere1, const sphere_t *sphere2)
{
	vector3_t sub = sphere1->center - sphere2->center;
	float distance = D3DXVec3Length(&sub);
	float radius_sum = sphere1->radius + sphere2->radius;
	return distance >= radius_sum * radius_sum;
}

extern void sphere_draw(IDirect3DDevice9 *device, const sphere_t *sphere)
{
	//スフィアの作成
	LPD3DXMESH pSphereMesh = nullptr;
	D3DMATERIAL9* pSphereMeshMaterials = nullptr;
	D3DXCreateSphere(device, sphere->radius, 16, 16, &pSphereMesh, nullptr);
	//スフィアメッシュのマテリアル　
	pSphereMeshMaterials = new D3DMATERIAL9;
	pSphereMeshMaterials->Diffuse.r = 1.0f;
	pSphereMeshMaterials->Diffuse.g = 1.0f;
	pSphereMeshMaterials->Diffuse.b = 1.0f;
	pSphereMeshMaterials->Diffuse.a = 1.0f;
	pSphereMeshMaterials->Ambient = pSphereMeshMaterials->Diffuse;
	pSphereMeshMaterials->Specular.r = 1.0f;
	pSphereMeshMaterials->Specular.g = 1.0f;
	pSphereMeshMaterials->Specular.b = 1.0f;
	pSphereMeshMaterials->Emissive.r = 0.1f;
	pSphereMeshMaterials->Emissive.g = 0.1f;
	pSphereMeshMaterials->Emissive.b = 0.1f;
	pSphereMeshMaterials->Power = 60.0f;

	//ワールド変換
	D3DXMATRIX world;
	D3DXMatrixTranslation(&world,
		sphere->center.x,
		sphere->center.y,
		sphere->center.z
	);
	device->SetTransform(D3DTS_WORLD, &world);

	//描画
	//スフィアのレンダリング
	device->SetMaterial(pSphereMeshMaterials);
	pSphereMesh->DrawSubset(0);

	//スフィアの破棄
	pSphereMesh->Release();
	delete pSphereMeshMaterials;
}


