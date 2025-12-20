// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/PlayerSpawner.h"
#include "MainCharacterSpawner.generated.h"

UCLASS(Blueprintable)
class COUCHGAME_API AMainCharacterSpawner : public APlayerSpawner
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMainCharacterSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
