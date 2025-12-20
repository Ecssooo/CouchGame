// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerSpawner.generated.h"

UCLASS(Blueprintable)
class ONLIFESPATH_API APlayerSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerSpawner();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int PlayerIndex;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EAutoReceiveInput::Type> Input;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
