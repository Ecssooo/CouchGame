// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeController.generated.h"

enum class ELevelDir : uint8;
class UTPManager;

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

	UPROPERTY(EditAnywhere)
	float AnimationDuration = 1.f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TObjectPtr<UTPManager> TPManager;

	UFUNCTION(BlueprintCallable)
	void StartRotationQuat(ELevelDir StartDir, ELevelDir EndDir);

private :
	bool IsRotating = false;
	float Elapsed;

	FQuat StartQuat;
	FQuat TargetQuat;
	FQuat CurrentRotationQuat;

	ELevelDir CurrentStartLevelDir;
	ELevelDir CurrentEndLevelDir;

	void RotateStepAxis(float DeltaTime);
	static FVector AxisUnitVector(EAxis::Type Axis);
};
