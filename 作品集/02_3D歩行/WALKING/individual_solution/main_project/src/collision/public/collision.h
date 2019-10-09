#pragma once


#include "..\..\common_def\public\common_def.h"

//axis_aligned_bounding_box
struct aabb_t
{
	vector3_t min;
	vector3_t max;
};

//‹…‘Ì
struct sphere_t
{
	vector3_t center;
	double radius;
};

extern vector3_t aabb_segment(const aabb_t *aabb);

extern bool aabb_is_hit(const aabb_t *aabb1, const aabb_t *aabb2);

extern bool sphere_is_hit(const sphere_t *sphere1, const sphere_t *sphere2);

extern void sphere_draw(IDirect3DDevice9 *device, const sphere_t *sphere);
