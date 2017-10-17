// Fill out your copyright notice in the Description page of Project Settings.

#include "BombermanDestructibleWall.h"


// Sets default values
ABombermanDestructibleWall::ABombermanDestructibleWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABombermanDestructibleWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABombermanDestructibleWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABombermanDestructibleWall::Destroy()
{
	// TODO: Instantiate the power-up if any in this tile
}

