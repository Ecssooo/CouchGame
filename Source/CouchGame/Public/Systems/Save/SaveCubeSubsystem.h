// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveCubeStruct.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveCubeSubsystem.generated.h"

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
};
