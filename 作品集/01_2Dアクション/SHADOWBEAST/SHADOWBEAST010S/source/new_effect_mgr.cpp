

#include "new_effect_pinch_effect.h"
#include "new_effect_rain_effect.h"
#include "new_effect_hp_heal.h"
#include "new_effect_shield.h"
#include "new_effect_shield_break.h"
#include "new_effect_confusion_star.h"
#include "new_effect_goblin_death_dust.h"


extern void newEffectInitAll()
{
	pinchEffectInit();
	rainEffectInit();
	hpHealEffectInit();
	newEffectShieldInit();
	newEffectShieldBreakInit();
	newEffectConfusionStarInit();
	newEffectGoblinDeathDustInit();
}


extern void newEffectUpdateAll()
{
	pinchEffectUpdate();
	rainEffectUpdate();
	hpHealEffectUpdate();
	newEffectShieldUpdate();
	newEffectShieldBreakUpdate();
	newEffectConfusionStarUpdate();
	newEffectGoblinDeathDustUpdate();
}


extern void newEffectDrawAll()
{
	pinchEffectDraw();
	rainEffectDraw();
	hpHealEffectDraw();
	newEffectShieldDraw();
	newEffectShieldBreakDraw();
	newEffectConfusionStarDraw();
	newEffectGoblinDeathDustDraw();
}


extern void newEffectEndAll()
{
	pinchEffectEnd();
	rainEffectEnd();
	hpHealEffectEnd();
	newEffectShieldEnd();
	newEffectShieldBreakEnd();
	newEffectConfusionStarEnd();
	newEffectGoblinDeathDustEnd();
}
