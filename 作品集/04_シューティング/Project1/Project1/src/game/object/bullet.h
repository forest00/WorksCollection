#pragma once
#include "primitive/vector2d.h"

//’e‚ğo‚·
extern int SetBullet(Vector2D pos, Vector2D velocity);

//’e‚ªÁ‚³‚ê‚½‚Æ‚«‚ÉŒÄ‚Î‚ê‚é
extern void BulletCleared(void *kind_info);

//’eXV
extern void UpdateBullet(int object_index);

//’e‚Ì”‚ğ“¾‚é
extern size_t GetBulletNum();
