// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveInteractionManager.generated.h"

class AInteraction;

UCLASS()
class COUCHGAME_API ASaveInteractionManager : public AActor
{
	GENERATED_BODY()
#pragma region UE
public:
	// Sets default values for this actor's properties
	ASaveInteractionManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion


#pragma region CG

public:
	UPROPERTY(EditAnywhere)
	TArray<AInteraction*> LevelInteractions;

	UFUNCTION()
	void UpdateInteractionDatas(int idInteraction, bool InIsCompleted) const;
#pragma endregion
};
