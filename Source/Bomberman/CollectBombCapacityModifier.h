// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modifier.h"

/**
 * Changes the number of simultaneous bombs that can be triggered by a player at the same time
 */
class BOMBERMAN_API CollectBombCapacityModifier : public Modifier
{
private:
	// How much will affect the player?
	float amount = 0.0f;

public:
	CollectBombCapacityModifier(ABombermanBoard* newBombermanBoard, int newCol, int newRow, float newAmount);
	~CollectBombCapacityModifier();

	// Inherited via Modifier
	virtual void Process() override;

protected:
	// Inherited via Modifier
	virtual void OnPlayerContact(ABombermanPlayer * playerToApply) override;
};
