// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveCubeSubsystem.generated.h"

#pragma region Struct

#pragma region Struct | Level

enum class EInteractionsID : uint8;

USTRUCT()
struct FSublevelCube
{
	GENERATED_BODY()
	int SubLevelId;
	bool IsUnlocked;
};

USTRUCT()
struct FLevelCube
{
	GENERATED_BODY()
	int LevelId;
	TArray<FSublevelCube> SubLevels;
};

#pragma endregion

#pragma region Struct | Interactions

USTRUCT(Blueprintable)
struct FInteractionsDatas
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int InteractionIndex;
	
	UPROPERTY(EditAnywhere)
	EInteractionsID InteractionsID;

	bool IsCompleted;
};

#pragma endregion

#pragma region Struct | Object




#pragma endregion 
#pragma endregion 

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
