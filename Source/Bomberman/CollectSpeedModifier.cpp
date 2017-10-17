// Fill out your copyright notice in the Description page of Project Settings.

#include "CollectSpeedModifier.h"

CollectSpeedModifier::CollectSpeedModifier(ABombermanBoard* newBombermanBoard, int newCol, int newRow, float newAmount)
	: Modifier(newBombermanBoard, newCol, newRow), amount(newAmount)
{
	type = EModifierType::PERM_SPEED;
}

CollectSpeedModifier::~CollectSpeedModifier()
{
}

void CollectSpeedModifier::Process()
{
	// Just destroy this modifier after being assigned to the player
	board->SetModifier(nullptr, col, row);
}

void CollectSpeedModifier::OnPlayerContact(ABombermanPlayer * playerToApply)
{
	if (playerToApply != nullptr)
	{
		playerToApply->AddToSpeed(amount);
		Process();
	}
}
