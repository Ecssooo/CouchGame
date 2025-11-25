// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Save/SaveSubLevelManager.h"

#include "Systems/PartitionLevel.h"
#include "Systems/Save/SaveCubeSubsystem.h"


// Sets default values
ASaveSubLevelManager::ASaveSubLevelManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASaveSubLevelManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASaveSubLevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

APartitionLevel* ASaveSubLevelManager::GetPartitionLevelFromID(int idSubLevel)
{
	for (APartitionLevel* PL : SubLevelsActor)
	{
		if (PL->IdSublevel == idSubLevel) return PL;
	}
	return nullptr;
}

void ASaveSubLevelManager::UpdateSublevelState(int idSubLevel, bool IsUnlocked)
{
	GetPartitionLevelFromID(idSubLevel)->DiscoverSubLevel();
}

void ASaveSubLevelManager::UpdateAllPartitionLevelState()
{
	if (USaveCubeSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveCubeSubsystem>())
	{
		for (FSublevelCube sublevelData : SaveSubsystem->LevelsDatas[FaceID-1].SubLevels)
		{
			if (sublevelData.IsUnlocked)
			{
				GetPartitionLevelFromID(sublevelData.SubLevelId)->DiscoverSubLevel();
			}
		}
	}
}



