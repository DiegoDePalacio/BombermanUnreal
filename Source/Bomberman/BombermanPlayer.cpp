// Fill out your copyright notice in the Description page of Project Settings.

#include "BombermanPlayer.h"
#include "Engine/World.h"


// Sets default values
ABombermanPlayer::ABombermanPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABombermanPlayer::BeginPlay()
{
	Super::BeginPlay();
	gameSettings = GetWorld()->GetAuthGameMode<ABombermanGameModeBase>();
}

// Called every frame
void ABombermanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABombermanPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ABombermanPlayer::MoveHorizontally(float amount)
{
	if (!CanDisplaceHorizontally()) { return false; }

	return true;
}

bool ABombermanPlayer::CanDisplaceHorizontally()
{
	// Unreal units are in cms
	FVector playerPositionMt = GetActorLocation() / gameSettings->UNREAL_UNIT_TO_MT;

	int nearestRow = FGenericPlatformMath::RoundToInt(playerPositionMt.X);

	// If the player is between horizontal walls, so the horizontal displacement is not possible
	// The game Dyna Blaster only allows movement in one of every two rows, in an altered way
	if (nearestRow % 2 != 0) { return false; }

	float distanceToNearestRow = FGenericPlatformMath::Abs(playerPositionMt.X - nearestRow);

	// If the player doesn't fit between walls, then the diplacement is not possible
	if (distanceToNearestRow > (1 - gameSettings->PLAYER_COLLISION_DIAMETER_MT))
	{
		return false;
	}

	return true;
}
