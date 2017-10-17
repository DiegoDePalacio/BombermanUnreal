// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BombermanBoard.h"
#include "BombermanPlayer.h"

// Type of modifications that the modifiers will do to the players affected by them
UENUM(BlueprintType)
enum class EModifierType : uint8
{
	// Type of board modifiers that can't be owned by the player and are not power-ups
	UNSET,          // The type is not yet assigned
	BLAST,          // A tile that will kill any player that touch it (Used mainly for the temporary effect left by the explosion of a bomb)

	// Modifiers that have a player as the owner
	OWN_TIME_BOMB,      // Regular type of bomb to explode after some time
	OWN_REMOTE_BOMB,    // A bomb that will explode when the owner activate it

	// Permanent power-ups (or "power-downs") types
	PERM_SPEED,           // Changes the movement speed
	PERM_BOMB_CAPACITY,   // Changes the number of simultaneous bombs that can be triggered by a player at the same time
	PERM_BOMB_BLAST,      // Increases the linear extension of the player bombs

	// Temporary (timed) power-ups (or "power-downs") types
	TEMP_REMOTE_BOMB
};

/**
 * Base class for all the possible assets that can be on a tile that affects the player in some way
 * only one of them can be on a tile at once
 */
class BOMBERMAN_API Modifier
{
public:
	// The specific behavior that the modifier will have, not directly related with a player, like destroying a wall
	virtual void Process() = 0;

protected:
	EModifierType type = EModifierType::UNSET;

	// A reference to the board
	ABombermanBoard* board = nullptr;

	// Tile in where this modifier resides
	int col = -1;
	int row = -1;

	// The effects that this modifier will cause to a player touching it
	virtual void OnPlayerContact(ABombermanPlayer playerToApply) = 0;
};
