// Fill out your copyright notice in the Description page of Project Settings.

#include "CollectRemoteBombModifier.h"

CollectRemoteBombModifier::CollectRemoteBombModifier(ABombermanBoard* newBombermanBoard, int newCol, int newRow, float newRemoteDuration)
	: Modifier(newBombermanBoard, newCol, newRow), remoteDuration(newRemoteDuration)
{
	type = EModifierType::TEMP_REMOTE_BOMB;
}

CollectRemoteBombModifier::~CollectRemoteBombModifier()
{
}

void CollectRemoteBombModifier::Process()
{
	// Remove the remote from the player
	player->RemoveRemoteBomb();
}

void CollectRemoteBombModifier::OnPlayerContact(ABombermanPlayer * playerToApply)
{
	if (playerToApply != nullptr)
	{
		playerToApply->AddRemoteBomb();
		timer = new TriggerModifierOnProcessTimer(this, remoteDuration);
	}
}
