// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/LevelComunicationManager.h"

#include "Kismet/GameplayStatics.h"
#include "Systems/LevelComunicationSubsystem.h"
#include "Systems/PartitionLevel.h"

#pragma region UE
// Sets default values
ALevelComunicationManager::ALevelComunicationManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALevelComunicationManager::BeginPlay()
{
	Super::BeginPlay();

	ULevelComunicationSubsystem* ComunicationSubsystem = GetGameInstance()->GetSubsystem<ULevelComunicationSubsystem>();
	if (!ComunicationSubsystem) return;

	ComunicationSubsystem->InitLevelData();
	ComunicationSubsystem->DiscoveredSubLevel(1,0);	
	ComunicationSubsystem->DiscoveredSubLevel(1,3);	
	ComunicationSubsystem->DiscoveredSubLevel(1,4);	
	ComunicationSubsystem->DiscoveredSubLevel(2,1);	
	ComunicationSubsystem->DiscoveredSubLevel(2,4);	
	ComunicationSubsystem->DiscoveredSubLevel(2,6);	
	ComunicationSubsystem->DiscoveredSubLevel(2,7);	
	ComunicationSubsystem->DiscoveredSubLevel(3,7);	
	LoadDiscoveredLevelPartition();
}

// Called every frame
void ALevelComunicationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
#pragma endregion

void ALevelComunicationManager::LoadDiscoveredLevelPartition()
{	
	ULevelComunicationSubsystem* ComSubsystem = GetGameInstance()->GetSubsystem<ULevelComunicationSubsystem>();
	if (!ComSubsystem) return;
	for (int i = 0; i < PartitionLevels.Num(); i++)
	{
		if (ComSubsystem->AllLevels[LevelID].Sublevels[i].isDiscover) PartitionLevels[i]->DiscoverSubLevel();
	}
}

void ALevelComunicationManager::LoadSpecificPartition(int idSub)
{
	PartitionLevels[idSub]->DiscoverSubLevel();
}

void ALevelComunicationManager::GetPartitionLevelsInWorld()
{
	TArray<AActor*> FoundActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APartitionLevel::StaticClass(), FoundActor);
	PartitionLevels.Empty();
	for (int i = 0; i < FoundActor.Num(); i++)
	{
		APartitionLevel* PartitionLevel = Cast<APartitionLevel>(FoundActor[i]);
		if (!PartitionLevel) continue;
		PartitionLevels.Add(PartitionLevel);
	}
}

APartitionLevel* ALevelComunicationManager::FindPartitionLevel(int Id)
{
	for (int i = 0; i < PartitionLevels.Num(); i++ )
	{
		if (Id == PartitionLevels[i]->IdSublevel)
		{
			return PartitionLevels[i];
		}
	}
	return nullptr;
}