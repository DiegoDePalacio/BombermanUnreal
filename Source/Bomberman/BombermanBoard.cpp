// Fill out your copyright notice in the Description page of Project Settings.

#include "BombermanBoard.h"
#include "BombermanPlayer.h"
#include "Modifier.h"
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
		for (auto wall : tile->destructibleWalls)
		{
			if (wall != nullptr)
			{
				delete(wall);
			}
		}
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

	for (auto timer : timers)
	{
		timer->Update(DeltaTime);
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
				// For simplicity reasons, the tile size is 1 square meters
				// Unreal unit by default is centimeter
				FVector wallPosition = FVector(realRow * 100.0f, col * 100.0f, 0.0f);

				if(FGenericPlatformMath::FRand() < normalizedProbabilityOfPowerUpInDestructibleWall)
				{
					if (destructibleWallsWithPowerUpBP.Num() > 0)
					{
						// TODO: Make the randomness of the power-up tiles non-equal and configurable

						// Choose one index of the possible power-ups randomly
						int powerUpIndex = FMath::RandRange(0, destructibleWallsWithPowerUpBP.Num() - 1);

						if (destructibleWallsWithPowerUpBP[powerUpIndex] != nullptr)
						{
							ABombermanDestructibleWall* newPowerUpWall = GetWorld()
								->SpawnActor<ABombermanDestructibleWall>(destructibleWallsWithPowerUpBP[powerUpIndex], wallPosition, FRotator(0, 0, 0));
							tiles[col]->destructibleWalls.Add(newPowerUpWall);
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("There is a missing power-up wall in blueprint array on the board (%i)!"), powerUpIndex);
						}
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("The power-up wall blueprint array is empty on the board!"));
					}
				}
				else
				{
					if (emptyDestructibleWallBP != nullptr)
					{
						ABombermanDestructibleWall* newEmptyWall = GetWorld()
							->SpawnActor<ABombermanDestructibleWall>(emptyDestructibleWallBP, wallPosition, FRotator(0, 0, 0));
						tiles[col]->destructibleWalls.Add(newEmptyWall);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("The empty wall blueprint was not assigned on the board!"));
					}
				}
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

bool ABombermanBoard::SetModifier(Modifier * newModifier, int col, int row)
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
			delete(modifierCols[col]->modifiers[row]);
		}

		modifierCols[col]->modifiers[row] = newModifier;
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
			delete(modifierCols[col]->modifiers[rowIndex]);
		}

		modifierCols[col]->modifiers[rowIndex] = newModifier;
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

