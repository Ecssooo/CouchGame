// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Save/SaveCubeManager.h"

#include "Kismet/GameplayStatics.h"
#include "Systems/Save/SaveCubeSubsystem.h"
#include "Systems/Save/SaveSubLevelManager.h"

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
	for (FSubLevel SubLevel : StartingSubLevel)
	{
		UpdateLevelData(SubLevel.FaceID, SubLevel.SubLevelID, true);
	}
}

// Called every frame
void ASaveCubeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
#pragma endregion


void ASaveCubeManager::InitSaveSubsystem() const
{
	if (USaveCubeSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveCubeSubsystem>())
	{
		SaveSubsystem->InitLevelData();
		SaveSubsystem->InitInteractionsDatas(InteractionsDatas);
		SaveSubsystem->InitObjectsDatas(ObjectsDatas);
		SaveSubsystem->InitTeleporterDatas(TelporterDatas);
	}
}

void ASaveCubeManager::GetSaveSublevelManager()
{
	SubLevelManager = Cast<ASaveSubLevelManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASaveSubLevelManager::StaticClass()));
}

void ASaveCubeManager::UpdateLevelData(int idFace, int idSubFace, bool IsUnlocked)
{
	if (USaveCubeSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveCubeSubsystem>())
	{
		GetSaveSublevelManager();
		SaveSubsystem->SaveLevelData(idFace, idSubFace, IsUnlocked);
		if (!SubLevelManager) return;
		if (idFace == SubLevelManager->FaceID) SubLevelManager->UpdateSublevelState(idSubFace, IsUnlocked);
	}
}

