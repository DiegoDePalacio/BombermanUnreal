// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombermanGameModeBase.h"
#include "BombermanDestructibleWall.h"
#include "TriggerModifierOnProcessTimer.h"
#include "BombermanBoard.generated.h"

// Forward declaration in order to avoid circular dependencies
class Modifier;
class ABombermanPlayer;

USTRUCT()
struct FBoardDestructibleWallCol
{
	GENERATED_USTRUCT_BODY()
	TArray<ABombermanDestructibleWall*> destructibleWalls;
};

USTRUCT()
struct FBoardModifierCol
{
	GENERATED_USTRUCT_BODY()
	TArray<Modifier*> modifiers;
};

UCLASS()
class BOMBERMAN_API ABombermanBoard : public AActor
{
	GENERATED_BODY()

#pragma region [AActor]
public:	
	// Sets default values for this actor's properties
	ABombermanBoard();
	~ABombermanBoard();

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

	// TODO: Replace these visual variables for an array of a custom class with modifiable probabilities
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<AActor> visualBlast;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<AActor> visualTimeBomb;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<AActor> visualRemoteBomb;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<AActor> visualSpeedPowerUp;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<AActor> visualBombCapacityPowerUp;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<AActor> visualBombBlastPowerUp;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<AActor> visualRemoteBombPowerUp;

	// Probability on the [0,1] range of having a destructible wall on a non-wall board tile 
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float normalizedProbabilityOfDestructibleWall = 0.0f;

	// Probability on the [0,1] range of having a wall on an board tile with a power-up inside
	// inside of a destructible wall to be collected after destroying the wall
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float normalizedProbabilityOfPowerUpInDestructibleWall = 0.3f;

private:
	ABombermanGameModeBase* gameSettings = nullptr;

	TArray<FBoardDestructibleWallCol*> tiles;
	TArray<TriggerModifierOnProcessTimer*> timers;
	TArray<FBoardModifierCol*> modifierCols;
	TArray<ABombermanPlayer*> players;

	// TODO: Make this available and configurable in the editor in order to be able to choose which 
	// power-ups will appear on a current level

	// The number of power ups implemented on the game
	const int AVAILABLE_POWER_UPS = 4;

private:
	void GenerateBoard();
	void InitModifiersMatrix();
	bool IsUndestructibleWall(int col, int row);
	bool IsInBoard(int col, int row);
	void SpawnModifierVisual(Modifier* modifier, int col, int row);
	void SetTileAsEmpty(int col, int row);

public:
	bool IsWalkableTile(int col, int row);

	ABombermanDestructibleWall* GetTile(int col, int row);

	void RegisterTimer(TriggerModifierOnProcessTimer* timer);
	void UnregisterTimer(TriggerModifierOnProcessTimer* timer);

	Modifier* GetModifier(int col, int row);
	bool SetModifier(Modifier* newModifier, int col, int row);

	TArray<ABombermanPlayer*> GetPlayersInTile(int col, int row);

	// Destroy a wall and return a power-up if is created behind the wall
	Modifier* DestroyWall(int col, int row);

	// Is inside the board and not a indestructible wall?
	bool IsPlayableTile(int col, int row);
};