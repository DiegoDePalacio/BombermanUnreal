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
	// For simplicity reasons, the tile size is 1 square meters
	// Unreal unit by default is centimeter
	const float UNREAL_UNIT_TO_MT = 0.01f;

	// So the diameter of the collision box of the player needs to be less than one meter
	const int PLAYER_COLLISION_DIAMETER_MT = 0.9f;
	
	
};
