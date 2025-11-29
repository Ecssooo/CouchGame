// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveCubeStruct.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveCubeSubsystem.generated.h"

class ATeleporterActor;
class ASaveCubeManager;

UCLASS(Blueprintable)
class COUCHGAME_API USaveCubeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
#pragma region Level
public:
	UPROPERTY()
	TArray<FLevelCube> LevelsDatas;

	FSublevelCube nullSubLevelDatas;
	
	UFUNCTION()
	void InitLevelData();

	UFUNCTION()
	void SaveLevelData(int idFace, int idSubLevel, bool isUnlocked);

	UFUNCTION()
	FSublevelCube& GetSublevelDatas(int idFace, int idSubLevel);
#pragma endregion

#pragma region Interactions
public:
	UPROPERTY()
	TArray<FInteractionsDatas> InteractionsDatas;

	FInteractionsDatas nullInteractionsDatas;
	
	UFUNCTION()
	void InitInteractionsDatas(TArray<FInteractionsDatas> InInteractionsDatas);

	UFUNCTION(BlueprintCallable)
	void SaveInteractionData(int InInteractionID, bool InIsCompleted);

	UFUNCTION(BlueprintCallable)
	void SetInteractionHighlight(int InInteractionID, bool InIsHighlight);

	UFUNCTION(BlueprintCallable)
	FInteractionsDatas& GetInteractionsDatasFromID(int InInteractionID);
#pragma endregion

#pragma region Object

public:
	UPROPERTY()
	TArray<FGrabObject> AllObjectsDatas;
	
	UFUNCTION()
	void InitObjectsDatas(TArray<FGrabObject> InObjectsDatas);
	UFUNCTION(BlueprintCallable)
	void SetObjectState(int ObjectID, EObjectState InObjectState, int Id);
	void SetObjectState(int ObjectID, EObjectState InObjectState, int FaceID, FVector Position);
private:
	FGrabObject* GetGrabObjectFromID(int InObjectID);
#pragma endregion

#pragma region Teleporter

public:
	UPROPERTY()
	TArray<FTeleporterData> TeleporterDatas;

	UFUNCTION(BlueprintCallable)
	void SetTeleporterDatas(int TeleporterID, bool InIsHighlight);

	FTeleporterData* GetTeleporterDatas(int TeleporterID);

	UFUNCTION(BlueprintCallable)
	void InitTeleporterDatas(TArray<FTeleporterData> InTeleporterDatas);
#pragma endregion
};
