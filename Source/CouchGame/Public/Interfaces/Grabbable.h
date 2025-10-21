// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Grabbable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGrabbable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COUCHGAME_API IGrabbable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	//Add player in paramter
	void Grab();
};
