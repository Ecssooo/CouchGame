// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveCubeManager.generated.h"

struct FSubLevel;
class ASaveSubLevelManager;
struct FGrabObject;
struct FInteractionsDatas;
struct FTeleporterData;

UCLASS(Blueprintable)
class COUCHGAME_API ASaveCubeManager : public AActor
{
	GENERATED_BODY()
#pragma region UE
public:
	// Sets default values for this actor's properties
	ASaveCubeManager();

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
	TArray<FSubLevel> StartingSubLevel;
	
	UPROPERTY(EditAnywhere)
	TArray<FInteractionsDatas> InteractionsDatas;

	UPROPERTY(EditAnywhere)
	TArray<FGrabObject> ObjectsDatas;
	
	UFUNCTION()
	void InitSaveSubsystem() const;

	UPROPERTY(EditAnywhere)
	TArray<FTeleporterData> TelporterDatas;

#pragma region Levels

	UPROPERTY()
	ASaveSubLevelManager* SubLevelManager;
	
	UFUNCTION()
	void GetSaveSublevelManager();
	
	UFUNCTION()
	void UpdateLevelData(int idFace, int idSubFace, bool IsUnlocked);

#pragma endregion

	
	
	#pragma endregion
};
