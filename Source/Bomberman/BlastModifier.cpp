// Fill out your copyright notice in the Description page of Project Settings.

#include "BlastModifier.h"

BlastModifier::BlastModifier(ABombermanBoard* newBombermanBoard, int newCol, int newRow)
{
	type = EModifierType::BLAST;
	timer = new TriggerModifierOnProcessTimer(this, Modifier::TIME_BOMB_IN_SECS);

	board = newBombermanBoard;
	col = newCol;
	row = newRow;
}

BlastModifier::~BlastModifier()
{
	delete(timer);
}

void BlastModifier::Process()
{
	// Just destroy this modifier after his time is over
	board->SetModifier(nullptr, col, row);
}

void BlastModifier::OnPlayerContact(ABombermanPlayer* playerToApply)
{
	if (playerToApply != nullptr)
	{
		// Wait for the end of the frame to kill the player, because maybe the other player will be also killed by the blast
		// In order to not give the victory to one player if it's a tie instead
		playerToApply->DieOnNextFrame();
	}
}