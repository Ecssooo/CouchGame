// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Save/SaveCubeManager.h"

#include "Systems/Save/SaveCubeSubsystem.h"

#pragma region UE
// Sets default values
ASaveCubeManager::ASaveCubeManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASaveCubeManager::BeginPlay()
{
	Super::BeginPlay();
	InitSaveSubsystem();
}

// Called every frame
void ASaveCubeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
#pragma endregion


void ASaveCubeManager::InitSaveSubsystem()
{
	USaveCubeSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveCubeSubsystem>();
	if (SaveSubsystem)
	{
		SaveSubsystem->InitInteractionsDatas(InteractionsDatas);
		SaveSubsystem->InitLevelData();
	}
}

