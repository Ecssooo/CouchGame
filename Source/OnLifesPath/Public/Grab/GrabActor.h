// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Grabbable.h"
#include "Systems/Save/SaveCubeStruct.h"
#include "GrabActor.generated.h"

class AGrabActorSocket;
class UBoxComponent;
struct FGrabObject;

UCLASS()
class ONLIFESPATH_API AGrabActor : public AActor, public IGrabbable
{
	GENERATED_BODY()
#pragma region UE
public:
	// Sets default values for this actor's properties
	AGrabActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

public:
	UPROPERTY(BlueprintReadWrite)
	FGrabObject ObjectData;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UUserWidget> GrabWidget;
	
	UFUNCTION(BlueprintCallable)
	bool Grab(ACharacterPlayer* Player);

	
	UFUNCTION(BlueprintCallable)
	bool Drop(ACharacterPlayer* Player); 
	
	virtual FGrabObject GetData_Implementation() override;
		
	UPROPERTY()
	TObjectPtr<AGrabActorSocket> GrabSocketInOverlap;
	
	UPROPERTY()
	UBoxComponent* BoxComponent;

	UFUNCTION(BlueprintImplementableEvent)
	void HighlightObject(bool IsHighlight);
		
	UFUNCTION()
	void OnBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnBoxEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
};
