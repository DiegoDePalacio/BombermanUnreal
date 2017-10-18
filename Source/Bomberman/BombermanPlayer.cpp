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

	// TODO: Instantiate the players here, instead of placing them manually on the Unreal editor
	// in order to assure that they will appear on the "safety" zones of the board (currently the 
	// bottom-left and top-right corners) 

	FVector playerPosition = GetActorLocation();

	// For simplicity reasons, the tile size is 1 square unit
	// Unreal unit by default is centimeter
	col = FGenericPlatformMath::RoundToInt(playerPosition.Y * 0.01f);
	row = FGenericPlatformMath::RoundToInt(playerPosition.X * 0.01f);
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
	// If no movement there is nothing to do
	if( amount == 0.0f ) { return 0.0f; }

	// Check first the board boundaries
	if (!WillStayInsideTheBoard(true,amount)) { return 0.0f; }

	// If the displacement is not possible, just return 0 as the movement value
	if (!CanMove(true)) { return 0.0f; }

	FVector playerPosition = GetActorLocation();

	// The radius of the player in the direction of the desired movement
	float playerOffset = (amount < 0.0f ? -playerDiameter : playerDiameter) * 0.5f;

	// For simplicity reasons, the tile size is 1 square unit
	// Unreal unit by default is centimeter
	int newCol = FGenericPlatformMath::RoundToInt((playerPosition.Y + amount + playerOffset) * 0.01f);

	if (newCol != col)
	{
		// Check if the tile is currently "walkable"
		if (bombermanBoard == nullptr ) { return 0.0f; }
		if (!bombermanBoard->IsWalkableTile(newCol, row)) { return 0.0f; }

		// Only change the col if the center of the player is inside of the new col
		col = FGenericPlatformMath::RoundToInt((playerPosition.Y + amount) * 0.01f);
	}

	// Otherwise allow the full movement
	return amount;
}

// Filter to restrict the horizontal movement of the player on the Dyna Blaster board
// Removing the need of physics calculations in a very simple grid-based game type
float ABombermanPlayer::MoveVertically(float amount)
{
	// If no movement there is nothing to do
	if (amount == 0.0f) { return 0.0f; }

	// Check first the board boundaries
	if (!WillStayInsideTheBoard(false, amount)) { return 0.0f; }

	// If the displacement is not possible, just return 0 as the movement value
	if (!CanMove(false)) { return 0.0f; }

	FVector playerPosition = GetActorLocation();

	// The radius of the player in the direction of the desired movement
	float playerOffset = (amount < 0.0f ? -playerDiameter : playerDiameter) * 0.5f;

	// For simplicity reasons, the tile size is 1 square unit
	// Unreal unit by default is centimeter
	int newRow = FGenericPlatformMath::RoundToInt((playerPosition.X + amount + playerOffset) * 0.01f);

	if (newRow != row)
	{
		// Check if the tile is currently "walkable"
		if (bombermanBoard == nullptr) { return 0.0f; }
		if (!bombermanBoard->IsWalkableTile(col, newRow)) { return 0.0f; }

		// Only change the row if the center of the player is inside of the new row
		row = FGenericPlatformMath::RoundToInt((playerPosition.X + amount) * 0.01f);
	}

	// Otherwise allow the full movement
	return amount;
}

void ABombermanPlayer::SetBoard(ABombermanBoard * board)
{
	bombermanBoard = board;

	if (gameSettings != nullptr)
	{
		// Unreal unit by default is centimeter
		playerDiameter = gameSettings->PLAYER_COLLISION_DIAMETER_MT * 100.0f;
	}

	// The half of the amount of space between the player and the tile size
	float halfOfPlayerOffset = 0.5f * (100.0f - playerDiameter);

	// For simplicity reasons, the tile size is 1 square meters
	// The center of the first column and row is the origin
	minPlayerBoardX = -halfOfPlayerOffset;
	minPlayerBoardY = minPlayerBoardX;

	// For simplicity reasons, the tile size is 1 square meters
	// Unreal unit by default is centimeter
	float boardHeight = bombermanBoard->rows * 100.0f;
	float boardWidth = bombermanBoard->cols * 100.0f;

	maxPlayerBoardX = minPlayerBoardX + boardHeight - playerDiameter;
	maxPlayerBoardY = minPlayerBoardY + boardWidth - playerDiameter;
}

void ABombermanPlayer::DecreaseOnGoingBombs()
{
	ongoingBombs--;
}

bool ABombermanPlayer::IsInTile(int colToCheck, int rowToCheck)
{
	return (col == colToCheck && row == rowToCheck);
}

void ABombermanPlayer::DieOnNextFrame()
{
}

void ABombermanPlayer::AddToSpeed(float amount)
{
	if (gameSettings != nullptr)
	{
		speed = FMath::Clamp(speed + amount, gameSettings->MIN_SPEED, gameSettings->MAX_SPEED);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The modifier speed can't be assigned because the game settings are not available!"));
	}
}

void ABombermanPlayer::AddToBombCapacity(int amount)
{
	if (gameSettings != nullptr)
	{
		bombCapacity = FMath::Clamp(bombCapacity + amount, gameSettings->MIN_BOMB_CAPACITY, gameSettings->MAX_BOMB_CAPACITY);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The modifier bomb capacity can't be assigned because the game settings are not available!"));
	}
}

void ABombermanPlayer::AddToBlastLinearExtension(int amount)
{
	if (gameSettings != nullptr)
	{
		blastLinearExtension = FMath::Clamp(blastLinearExtension + amount, gameSettings->MIN_BLAST_EXTENSION, gameSettings->MAX_BLAST_EXTENSION);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The modifier for increase the blast range can't be assigned because the game settings are not available!"));
	}
}

void ABombermanPlayer::AddRemoteBomb()
{
	hasRemote = true;
}

void ABombermanPlayer::RemoveRemoteBomb()
{
	hasRemote = false;
}

bool ABombermanPlayer::CanMove(bool horizontally)
{
	// Sanity check, if the game settings are not available, then there is nothing to do
	if (gameSettings == nullptr) { return false; }

	// Unreal units are in cms
	FVector playerPositionMt = GetActorLocation() * 0.01f;

	float positionBetweenCorridors = (horizontally ? playerPositionMt.X : playerPositionMt.Y);

	// For simplicity reasons, the corridor width is 1 square meters
	int nearestCorridor = FGenericPlatformMath::RoundToInt(positionBetweenCorridors);

	float distanceToNearestCorridor = FGenericPlatformMath::Abs(positionBetweenCorridors - nearestCorridor);

	// TODO: Allow small movements, even if a wall is in front, because the diameter of the player is less
	// than the size of a board tile

	// If the player have a wall in front, the displacement is not possible
	// The game Dyna Blaster only allows movement in one of every two rows, in an altered way
	if (nearestCorridor % 2 != 0) { return false; }

	// If the player doesn't fit between walls, then the diplacement is not possible
	if (distanceToNearestCorridor > (1.0f - gameSettings->PLAYER_COLLISION_DIAMETER_MT))
	{
		return false;
	}

	return true;
}

// The desired movement will place the player inside of the board?
bool ABombermanPlayer::WillStayInsideTheBoard(bool horizontally, float movementAmount)
{
	if (bombermanBoard == nullptr) { return false; }

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

