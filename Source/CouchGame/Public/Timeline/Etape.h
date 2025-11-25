// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Etape.generated.h"
#include "Systems/Save/SaveCubeStruct.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UEtape : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COUCHGAME_API IEtape
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartEtape();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndEtape();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSubLevel> SubLevelToSpawnAtEndEtape;
};
