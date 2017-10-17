// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modifier.h"
#include "TriggerModifierOnProcessTimer.h"
#include "BombermanPlayer.h"

/**
 * 
 */
class BOMBERMAN_API TimeBombModifier : public Modifier
{
public:
	TriggerModifierOnProcessTimer* timer;

private:
	// A reference to the board
	ABombermanBoard* bombermanBoard = nullptr;

	// The owner of the bomb
	ABombermanPlayer* player = nullptr;

	// How much adjacent tiles to the right, left, up and down will be spreading his "post-mortem" effect?
	int linearExtension = 0;

	// In order to not trigger again the bombs that are already exploding
	bool ongoing = false;

public:
	TimeBombModifier (ABombermanBoard* newBombermanBoard, int newCol, int newRow, ABombermanPlayer* newPlayer, int newLinearExtension);
	virtual ~TimeBombModifier();

	void Process() override;

protected:
	void OnPlayerContact (ABombermanPlayer playerToApply) override;

private:
	// Trigger any bombs on the specified tile and destroy the things inside
	void CheckForBlastOnTile(int blastCol, int blastRow);

	// Kill and destroy all the power-ups and destructible walls on the tile to be the place of the blast
	void DestroyAndBlastOnTile(int blastCol, int blastRow);
};
