// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <limits>

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BombermanGameModeBase.h"
#include "BombermanBoard.h"
#include "BombermanPlayer.generated.h"

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

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<class ABombermanBoard> board = nullptr;

private:
	ABombermanGameModeBase* gameSettings = nullptr;

	// Max and min positions that a player can reach inside of the board
	float minPlayerBoardX = std::numeric_limits<float>::max();
	float maxPlayerBoardX = std::numeric_limits<float>::min();
	float minPlayerBoardY = std::numeric_limits<float>::max();
	float maxPlayerBoardY = std::numeric_limits<float>::min();

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Try to move the player horizontally
	UFUNCTION(BlueprintCallable)
	float MoveHorizontally(float amount);

	// Try to move the player vertically
	UFUNCTION(BlueprintCallable)
	float MoveVertically(float amount);

private:
	// Is possible to move horizontally / vertically?
	bool CanDisplace( bool horizontally );
	
	// The desired movement will place the player inside of the board?
	bool WillStayInsideTheBoard(bool horizontally, float movementAmount);
};
