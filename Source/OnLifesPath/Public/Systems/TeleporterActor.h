// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleporterActor.generated.h"

UCLASS()
class ONLIFESPATH_API ATeleporterActor : public AActor
{
	GENERATED_BODY()
#pragma region EU
public:
	// Sets default values for this actor's properties
	ATeleporterActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

public:
	UPROPERTY(EditAnywhere)
	int TeleporterID;

	UFUNCTION(BlueprintImplementableEvent)
	void HighlightTeleporter(bool IsHighlight);
};
