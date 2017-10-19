// Fill out your copyright notice in the Description page of Project Settings.

#include "RemoteBombModifier.h"
#include "BlastModifier.h"

RemoteBombModifier::RemoteBombModifier(ABombermanBoard* newBombermanBoard, int newCol, int newRow, ABombermanPlayer* newPlayer, int newLinearExtension)
	: Modifier( newBombermanBoard, newCol, newRow ), player(newPlayer), linearExtension(newLinearExtension)
{
	type = EModifierType::OWN_REMOTE_BOMB;
}

RemoteBombModifier::~RemoteBombModifier()
{
}

void RemoteBombModifier::Process()
{
	ongoing = true;

	// Only destroy the adjacent columns if the player is not between indestructible walls at his sides
	if (row % 2 == 0)
	{
		// Create a blast on the adjacent columns
		int initialCol = col - linearExtension;
		int finalCol = col + linearExtension;

		for (int colToBlast = initialCol; colToBlast <= finalCol; ++colToBlast)
		{
			CheckForBlastOnTile(colToBlast, row);
		}
	}

	// Only destroy the adjacent rows if the player is not between indestructible walls on his top and on his bottom
	if (col % 2 == 0)
	{
		// Create a blast on the adjacent rows
		int initialRow = row - linearExtension;
		int finalRow = row + linearExtension;

		for (int rowToBlast = initialRow; rowToBlast <= finalRow; ++rowToBlast)
		{
			CheckForBlastOnTile(col, rowToBlast);
		}
	}

	player->DecreaseOnGoingBombs();

	// Finally process the current tile
	DestroyAndBlastOnTile(col, row);
}

void RemoteBombModifier::OnPlayerContact(ABombermanPlayer* playerToApply)
{
	// The bomb is innocuous if is not exploding yet
}

// Trigger any bombs that are on the tile to be blasted and call the method for destroying the things inside 
void RemoteBombModifier::CheckForBlastOnTile(int blastCol, int blastRow)
{
	if (!board->IsPlayableTile(blastCol, blastRow)) { return; }

	// The current tile will be processed after all the other destruction
	if (col == blastCol && row == blastRow) { return; }

	Modifier* modifier = board->GetModifier(blastCol, blastRow);

	if (modifier != nullptr)
	{
		// If there is already a blast, then there is nothing to destroy
		if (modifier->type == EModifierType::BLAST) { return; }

		// If the blast affect a bomb, the make the bomb explode
		if (modifier->type == EModifierType::OWN_REMOTE_BOMB || modifier->type == EModifierType::OWN_TIME_BOMB)
		{
			RemoteBombModifier* bomb = static_cast<RemoteBombModifier*>(modifier);

			//...only if the affected bomb was not already triggered
			if (!bomb->ongoing)
			{
				modifier->Process();
			}

			// If there is a bomb exploding or about to explode, this bomb will take care of the destruction instead
			return;
		}
	}

	DestroyAndBlastOnTile(blastCol, blastRow);
}

// Kill and destroy all the power-ups and destructible walls on the tile to be the place of the blast
void RemoteBombModifier::DestroyAndBlastOnTile(int blastCol, int blastRow)
{
	// Destroy the destructible wall if any and set a blast if there was no power-up behind
	if (board->DestroyWall(blastCol, blastRow) == nullptr)
	{
		// If not, kill all the players that are on this BLAST tile if any
		TArray<ABombermanPlayer*> playersOnBlast = board->GetPlayersInTile(blastCol, blastRow);
		for (auto player : playersOnBlast)
		{
			player->DieOnNextFrame();
		}

		// Create a blast on the current tile (destroying with this the powerups in it)
		board->SetModifier(new BlastModifier(board, blastCol, blastRow), blastCol, blastRow);
	}
}
