// Fill out your copyright notice in the Description page of Project Settings.

#include "BombermanBoard.h"
#include "Engine/World.h"


// Sets default values
ABombermanBoard::ABombermanBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABombermanBoard::BeginPlay()
{
	Super::BeginPlay();
	GenerateBoard();
}

// Called every frame
void ABombermanBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		
		tiles.Emplace(new FBoardCol());

		for (int row = 0; row < newColRows; ++row)
		{
			// TODO: Make a public variable in the editor to set a different starting points for the players,
			// other than the board left-bottom corner and top-right corner

			// The row number if we had no indestructible walls in between on the odd columns
			int realRow = (col % 2 == 0 ? row : row * 2);

			// Don't create destructible tiles on the first player tile 
			// Or in the next at his adjacent tiles, because can cause a certain suicide situation for him
			if (col < 2 && realRow < 2) { continue; }

			// Do the same for the second player
			if (col > (cols - 3) && realRow > (rows - 3)) { continue; }

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
							tiles[col]->destructibleWalls.Emplace(newPowerUpWall);
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
						tiles[col]->destructibleWalls.Emplace(newEmptyWall);
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

