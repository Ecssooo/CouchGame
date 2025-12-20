// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveObjectManager.generated.h"

class AGrabActor;
class AGrabActorSpawner;
class AGrabActorSocket;

UCLASS()
class ONLIFESPATH_API ASaveObjectManager : public AActor
{
	GENERATED_BODY()
#pragma region UE
public:
	// Sets default values for this actor's properties
	ASaveObjectManager();

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
	int LevelID;
	
	UPROPERTY(EditAnywhere)
	TArray<AGrabActorSocket*> SocketActors;
	
	UPROPERTY(EditAnywhere)
	TArray<AGrabActorSpawner*> SpawnersActors;

	UPROPERTY(BlueprintReadWrite)
	TArray<AGrabActor*> GrabActors;
	
	UFUNCTION()
	void LoadObjectData();

	UFUNCTION(BlueprintCallable)
	void UpdateAllObjectHighlight();	
		
	UFUNCTION()
	AGrabActorSocket* GetActorSocketFromID(int socketID);
	AGrabActorSpawner* GetActorSpawnerFromID(int socketID);
#pragma endregion
};

