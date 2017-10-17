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
	
	
};
