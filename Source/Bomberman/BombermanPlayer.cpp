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

	if (gameSettings == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Game settings not found!"));
	}

	if( board != NULL )
	{
		float playerDiameter = 0.0f;
		float playerRadius = 0.0f;

		if (gameSettings != nullptr)
		{
			// Unreal unit by default is centimeter
			playerDiameter = gameSettings->PLAYER_COLLISION_DIAMETER_MT * 100.0f;
			playerRadius = playerDiameter * 0.5f;
		}

		// For simplicity reasons, the tile size is 1 square meters
		// The center of the first column and row is the origin
		minPlayerBoardX = -0.5f + playerRadius;
		minPlayerBoardY = -0.5f + playerRadius;

		// For simplicity reasons, the tile size is 1 square meters
		// Unreal unit by default is centimeter
		float boardHeight = board->GetDefaultObject<ABombermanBoard>()->rows * 100.0f;
		float boardWidth = board->GetDefaultObject<ABombermanBoard>()->cols * 100.0f;

		maxPlayerBoardX = minPlayerBoardX + boardHeight - playerDiameter;
		maxPlayerBoardY = minPlayerBoardY + boardWidth - playerDiameter;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Game board not found!"));
	}
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

// Filter to restrict the horizontal movement of the player on the Dyna Blaster board
// Removing the need of physics calculations in a very simple grid-based game type
float ABombermanPlayer::MoveHorizontally(float amount)
{
	// Check first the board boundaries
	if (!WillStayInsideTheBoard(true,amount)) { return 0.0f; }

	// If the displacement is not possible, just return 0 as the movement value
	if (!CanDisplace(true)) { return 0.0f; }

	// Otherwise allow the full movement
	return amount;
}

// Filter to restrict the horizontal movement of the player on the Dyna Blaster board
// Removing the need of physics calculations in a very simple grid-based game type
float ABombermanPlayer::MoveVertically(float amount)
{
	// Check first the board boundaries
	if (!WillStayInsideTheBoard(false, amount)) { return 0.0f; }

	// If the displacement is not possible, just return 0 as the movement value
	if (!CanDisplace(false)) { return 0.0f; }

	// Otherwise allow the full movement
	return amount;
}

// A method for check the specific Dyna Blaster conditions, regarding walls position to see if the player can move horizontally
bool ABombermanPlayer::CanDisplace( bool horizontally )
{
	// Sanity check, if the game settings are not available, then there is nothing to do
	if (gameSettings == nullptr) { return false; }

	// Unreal units are in cms
	FVector playerPositionMt = GetActorLocation() * gameSettings->UNREAL_UNIT_TO_MT;

	float positionBetweenCorridors = (horizontally ? playerPositionMt.X : playerPositionMt.Y);

	// For simplicity reasons, the corridor width is 1 square meters
	int nearestCorridor = FGenericPlatformMath::RoundToInt(positionBetweenCorridors);

	// If the player is between horizontal walls, so the horizontal displacement is not possible
	// The game Dyna Blaster only allows movement in one of every two rows, in an altered way
	if (nearestCorridor % 2 != 0) { return false; }

	float distanceToNearestCorridor = FGenericPlatformMath::Abs(positionBetweenCorridors - nearestCorridor);


	// If the player doesn't fit between walls, then the diplacement is not possible
	if (distanceToNearestCorridor > (1 - gameSettings->PLAYER_COLLISION_DIAMETER_MT))
	{
		return false;
	}

	return true;
}

// The desired movement will place the player inside of the board?
bool ABombermanPlayer::WillStayInsideTheBoard(bool horizontally, float movementAmount)
{
	if (board == NULL) { return false; }

	FVector playerPosition = GetActorLocation();

	if (horizontally)
	{
		if (playerPosition.Y + movementAmount < minPlayerBoardY) { return false; }
		if (playerPosition.Y + movementAmount > maxPlayerBoardY) { return false; }
		return true;
	}
	else
	{
		if (playerPosition.X + movementAmount < minPlayerBoardX) { return false; }
		if (playerPosition.X + movementAmount > maxPlayerBoardX) { return false; }
		return true;
	}
}

