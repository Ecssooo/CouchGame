// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelComunicationManager.generated.h"

class APartitionLevel;

UCLASS(Blueprintable)
class COUCHGAME_API ALevelComunicationManager : public AActor
{
	GENERATED_BODY()
#pragma region UE
public:
	// Sets default values for this actor's properties
	ALevelComunicationManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

public:
	UPROPERTY(EditAnywhere)
	int LevelID;
	
	UPROPERTY(EditAnywhere)
	TArray<APartitionLevel*> PartitionLevels;

	UFUNCTION()
	void LoadDiscoveredLevelPartition();
	UFUNCTION()
	void LoadSpecificPartition(int idSub);
	UFUNCTION()
	void GetPartitionLevelsInWorld();
	UFUNCTION()
	APartitionLevel* FindPartitionLevel(int Id);
	
};
