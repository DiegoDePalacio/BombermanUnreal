// Fill out your copyright notice in the Description page of Project Settings.

#include "CollectRemoteBombModifier.h"

CollectRemoteBombModifier::CollectRemoteBombModifier(ABombermanBoard* newBombermanBoard, int newCol, int newRow, float newRemoteDuration)
	: Modifier(newBombermanBoard, newCol, newRow), remoteDuration(newRemoteDuration)
{
	type = EModifierType::TEMP_REMOTE_BOMB;
}

CollectRemoteBombModifier::~CollectRemoteBombModifier()
{
	if (timer != nullptr)
	{
		board->UnregisterTimer(timer);
		delete(timer);
	}
}

void CollectRemoteBombModifier::Process()
{
	// Remove the remote from the player
	player->RemoveRemoteBomb();

	board->UnregisterTimer(timer);

	// Destroy this modifier after his time is over
	board->SetModifier(nullptr, col, row);
}

void CollectRemoteBombModifier::OnPlayerContact(ABombermanPlayer * playerToApply)
{
	// If the modifier was already assigned, then there is nothing to do
	if (player != nullptr) { return; }

	if (playerToApply != nullptr)
	{
		player = playerToApply;
		player->AddRemoteBomb();
		
		timer = new TriggerModifierOnProcessTimer(this, remoteDuration);
		board->RegisterTimer(timer);

		visual->SetActorHiddenInGame(true);
	}
}
