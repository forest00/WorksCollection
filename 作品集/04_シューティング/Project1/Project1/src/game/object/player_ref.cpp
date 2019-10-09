#include "player_ref.h"

static Object *object_ref = nullptr;

extern void SetPlayerRef(Object *player_ref)
{
	object_ref = player_ref;
}

extern const Object *GetPlayerRef()
{
	return object_ref;
}
