// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/PlayerSpawners/MainCharacterSpawner.h"


// Sets default values
AMainCharacterSpawner::AMainCharacterSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMainCharacterSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

