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
	Right UMETA(DisplayName="Right")
};

USTRUCT(BlueprintType)
struct FLevelNeighbors
{
	GENERATED_BODY()

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

USTRUCT(BlueprintType)
struct FCubeBasis
{
	GENERATED_BODY()

	float YawDeg = 0.f;

	void Turn(ELevelDir Dir)
	{
		switch (Dir)
		{
		case ELevelDir::Up:
			break;

		case ELevelDir::Down:
			break;

		case ELevelDir::Left:
			YawDeg += 90.f;
			break;

		case ELevelDir::Right:
			YawDeg -= 90.f;
			break;
		default:
			break;
		}

		// normalise pour rester entre 0 et 360
		if (YawDeg >= 360.f) YawDeg -= 360.f;
		if (YawDeg < 0.f) YawDeg += 360.f;
	}

	FRotator AsRotator() const
	{
		return FRotator(0.f, YawDeg, 0.f);
	}
};


UCLASS()
class COUCHGAME_API ALevelStreamerActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelStreamerActor();

	//tableau qui contient tout les levels renseigner en brut dans le bp_levelStreamerActor pr�sent dans la sc�ne main
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Streaming")
	TArray<FName> LevelSequence;

	//valeur qui donne le niveau actuel ou se trouve le joueur
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Streaming")
	FName CurrentLevel;

	//valeur en int qui donne le niveau actuel ou se trouve le joueur
	int32 CurrentLevelIndex = 0;

	//Stocke temporairement le nom du prochain niveau � charger
	FName NextLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Streaming")
	FName StartingLevel;

	// cré un tableau d'adjacence
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level Streaming")
	TMap<FName, FLevelNeighbors> Adjacency;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Level Streaming")
	void UnloadActualLevel();

	//fonction qui change le level � un level sp�cific
	UFUNCTION(BlueprintCallable, Category = "Level Streaming")
	void SwitchToSpecificLevel(FName NewLevelName);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FCubeBasis CubeBasis;

	UFUNCTION(BlueprintCallable)
	FRotator GetCurrentFaceRotation() const { return CubeBasis.AsRotator(); }
};
