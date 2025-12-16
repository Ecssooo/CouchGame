// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrabActorSpawner.generated.h"

UCLASS()
class ONLIFESPATH_API AGrabActorSpawner : public AActor
{
	GENERATED_BODY()
#pragma region UE
public:
	// Sets default values for this actor's properties
	AGrabActorSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SpawnerID;
};
