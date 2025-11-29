// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveCubeStruct.generated.h"


class AGrabActor;

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

USTRUCT(Blueprintable)
struct FSubLevel
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	int FaceID;
	UPROPERTY(EditAnywhere)
	int SubLevelID;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsHighlight;
	
	bool IsCompleted;
};

#pragma endregion

#pragma region Struct | Object

UENUM(Blueprintable)
enum EObjectState
{
	InSocket	UMETA(DisplayName="InSocket"),
	InSpawner	UMETA(DisplayName="InSpawner"),
	InPlayer	UMETA(DisplayName="InPlayer"),
	InWorld		UMETA(DisplayName="InWorld")
};

USTRUCT(Blueprintable)
struct FGrabObject
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	int ObjectID;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGrabActor> ObjectType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EObjectState> ObjectState;

	//Cube Position
	UPROPERTY(BlueprintReadWrite)
	int FaceID;
	
	UPROPERTY(BlueprintReadWrite)
	FVector Position;
	
	//Socket
	UPROPERTY(EditAnywhere)
	int SocketID;

	UPROPERTY(EditAnywhere)
	int SpawnerID;
	
	UPROPERTY(BlueprintReadWrite)
	int PlayerID;

	UPROPERTY(BlueprintReadWrite)
	bool IsHighlight;
	
	UPROPERTY(BlueprintReadWrite)
	bool IsSocketHighlight;
};
#pragma endregion

#pragma region Struct | TP


USTRUCT(Blueprintable)
struct FTeleporterData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TeleporterID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsHighlight;
};



#pragma endregion

#pragma endregion 
