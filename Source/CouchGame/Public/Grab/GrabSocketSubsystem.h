// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrabSocketManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GrabSocketSubsystem.generated.h"

#pragma region Structs

USTRUCT()
struct FGrabSocketData
{
	GENERATED_BODY()
	int Id;
	bool HasObjectInSocket;
};

USTRUCT()
struct FGrabSocketLevelData
{
	GENERATED_BODY()
	int Id;
	bool IsDiscovered;
	TArray<FGrabSocketData> GrabSocketDatas;
};

#pragma endregion
/**
 * 
 */
UCLASS(Blueprintable)
class COUCHGAME_API UGrabSocketSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	//BlueprintReadOnly
	UPROPERTY()
	TArray<FGrabSocketLevelData> AllLevelDatas;

	UFUNCTION()
	void InitLevelDatas();

	UFUNCTION(BlueprintCallable)
	bool AddLevelData(int levelId, AGrabSocketManager* SocketManager);

	UFUNCTION(BlueprintCallable)
	void LoadLevelDataInSocket(int levelId, AGrabSocketManager* SocketManager);

	UFUNCTION(BlueprintCallable)
	void SaveSocketData(int LevelId, int SocketId);
};
