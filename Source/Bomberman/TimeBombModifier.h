// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RemoteBombModifier.h"
#include "TriggerModifierOnProcessTimer.h"
#include "BombermanPlayer.h"

/**
 * Regular type of bomb to explode after some time
 */
class BOMBERMAN_API TimeBombModifier : public RemoteBombModifier
{
public:
	TriggerModifierOnProcessTimer* timer;

public:
	TimeBombModifier (ABombermanBoard* newBombermanBoard, int newCol, int newRow, ABombermanPlayer* newPlayer, int newLinearExtension);
	virtual ~TimeBombModifier();
};
