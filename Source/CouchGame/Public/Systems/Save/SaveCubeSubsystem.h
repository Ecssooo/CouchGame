// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveCubeStruct.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveCubeSubsystem.generated.h"

class ASaveCubeManager;

UCLASS()
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

	UFUNCTION()
	void SaveInteractionData(int InInteractionID, bool InIsCompleted);

	UFUNCTION()
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
};
