#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelComunicationSubsystem.generated.h"

class APartitionLevel;
struct FSubCubeLevelOld;

USTRUCT(BlueprintType)
struct FCubeLevelOld
{
	GENERATED_BODY()
	int IdLevel;
	TArray<FSubCubeLevelOld> Sublevels;
};

USTRUCT(BlueprintType)
struct FSubCubeLevelOld
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

	TArray<FCubeLevelOld> AllLevels;
	
	UFUNCTION()
	void InitLevelData();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsTPVisible;
	
	UFUNCTION()
	FSubCubeLevelOld& FindSubLevel(int IdFace, int IdSubLevel);

	UFUNCTION(BlueprintCallable)
	void DiscoveredSubLevel(int IdFace, int IdSubLevel);
	
	// UFUNCTION(BlueprintCallable)
	// void AriseSubLevel(int id);
};
