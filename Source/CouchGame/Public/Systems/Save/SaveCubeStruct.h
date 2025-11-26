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

	bool IsCompleted;
};

#pragma endregion

#pragma region Struct | Object

UENUM()
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
};
#pragma endregion 
#pragma endregion 
