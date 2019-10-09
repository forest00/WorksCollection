#pragma once

#include "chara__common.h"

typedef enum{
    PLAYER1_BASE,
    BLOCK_BASE,
    DATA_END,
}CharacterData;


const pCharacter getCharacterBase(CharacterData index);

