// Fill out your copyright notice in the Description page of Project Settings.

#include "CollectBombCapacityModifier.h"

CollectBombCapacityModifier::CollectBombCapacityModifier(ABombermanBoard* newBombermanBoard, int newCol, int newRow, float newAmount)
	: Modifier(newBombermanBoard, newCol, newRow), amount(newAmount)
{
	type = EModifierType::PERM_BOMB_CAPACITY;
}

CollectBombCapacityModifier::~CollectBombCapacityModifier()
{
}

void CollectBombCapacityModifier::Process()
{
	// Just destroy this modifier after being assigned to the player
	board->SetModifier(nullptr, col, row);
}

void CollectBombCapacityModifier::OnPlayerContact(ABombermanPlayer * playerToApply)
{
	if (playerToApply != nullptr)
	{
		playerToApply->AddToBombCapacity(amount);
		Process();
	}
}
