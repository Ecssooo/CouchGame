// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GrabSocketActor.generated.h"


class AGrabActor;

UCLASS(Blueprintable)
class COUCHGAME_API AGrabSocketActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrabSocketActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	int id;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Grab")
	TSubclassOf<AGrabActor> ObjectType;

	UPROPERTY(BlueprintReadWrite)
	AGrabActor* ActorInSocket;

	UPROPERTY(BlueprintReadWrite)
	AGrabActor* ActorInOverlap;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Grab")
	UBoxComponent* BoxCollision;

	UPROPERTY(BlueprintReadWrite)
	bool HasObjectInSocket;

	

	UFUNCTION(BlueprintCallable)
	void AttachObjectToSocket(AGrabActor* GrabActor);

	UFUNCTION(BlueprintCallable)
	void SaveState();

	UFUNCTION(BlueprintCallable)
	void SpawnObjectInSocket();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
