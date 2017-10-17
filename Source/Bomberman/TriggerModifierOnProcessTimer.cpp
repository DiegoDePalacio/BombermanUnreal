// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerModifierOnProcessTimer.h"
#include "Modifier.h"

TriggerModifierOnProcessTimer::TriggerModifierOnProcessTimer(Modifier * newModifier, float newDuration)
	: modifier(newModifier), duration(newDuration)
{
}

TriggerModifierOnProcessTimer::~TriggerModifierOnProcessTimer()
{
}

void TriggerModifierOnProcessTimer::Update(float deltaTime)
{
	if (duration > 0.0f)
	{
		duration -= deltaTime;

		if (duration <= 0.0f)
		{
			modifier->Process();
		}
	}
}
