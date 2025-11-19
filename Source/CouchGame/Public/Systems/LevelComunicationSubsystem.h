#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelComunicationSubsystem.generated.h"

class APartitionLevel;
struct FSubCubeLevel;

USTRUCT(BlueprintType)
struct FCubeLevel
{
	GENERATED_BODY()
	int IdLevel;
	TArray<FSubCubeLevel> Sublevels;
};

USTRUCT(BlueprintType)
struct FSubCubeLevel
{
	GENERATED_BODY()
	int IdSublevel;
	bool isDiscover;
};

UCLASS(Blueprintable)
class COUCHGAME_API ULevelComunicationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	
public:

	TArray<FCubeLevel> AllLevels;

	TArray<APartitionLevel*> AllPartitionLevels; 

	UFUNCTION()
	void InitLevelData();

	UFUNCTION()
	FSubCubeLevel& FindSubLevel(int IdFace, int IdSubLevel);

	UFUNCTION()
	void DiscoveredSubLevel(int IdFace, int IdSubLevel);

	UFUNCTION()
	void GetPartitionLevelsInWorld();

	UFUNCTION()
	APartitionLevel* FindPartitionLevel(int Id);
	
	UFUNCTION(BlueprintCallable)
	void AriseSubLevel(int id);
	
	UFUNCTION(BlueprintCallable)
	void LevelLoaded(int idFace);
};
