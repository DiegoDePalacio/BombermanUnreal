// Fill out your copyright notice in the Description page of Project Settings.

#include "CollectBombBlastModifier.h"

CollectBombBlastModifier::CollectBombBlastModifier(ABombermanBoard* newBombermanBoard, int newCol, int newRow, float newAmount)
	: Modifier(newBombermanBoard, newCol, newRow), amount(newAmount)
{
	type = EModifierType::PERM_BOMB_BLAST;
}

CollectBombBlastModifier::~CollectBombBlastModifier()
{
}

void CollectBombBlastModifier::Process()
{
	// Just destroy this modifier after being assigned to the player
	board->SetModifier(nullptr, col, row);
}

void CollectBombBlastModifier::OnPlayerContact(ABombermanPlayer * playerToApply)
{
	if (playerToApply != nullptr)
	{
		playerToApply->AddToBlastLinearExtension(amount);
		Process();
	}
}
