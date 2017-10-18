// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BombermanGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMAN_API ABombermanGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// So the diameter of the collision box of the player needs to be less than one meter
	const float PLAYER_COLLISION_DIAMETER_MT = 0.8f;
	
	// TODO: Expose this parameters to be configurable on the Editor

	// Maximum / minimum amount of the player speed 
	const float MIN_SPEED = 1.0f;
	const float MAX_SPEED = 5.0f;

	// Maximum / minimum amount of the player bomb capacity
	const int MIN_BOMB_CAPACITY = 1;
	const int MAX_BOMB_CAPACITY = 5;

	// Maximum / minimum amount of the blast extension that the player can have on the bombs
	const int MIN_BLAST_EXTENSION = 1;
	const int MAX_BLAST_EXTENSION = 5;
};
