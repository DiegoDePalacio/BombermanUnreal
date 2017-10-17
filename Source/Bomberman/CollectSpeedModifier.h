// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modifier.h"

/**
 * Changes the movement speed of the player
 */
class BOMBERMAN_API CollectSpeedModifier : public Modifier
{
private:
	// How much will affect the player?
	float amount = 0.0f;

public:
	CollectSpeedModifier(ABombermanBoard* newBombermanBoard, int newCol, int newRow, float newAmount);
	~CollectSpeedModifier();

	// Inherited via Modifier
	virtual void Process() override;

protected:
	// Inherited via Modifier
	virtual void OnPlayerContact(ABombermanPlayer * playerToApply) override;
};
