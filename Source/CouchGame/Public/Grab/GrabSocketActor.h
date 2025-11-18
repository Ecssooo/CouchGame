// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GrabSocketActor.generated.h"


class AGrabMultiSocket;
class AGrabActorSpawner;
class AGrabActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectPlaced, class AGrabSocketActor*, Socket);

UCLASS(Blueprintable)
class COUCHGAME_API AGrabSocketActor : public AActor
{
	GENERATED_BODY()
#pragma region UE
public:
	// Sets default values for this actor's properties
	AGrabSocketActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

#pragma region CG
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Socket Parameters")
	int id;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Socket Parameters")
	TSubclassOf<AGrabActor> ObjectType;

	UPROPERTY(EditAnywhere,Category="Socket Parameters")
	AGrabActorSpawner* ActorSpawner;

	UPROPERTY(BlueprintAssignable)
	FOnObjectPlaced OnObjectPlaced;
	
	UPROPERTY(BlueprintReadWrite)
	bool HasObjectInSocket;
protected:
	UPROPERTY(BlueprintReadWrite)
	AGrabActor* ActorInSocket;

	UPROPERTY(BlueprintReadWrite)
	AGrabActor* ActorInOverlap;

	UPROPERTY(BlueprintReadWrite)
	UBoxComponent* BoxCollision;
	
public:
	UFUNCTION(BlueprintCallable)
	void AttachObjectToSocket(AGrabActor* GrabActor);
	
	UFUNCTION(BlueprintCallable)
	void SaveState();

	UFUNCTION()
	void SpawnObject(bool IsInSocket);

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void SpawnObjectInSocket();

	UFUNCTION()
	void SpawnObjectInSpawners();

	#pragma endregion
};
