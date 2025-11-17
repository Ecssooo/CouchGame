// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrabSocketManager.generated.h"

class AGrabSocketActor;

UCLASS(Blueprintable)
class COUCHGAME_API AGrabSocketManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrabSocketManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int LevelId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AGrabSocketActor*> AllGrabSocketActors;

	// UFUNCTION()
	TObjectPtr<AGrabSocketActor> FindSocketFromId(int socketId);

	UFUNCTION(BlueprintCallable)
	void SaveSocketState();
	void LoadAllSocket();
};
