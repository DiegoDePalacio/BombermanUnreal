// Fill out your copyright notice in the Description page of Project Settings.

#include "BombermanBoard.h"
#include "BombermanPlayer.h"
#include "Modifier.h"
#include "CollectSpeedModifier.h"
#include "CollectBombCapacityModifier.h"
#include "CollectBombBlastModifier.h"
#include "CollectRemoteBombModifier.h"
#include "EngineUtils.h"
#include "Engine/World.h"


// Sets default values
ABombermanBoard::ABombermanBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ABombermanBoard::~ABombermanBoard()
{
	// Clean the destructible walls
	for (auto tile : tiles)
	{
		delete(tile);
	}

	// Clean the modifiers
	for (auto modifierCol : modifierCols)
	{
		for (auto modifier : modifierCol->modifiers)
		{
			if (modifier != nullptr)
			{
				delete(modifier);
			}
		}
	}
}

// Called when the game starts or when spawned
void ABombermanBoard::BeginPlay()
{
	Super::BeginPlay();
	gameSettings = GetWorld()->GetAuthGameMode<ABombermanGameModeBase>();

	if (gameSettings == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Game settings not found!"));
	}

	InitModifiersMatrix();
	GenerateBoard();

	for (TActorIterator<ABombermanPlayer> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ABombermanPlayer* player = *ActorItr;
		players.Add(player);
		player->SetBoard(this);
	}
}

// Called every frame
void ABombermanBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = timers.Num() - 1; i >= 0; --i)
	{
		timers[i]->Update(DeltaTime);
	}
}

void ABombermanBoard::GenerateBoard()
{
	// TODO: Generate also the walls and the ground in order to allow different dimensions of the board, instead of being
	// fixed to a 11 x 11 board

	if (cols % 2 == 0 || rows % 2 == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("The board should contain and odd number of columns and rows!"));
	}

	// The game Dyna Blaster only can contain empty tiles in one of every two rows on the odd columns, in an altered way
	int rowsInOddColumns = FGenericPlatformMath::CeilToInt(cols / 2.0f);

	for (int col = 0; col < cols; ++col)
	{
		int newColRows = (col % 2 == 0 ? rows : rowsInOddColumns);
		
		tiles.Add(new FBoardDestructibleWallCol());

		for (int row = 0; row < newColRows; ++row)
		{
			// TODO: Make a public variable in the editor to set a different starting points for the players,
			// other than the board left-bottom corner and top-right corner

			// The row number if we had no indestructible walls in between on the odd columns
			int realRow = (col % 2 == 0 ? row : row * 2);

			// Don't create destructible tiles on the first player tile 
			// Or in the next at his adjacent tiles, because can cause a certain suicide situation for him
			if (col < 2 && realRow < 2) 
			{ 
				tiles[col]->destructibleWalls.Emplace(nullptr);
				continue; 
			}

			// Do the same for the second player
			if (col > (cols - 3) && realRow > (rows - 3)) 
			{
				tiles[col]->destructibleWalls.Emplace(nullptr);
				continue; 
			}

			if (FGenericPlatformMath::FRand() < normalizedProbabilityOfDestructibleWall)
			{
				FVector wallPosition = FVector(realRow * 100.0f, col * 100.0f, 0.0f);

				ABombermanDestructibleWall* newEmptyWall = GetWorld()
					->SpawnActor<ABombermanDestructibleWall>(emptyDestructibleWallBP, wallPosition, FRotator(0, 0, 0));
				tiles[col]->destructibleWalls.Add(newEmptyWall);
			}
			else
			{
				tiles[col]->destructibleWalls.Emplace(nullptr);
			}
		}
	}
}

// Create an empty "matrix" to be populated later with the modifiers to be used during the game
void ABombermanBoard::InitModifiersMatrix()
{
	int rowsInOddColumns = FGenericPlatformMath::CeilToInt(cols / 2.0f);

	for (int col = 0; col < cols; ++col)
	{
		modifierCols.Add(new FBoardModifierCol());

		int newColRows = (col % 2 == 0 ? rows : rowsInOddColumns);

		for (int row = 0; row < newColRows; ++row)
		{
			modifierCols[col]->modifiers.Emplace(nullptr);
		}
	}
}

bool ABombermanBoard::IsWalkableTile(int col, int row)
{
	return (GetTile(col, row) == nullptr);
}

bool ABombermanBoard::IsUndestructibleWall(int col, int row)
{
	return (col % 2 != 0 && row % 2 != 0);
}

bool ABombermanBoard::IsInBoard(int col, int row)
{
	if (col < 0) { return false; }
	if (col >= cols) { return false; }
	if (row < 0) { return false; }
	if (row >= rows) { return false; }
	return true;
}

