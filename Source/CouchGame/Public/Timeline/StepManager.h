// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StepManager.generated.h"

class AStep;
class ASaveCubeManager;

UCLASS()
class COUCHGAME_API AStepManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStepManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void StartStep();

	UFUNCTION()
	void EndStep();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AStep*> ListStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int IndexStep;

	UFUNCTION(BlueprintCallable, Category="StepManager")
	void NextStep();

	UFUNCTION(BlueprintCallable, Category="StepManager")
	void FirstStep();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ASaveCubeManager> SaveCubeManager;
};
