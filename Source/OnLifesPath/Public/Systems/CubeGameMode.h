// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/GameModeBase.h"
#include "CubeGameMode.generated.h"

class ACharacterPlayer;
class AStreamedCharacterSpawner;
class AMainTeleporterOut;
class AMainCharacterSpawner;
/**
 * 
 */
UCLASS(Blueprintable)
class ONLIFESPATH_API ACubeGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;
	
	UPROPERTY()
	TArray<AMainCharacterSpawner*> MainCharacterSpawners;
	UPROPERTY()
	TArray<AMainTeleporterOut*> MainTeleportersOut;
	UPROPERTY()
	TArray<AStreamedCharacterSpawner*> StreamedCharacterSpawners;

	UPROPERTY()
	TArray<ACharacterPlayer*> Players;
	
	UFUNCTION(BlueprintCallable)
	void SpawnCharacterAtBeginPlay();
	UFUNCTION(BlueprintCallable)
	void TeleportCharacterOut();
	UFUNCTION(BlueprintCallable)
	void SpawnCharacterInStreamedLevel(ELevelDir dir);
	UFUNCTION(BlueprintCallable)
	ACharacterPlayer* GetPlayerFromID(int PlayerID);

protected:
	void FindMainCharacterSpawners(TArray<AMainCharacterSpawner*>& MainCharacterSpawners);
	void FindMainTeleporterOut(TArray<AMainTeleporterOut*>& MainTeleportersOut);
	void FindStreamedCharacterSpawners(TArray<AStreamedCharacterSpawner*>& StreamedCharacterSpawners);

	UFUNCTION()
	TSubclassOf<ACharacterPlayer> GetCharacterClass(int PlayerIndex);
	UInputMappingContext* GetKeyboardProfile(int PlayerIndex);
};
