// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Save/SaveSubLevelManager.h"

#include "Systems/PartitionLevel.h"
#include "Systems/TeleporterActor.h"
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
	UpdateAllPartitionLevelState();
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
	APartitionLevel* PL = GetPartitionLevelFromID(idSubLevel);
	if (!PL) return;
	if (IsUnlocked) PL->DiscoverSubLevel();
	else PL->UndiscoverSubLevel();
}

void ASaveSubLevelManager::UndiscoverAllSubLevel()
{
	for (APartitionLevel* PL : SubLevelsActor)
	{
		if (PL) PL->UndiscoverSubLevel();
	}
}

void ASaveSubLevelManager::UpdateAllPartitionLevelState()
{
	if (USaveCubeSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveCubeSubsystem>())
	{
		for (FSublevelCube sublevelData : SaveSubsystem->LevelsDatas[FaceID-1].SubLevels)
		{
			APartitionLevel* PL = GetPartitionLevelFromID(sublevelData.SubLevelId);
			if (sublevelData.IsUnlocked) PL->DiscoverSubLevel();
			else PL->UndiscoverSubLevel();
		}
	}
}

void ASaveSubLevelManager::UpdateAllTeleporterState()
{
	if (USaveCubeSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveCubeSubsystem>())
	{
		for (FTeleporterData TPData : SaveSubsystem->TeleporterDatas)
		{
			if (ATeleporterActor* TP = GetTeleporterFromID(TPData.TeleporterID))
			{
				TP->HighlightTeleporter(TPData.IsHighlight);
			}
		}
	}
}

ATeleporterActor* ASaveSubLevelManager::GetTeleporterFromID(int TeleporterID)
{
	for (ATeleporterActor* TP : TeleporterReferences)
	{
		if (TP->TeleporterID == TeleporterID) return TP;
	}
	return nullptr;
}
