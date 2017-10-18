// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <limits>

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BombermanGameModeBase.h"
#include "BombermanBoard.h"
#include "BombermanPlayer.generated.h"

class RemoteBombModifier;

UCLASS()
class BOMBERMAN_API ABombermanPlayer : public APawn
{
	GENERATED_BODY()

#pragma region [APawn]
public:
	// Sets default values for this pawn's properties
	ABombermanPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion [APawn]

private:
	ABombermanGameModeBase* gameSettings = nullptr;

	// Max and min positions that a player can reach inside of the board
	float minPlayerBoardX = std::numeric_limits<float>::max();
	float maxPlayerBoardX = std::numeric_limits<float>::min();
	float minPlayerBoardY = std::numeric_limits<float>::max();
	float maxPlayerBoardY = std::numeric_limits<float>::min();

	int col = -1;
	int row = -1;

	// Cached variables
	ABombermanBoard* bombermanBoard = nullptr;
	float playerDiameter = 0.0f;

	float speed = 1.0f;
	int bombCapacity = 1;
	int blastLinearExtension = 1;

	int ongoingBombs = 0;

	bool hasRemote = false;
	RemoteBombModifier* remoteBomb = nullptr;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Try to move the player horizontally
	UFUNCTION(BlueprintCallable)
	float MoveHorizontally(float amount);

	// Try to move the player vertically
	UFUNCTION(BlueprintCallable)
	float MoveVertically(float amount);

	// The action button was pressed
	UFUNCTION(BlueprintCallable)
	void ActionButtonPressed();

	void SetBoard(ABombermanBoard* board);

	void DecreaseOnGoingBombs();

	bool IsInTile(int colToCheck, int rowToCheck);

	// Don't kill instantaneously, because the other player can also be killed in the same frame
	// and then the game is a tie
	void DieOnNextFrame();

	// Methods used by the power-ups to modify the player variables
	void AddToSpeed(float amount);
	void AddToBombCapacity(int amount);
	void AddToBlastLinearExtension(int amount);

	void AddRemoteBomb();
	void RemoveRemoteBomb();

private:
	// Is possible to move horizontally / vertically?
	bool CanMove(bool horizontally);
	
	// The desired movement will place the player inside of the board?
	bool WillStayInsideTheBoard(bool horizontally, float movementAmount);

	// Trigger the Modifier.OnPlayerTouch of the modifiers that are on the way of the player while moving to a new column / row
	void TouchHorizontalModifiers(int newCol, int direction);
	void TouchVerticalModifiers(int newRow, int direction);
};
