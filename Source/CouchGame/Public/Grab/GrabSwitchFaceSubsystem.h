// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GrabSwitchFaceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME_API UGrabSwitchFaceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

/*public:
	UPROPERTY()
	TSubclassOf<AGrabActor> GrabActorInPlayer1; 
	UPROPERTY()
	TSubclassOf<AGrabActor> GrabActorInPlayer2;

	UPROPERTY()
	ACharacterPlayer* Player1;
	
	UPROPERTY()
	ACharacterPlayer* Player2;

	void InitPlayer(ACharacterPlayer* InPlayer, int PlayerIndex);
	UFUNCTION()
	void SaveGrabObject(ACharacterPlayer* player, TSubclassOf<AGrabActor> grabActorClass);

	UFUNCTION()
	void LoadGrabObject(int playerIndex, ACharacterPlayer* Player);

	TSubclassOf<AGrabActor> GetGrabActorSubclass(int playerIndex);

	void ClearSubclass(ACharacterPlayer* player);*/
	
};
