// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeController.generated.h"

enum class ELevelDir : uint8;
class UTPManager;
class AArrowHelper;
class ALevelStreamerActor;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRotationEnd);


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

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> ActorCubeMinimap;

	UFUNCTION(BlueprintCallable)
	void StartRotationQuat(ELevelDir StartDir, ELevelDir EndDir);

	UFUNCTION(BlueprintCallable)
	AArrowHelper* GetArrow(int Index);

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<TObjectPtr<AArrowHelper>> ArrowHelpers;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ALevelStreamerActor> LevelStreamerActor;

	UFUNCTION()
	ELevelDir GetDirectionFromArrow(const UArrowComponent* Arrow);

	UFUNCTION()
	ELevelDir GetNewStartDir(ELevelDir StartDir, ELevelDir ArrowDir);

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnRotationEnd OnRotationEnd;
	
private :
	bool IsRotating = false;
	float Elapsed;

	FQuat StartQuat;
	FQuat TargetQuat;
	FQuat CurrentRotationQuat;

	ELevelDir CurrentStartLevelDir;
	ELevelDir CurrentStartTPLevelDir;
	ELevelDir CurrentEndLevelDir;

	void CollectArrowHelpers();

	void RotateStepAxis(float DeltaTime);
	static FVector AxisUnitVector(EAxis::Type Axis);
};
