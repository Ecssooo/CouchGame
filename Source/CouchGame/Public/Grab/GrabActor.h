// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Grabbable.h"
#include "GrabActor.generated.h"

UCLASS(Blueprintable)
class COUCHGAME_API AGrabActor : public AActor, public IGrabbable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrabActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	bool IsGrabbed;

	UPROPERTY(BlueprintReadWrite)
	bool IsInSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int SocketID;

	UPROPERTY(EditAnywhere)
	int ObjectID;
	
	UFUNCTION()
	virtual void OnGrab_Implementation(ACharacterPlayer* Player) override;

	UFUNCTION()
	virtual void OnDrop_Implementation(ACharacterPlayer* Player) override;

	UFUNCTION()
	virtual bool GetIsInSocket_Implementation() override;
};
