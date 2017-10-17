// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombermanDestructibleWall.h"
#include "TriggerModifierOnProcessTimer.h"
#include "BombermanBoard.generated.h"

USTRUCT()
struct FBoardCol
{
	GENERATED_USTRUCT_BODY()

	TArray<ABombermanDestructibleWall*> destructibleWalls;
};

UCLASS()
class BOMBERMAN_API ABombermanBoard : public AActor
{
	GENERATED_BODY()

#pragma region [AActor]
public:	
	// Sets default values for this actor's properties
	ABombermanBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion [AActor]
	
public:
	// TODO: Add the functionlity of create the walls in this class and expose the cols and 
	// rows variables to allow the creation of different size boards
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int cols = 11;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int rows = 11;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<ABombermanDestructibleWall> emptyDestructibleWallBP;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TArray<TSubclassOf<ABombermanDestructibleWall>> destructibleWallsWithPowerUpBP;

	// Probability on the [0,1] range of having a destructible wall on a non-wall board tile 
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float normalizedProbabilityOfDestructibleWall = 0.0f;

	// Probability on the [0,1] range of having a wall on an board tile with a power-up inside
	// inside of a destructible wall to be collected after destroying the wall
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float normalizedProbabilityOfPowerUpInDestructibleWall = 0.3f;

private:
	TArray<FBoardCol*> tiles;
	TArray<TriggerModifierOnProcessTimer*> timers;

private:
	void GenerateBoard();
	bool IsUndestructibleWall(int col, int row);

public:
	bool IsWalkableTile(int col, int row);
	ABombermanDestructibleWall* GetTile(int col, int row);

	void RegisterTimer(TriggerModifierOnProcessTimer* timer);
};
