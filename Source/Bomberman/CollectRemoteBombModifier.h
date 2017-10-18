// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modifier.h"
#include "TriggerModifierOnProcessTimer.h"

/**
 * Temporary (timed) power-up for having the possibility of using a remote bomb
 */
class BOMBERMAN_API CollectRemoteBombModifier : public Modifier
{
public:
	TriggerModifierOnProcessTimer* timer;

private:
	// How much time the player will have the remote?
	float remoteDuration = 0.0f;

	// The holder of the remote
	ABombermanPlayer* player = nullptr;

public:
	CollectRemoteBombModifier(ABombermanBoard* newBombermanBoard, int newCol, int newRow, float newRemoteDuration);
	~CollectRemoteBombModifier();

	// Inherited via Modifier
	virtual void Process() override;

protected:
	// Inherited via Modifier
	virtual void OnPlayerContact(ABombermanPlayer * playerToApply) override;
};
