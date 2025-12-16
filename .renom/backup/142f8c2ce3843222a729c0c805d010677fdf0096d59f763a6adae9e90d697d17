// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveSubLevelManager.generated.h"

class ATeleporterActor;
class APartitionLevel;

UCLASS()
class COUCHGAME_API ASaveSubLevelManager : public AActor
{
	GENERATED_BODY()
#pragma region UE
public:
	// Sets default values for this actor's properties
	ASaveSubLevelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion


#pragma region CG

public:

	UPROPERTY(EditAnywhere)
	int FaceID;
	
	UPROPERTY(EditAnywhere)
	TArray<APartitionLevel*> SubLevelsActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ATeleporterActor*> TeleporterReferences;

	UFUNCTION()
	APartitionLevel* GetPartitionLevelFromID(int idSubLevel);

	UFUNCTION(BlueprintCallable)
	void UpdateSublevelState(int idSubLevel, bool IsUnlocked);
	UFUNCTION(BlueprintCallable)
	void UndiscoverAllSubLevel();

	UFUNCTION(BlueprintCallable)
	void UpdateAllPartitionLevelState();

	UFUNCTION(BlueprintCallable)
	void UpdateAllTeleporterState();

	UFUNCTION()
	ATeleporterActor* GetTeleporterFromID(int TeleporterID);
	
#pragma endregion
};
