// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BombermanGameModeBase.h"
#include "BombermanPlayer.generated.h"

UCLASS()
class BOMBERMAN_API ABombermanPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABombermanPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	ABombermanGameModeBase* gameSettings = nullptr;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Try to move the player horizontally
	UFUNCTION(BlueprintCallable)
	bool MoveHorizontally(float amount);

private:
	bool CanDisplaceHorizontally();
	

};
