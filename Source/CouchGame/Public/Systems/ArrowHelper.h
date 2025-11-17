// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArrowHelper.generated.h"

class UArrowComponent;

UCLASS()
class COUCHGAME_API AArrowHelper : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArrowHelper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> Arrow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Arrow")
	int32 FaceIndex = -1;
};
