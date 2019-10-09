
//親ヘッダー
#include "test_enemy_brain.h"
//その他のヘッダー
#include ".\..\..\public\test_enemy\test_enemy_manager.h"
#include "..\..\..\player\public\player.h"


//自分の置かれている状況を調べて、どう行動するか判断する
extern void test_enemy_brain_t_judge_action(test_enemy_brain_t *brain, const test_enemy_object_t *object)
{
	*brain = {};
}


