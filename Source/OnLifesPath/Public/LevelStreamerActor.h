// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelStreamerActor.generated.h"

UENUM(BlueprintType)
enum class ELevelDir : uint8
{
	Up UMETA(DisplayName="Up"),
	Down UMETA(DisplayName="Down"),
	Left UMETA(DisplayName="Left"),
	Right UMETA(DisplayName="Right"),
	None UMETA(DisplayName="None")
};

USTRUCT(BlueprintType)
struct FLevelNeighbors
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ArrowIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Up;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Down;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Right;

	FName Get(ELevelDir Dir) const
	{
		switch (Dir)
		{
		case ELevelDir::Up: return Up;
		case ELevelDir::Down: return Down;
		case ELevelDir::Left: return Left;
		case ELevelDir::Right: return Right;
		default: return NAME_None;
		}
	}
};

UENUM(BlueprintType)
enum class EStartFace : uint8 { Top, Bottom, North, South, West, East };


class ACubeGameMode;
class ACubeController;

UCLASS()
class ONLIFESPATH_API ALevelStreamerActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelStreamerActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Streaming")
	TArray<FName> LevelSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Streaming")
	FName CurrentLevel;

	int32 CurrentLevelIndex = 0;

	FName NextLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Streaming")
	FName StartingLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level Streaming")
	TMap<FName, FLevelNeighbors> Adjacency;

	UPROPERTY()
	mutable FName CurrentLevelName;

	UPROPERTY()
	TObjectPtr<ACubeGameMode> GameMode;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ACubeController> CubeController;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Level Streaming")
	void UnloadActualLevel();

	//fonction qui change le level � un level sp�cific
	UFUNCTION(BlueprintCallable, Category = "Level Streaming")
	void SwitchToSpecificLevel(FName NewLevelName, ELevelDir Dir);

	//quand l'ancien niveau est d�charg� on appel cette fonction
	UFUNCTION()
	void OnLevelUnloaded();

	//on appel cette fonction pour charger le level
	UFUNCTION()
	void LoadLevel(FName NewLevelName);

	//quand le level est charger on appel cette fonction
	UFUNCTION()
	void OnLevelLoaded();

	UFUNCTION()
	void RotateLevel();

	UFUNCTION(BlueprintCallable, Category="Level Streaming")
	FName GetNeighborLevel(FName FromLevel, ELevelDir Dir) const;

	UFUNCTION()
	int GetArrowIndex();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private :
	ELevelDir TmpLevelDir;
};