// Create the visual model for the modifier on the provided position
void ABombermanBoard::SpawnModifierVisual(Modifier* modifier, int col, int row)
{
	// If there is no associated modifier, then there is nothing to spawn
	if (modifier == nullptr) { return; }

	TSubclassOf<AActor> visualToSpawn;

	// Cases shrinked to improve readability
	switch (modifier->type)
	{
		case EModifierType::BLAST:				{ visualToSpawn = visualBlast;				 break; }
		case EModifierType::OWN_REMOTE_BOMB:	{ visualToSpawn = visualRemoteBomb;			 break; }
		case EModifierType::OWN_TIME_BOMB:		{ visualToSpawn = visualTimeBomb;			 break; }
		case EModifierType::PERM_BOMB_BLAST:	{ visualToSpawn = visualBombBlastPowerUp;	 break; }
		case EModifierType::PERM_BOMB_CAPACITY: { visualToSpawn = visualBombCapacityPowerUp; break; }
		case EModifierType::PERM_SPEED:			{ visualToSpawn = visualSpeedPowerUp;		 break; }
		case EModifierType::TEMP_REMOTE_BOMB:	{ visualToSpawn = visualRemoteBombPowerUp;   break; }
		default: { return; } // No visual for the specified modifier
	}

	FVector visualPosition = FVector(row * 100.0f, col * 100.0f, 0.0f);

	modifier->visual = GetWorld()
		->SpawnActor<AActor>(visualToSpawn, visualPosition, FRotator(0, 0, 0));
}

void ABombermanBoard::SetTileAsEmpty(int col, int row)
{
	// Check first if it's outside of the board
	if (!IsInBoard(col, row)) { return; }

	// Check if is in a tile with a indestructible wall
	if (IsUndestructibleWall(col, row)) { return; }

	if (tiles.Num() <= col) { return; }

	// If is an even column, set directly on the row number of the FBoardDestructibleWallCol instance
	if (col % 2 == 0)
	{
		if (tiles[col]->destructibleWalls.Num() <= row) { return; }

		tiles[col]->destructibleWalls[row] = nullptr;
	}
	else
	{
		// If not, first calculate the respective index
		int rowIndex = FGenericPlatformMath::RoundToInt(row / 2.0f);

		if (tiles[col]->destructibleWalls.Num() <= rowIndex) { return; }

		tiles[col]->destructibleWalls[rowIndex] = nullptr;
	}
}

ABombermanDestructibleWall* ABombermanBoard::GetTile(int col, int row)
{
	// Check first if it's outside of the board
	if (!IsInBoard(col, row)) { return nullptr; }

	// Check if is in a tile with a indestructible wall
	if (IsUndestructibleWall(col, row)) { return nullptr; }

	if (tiles.Num() <= col) { return nullptr; }

	// If is an even column, check directly on the row number of the FBoardDestructibleWallCol instance
	if (col % 2 == 0)
	{
		if (tiles[col]->destructibleWalls.Num() <= row) { return nullptr; }

		return tiles[col]->destructibleWalls[row];
	}
	else
	{
		// If not, first calculate the respective index
		int rowIndex = FGenericPlatformMath::RoundToInt(row / 2.0f);

		if (tiles[col]->destructibleWalls.Num() <= rowIndex) { return nullptr; }

		return tiles[col]->destructibleWalls[rowIndex];
	}
}

void ABombermanBoard::RegisterTimer(TriggerModifierOnProcessTimer* timer)
{
	timers.Add(timer);
}

void ABombermanBoard::UnregisterTimer(TriggerModifierOnProcessTimer* timer)
{
	timers.Remove(timer);
}

Modifier* ABombermanBoard::GetModifier(int col, int row)
{
	// Check first if it's outside of the board
	if (!IsInBoard(col, row)) { return nullptr; }

	// Check if is in a tile with a indestructible wall
	if (IsUndestructibleWall(col, row)) { return nullptr; }

	if (modifierCols.Num() <= col) { return nullptr; }

	// If is an even column, check directly on the row number of the FBoardModifierCol instance
	if (col % 2 == 0)
	{
		if (modifierCols[col]->modifiers.Num() <= row) { return nullptr; }

		return modifierCols[col]->modifiers[row];
	}
	else
	{
		// If not, first calculate the respective index
		int rowIndex = FGenericPlatformMath::RoundToInt(row / 2.0f);

		if (modifierCols[col]->modifiers.Num() <= rowIndex) { return nullptr; }

		return modifierCols[col]->modifiers[rowIndex];
	}
}

