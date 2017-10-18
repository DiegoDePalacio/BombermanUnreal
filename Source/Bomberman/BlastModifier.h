// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modifier.h"
#include "TriggerModifierOnProcessTimer.h"

/**
 * This tile modifier will kill any player that touch it (Used mainly for the temporary effect left by the explosion of a bomb)
 */
class BOMBERMAN_API BlastModifier : public Modifier
{
public:
	TriggerModifierOnProcessTimer* timer;

public:
	BlastModifier(ABombermanBoard* newBombermanBoard, int newCol, int newRow);
	virtual ~BlastModifier();

	// Inherited via Modifier
	virtual void Process() override;
	virtual void OnPlayerContact(ABombermanPlayer* playerToApply) override;
};
