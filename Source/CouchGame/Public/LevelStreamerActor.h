// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelStreamerActor.generated.h"

UCLASS()
class COUCHGAME_API ALevelStreamerActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelStreamerActor();

	//tableau qui contient tout les levels renseigner en brut dans le bp_levelStreamerActor présent dans la scéne main
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Streaming")
	TArray<FName> LevelSequence;

	//valeur qui donne le niveau actuel ou se trouve le joueur
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Streaming")
	FName CurrentLevel;

	//valeur en int qui donne le niveau actuel ou se trouve le joueur
	int32 CurrentLevelIndex;

	//Stocke temporairement le nom du prochain niveau à charger
	FName NextLevel;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//fonction qui change les levels à la suite dans un ordre
	UFUNCTION(BlueprintCallable, Category = "Level Streaming")
	void SwitchToNextLevel();

	//fonction qui change le level à un level spécific
	UFUNCTION(BlueprintCallable, Category = "Level Streaming")
	void SwitchToSpecificLevel(FName NewLevelName);

	//quand l'ancien niveau est déchargé on appel cette fonction
	UFUNCTION()
	void OnLevelUnloaded();

	//on appel cette fonction pour charger le level
	UFUNCTION()
	void LoadLevel(FName NewLevelName);

	//quand le level est charger on appel cette fonction
	UFUNCTION()
	void OnLevelLoaded();
};
