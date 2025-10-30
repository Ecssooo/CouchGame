// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeController.generated.h"

UCLASS()
class COUCHGAME_API ACubeController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACubeController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// List<AActor*> Arrow;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
