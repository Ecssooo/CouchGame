// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Timeline.generated.h"

class UEtape;

UCLASS()
class COUCHGAME_API ATimeline : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimeline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void StartEtape();

	UFUNCTION()
	void EndEtape();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> Etapes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int IndexEtape;

	UFUNCTION()
	void NextEtape();

	UFUNCTION()
	void FirstEtape();
};
