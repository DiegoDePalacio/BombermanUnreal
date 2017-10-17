// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Forward declaration in order to avoid circular dependencies
class Modifier;

/**
 * A class for triggering the Modifier::OnProcess method after the duration ends for using 
 * composition inside of the "time-based" modifier classes in order to avoid multiple inheritance
 */
class BOMBERMAN_API TriggerModifierOnProcessTimer
{
private:
	Modifier* modifier = nullptr;

	// How much time will have to pass in order to trigger the modifier::OnProcess method?
	float duration = 0.0f;

public:
	TriggerModifierOnProcessTimer(Modifier* newModifier, float newDuration);
	~TriggerModifierOnProcessTimer();

	void Update(float deltaTime);
};
