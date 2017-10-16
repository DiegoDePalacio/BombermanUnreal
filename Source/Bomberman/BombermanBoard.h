// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombermanBoard.generated.h"

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
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int cols = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int rows = 0;

};
