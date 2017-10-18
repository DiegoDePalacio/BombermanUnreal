// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modifier.h"

/**
 * Increases the linear extension of the player bombs
 */
class BOMBERMAN_API CollectBombBlastModifier : public Modifier
{
private:
	// How much will affect the player?
	float amount = 0.0f;

public:
	CollectBombBlastModifier(ABombermanBoard* newBombermanBoard, int newCol, int newRow, float newAmount);
	~CollectBombBlastModifier();

	// Inherited via Modifier
	virtual void Process() override;

protected:
	// Inherited via Modifier
	virtual void OnPlayerContact(ABombermanPlayer * playerToApply) override;
};
