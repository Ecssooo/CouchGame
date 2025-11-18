// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrabMultiSocket.generated.h"

class AGrabSocketActor;

UCLASS(Blueprintable)
class COUCHGAME_API AGrabMultiSocket : public AActor
{
	GENERATED_BODY()
#pragma region UE
public:
	// Sets default values for this actor's properties
	AGrabMultiSocket();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

#pragma region CG

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AGrabSocketActor*> GrabSockets;

private:
	UFUNCTION()
	void InitEvent();

	UFUNCTION()
	void GrabActorPlacedInSocket(AGrabSocketActor* socket);
#pragma endregion
};
