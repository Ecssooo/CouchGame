// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Save/SaveObjectManager.h"


// Sets default values
ASaveObjectManager::ASaveObjectManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASaveObjectManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASaveObjectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