bool ABombermanBoard::SetModifier(Modifier* newModifier, int col, int row)
{
	// Check first if it's outside of the board
	if (!IsInBoard(col, row)) 
	{ 
		UE_LOG(LogTemp, Error, TEXT("Trying to set a modifier outside of the board!"));
		return false;
	}

	// Check if is in a tile with a indestructible wall
	// This is not an error, because will be called on explosions
	if (IsUndestructibleWall(col, row)) 
	{ 
		return false; 
	}

	if (modifierCols.Num() <= col) 
	{ 
		UE_LOG(LogTemp, Error, TEXT("The modifier matrix was not initiated on the board!"));
		return false; 
	}

	// If is an even column, then the row number should be the same as the index on the FBoardModifierCol instance
	if (col % 2 == 0)
	{
		if (modifierCols[col]->modifiers.Num() <= row) 
		{
			UE_LOG(LogTemp, Error, TEXT("The modifier matrix was not initiated on the board!"));
			return false; 
		}

		// Destroy the previous modifier if any
		if (modifierCols[col]->modifiers[row] != nullptr)
		{
			modifierCols[col]->modifiers[row]->visual->Destroy();
			delete(modifierCols[col]->modifiers[row]);
			modifierCols[col]->modifiers[row] = nullptr;
		}

		modifierCols[col]->modifiers[row] = newModifier;
		SpawnModifierVisual(newModifier, col, row);
	}
	else
	{
		// If not, first calculate the respective index
		int rowIndex = FGenericPlatformMath::RoundToInt(row / 2.0f);

		if (modifierCols[col]->modifiers.Num() <= rowIndex) 
		{
			UE_LOG(LogTemp, Error, TEXT("The modifier matrix was not initiated on the board!"));
			return false;
		}

		// Destroy the previous modifier if any
		if (modifierCols[col]->modifiers[rowIndex] != nullptr)
		{
			modifierCols[col]->modifiers[rowIndex]->visual->Destroy();
			delete(modifierCols[col]->modifiers[rowIndex]);
			modifierCols[col]->modifiers[rowIndex] = nullptr;
		}

		modifierCols[col]->modifiers[rowIndex] = newModifier;
		SpawnModifierVisual(newModifier, col, row);
	}

	return true;
}

TArray<ABombermanPlayer*> ABombermanBoard::GetPlayersInTile(int col, int row)
{
	TArray<ABombermanPlayer*> playersInTile;

	for (auto player : players)
	{
		if (player->IsInTile(col, row))
		{
			playersInTile.Add(player);
		}
	}
	return playersInTile;
}

// Destroy a wall and return a power-up if is created behind the wall
Modifier* ABombermanBoard::DestroyWall(int col, int row)
{
	AActor* wallToDestroy = GetTile(col, row);

	// If there is no wall, then there is nothing to do
	if (wallToDestroy == nullptr) { return nullptr; }

	// Remove the visual model
	wallToDestroy->Destroy();
	SetTileAsEmpty(col, row);

	if (FGenericPlatformMath::FRand() < normalizedProbabilityOfPowerUpInDestructibleWall)
	{
		// TODO: Make the randomness of the power-up tiles non-equal and configurable
		// also replace this 

		// Choose one of the possible power-ups randomly
		int powerUpType = FMath::RandRange(0, AVAILABLE_POWER_UPS-1);

		Modifier* modifier = nullptr;

		if (gameSettings == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Game settings not found!"));
			return nullptr;
		}

		// Cases shrinked to improve readability
		switch (powerUpType)
		{
			case 1: { modifier = new CollectSpeedModifier(this, col, row, gameSettings->POWER_UP_SPEED); break; }
			case 2: { modifier = new CollectBombCapacityModifier(this, col, row, gameSettings->POWER_UP_BOMB_CAPACITY); break; }
			case 3: { modifier = new CollectBombBlastModifier(this, col, row, gameSettings->POWER_UP_BOMB_BLAST); break; }
			default: { modifier = new CollectRemoteBombModifier(this, col, row, gameSettings->POWER_UP_REMOTE_BOMB); break; }
		}

		int rowIndex = ( col % 2 == 0 ? row : FGenericPlatformMath::RoundToInt(row / 2.0f) );

		modifierCols[col]->modifiers[rowIndex] = modifier;
		SpawnModifierVisual(modifier, col, row);

		return modifier;
	}

	return nullptr;
}

// Is inside the board and not a indestructible wall?
bool ABombermanBoard::IsPlayableTile(int col, int row)
{
	if (!IsInBoard(col, row)) { return false; }

	// Is a tile without a indestructible wall?
	return (col % 2 == 0 || row % 2 == 0);
}

