// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrabSocketManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GrabSocketSubsystem.generated.h"


class AGrabSocketManager;

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

	UPROPERTY()
	TObjectPtr<AGrabSocketManager> SocketManager;
	
	UFUNCTION()
	void InitLevelDatas();
	
	UFUNCTION(BlueprintCallable)
	bool AddLevelData(int levelId, AGrabSocketManager* InSocketManager);

	UFUNCTION(BlueprintCallable)
	void LoadLevelDataInSocket(int levelId, AGrabSocketManager* InSocketManager);

	UFUNCTION(BlueprintCallable)
	void SaveSocketData(int LevelId, int SocketId);
};
