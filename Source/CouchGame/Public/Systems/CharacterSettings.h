// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CharacterSettings.generated.h"

class UInputMappingContext;

USTRUCT()
struct FLocalMultiplayerProfile
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> IMCGamePad;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> IMCKeyboardP1;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> IMCKeyboardP2;
	
	int GetPlayerIndexFromKey(const FKey& Key) const;
};






class ACharacterPlayer;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName = "Character Settings"))
class COUCHGAME_API UCharacterSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category="LocalMultiplayer", BlueprintReadOnly)
	bool UsKeyboardControl = false;
	
	UPROPERTY(Config, EditAnywhere, Category="LocalMultiplayer")
	FLocalMultiplayerProfile IMCInGame;
	
	UPROPERTY(Config, EditAnywhere, Category="Character class")
	TSubclassOf<ACharacterPlayer> CharacterPlayer1;
	
	UPROPERTY(Config, EditAnywhere, Category="Character class")
	TSubclassOf<ACharacterPlayer> CharacterPlayer2;

	UPROPERTY(EditAnywhere, Config, Category="Move")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, Config, Category="Move")
	float RunSpeed;

	UPROPERTY(EditAnywhere, Config, Category="Move")
	float JumpPower;

	UPROPERTY(EditAnywhere, Config, Category="Move")
	float MoveDeadZone;

	UPROPERTY(EditAnywhere, Config, Category="Walk")
	float WalkSmoothedMaxSpeed; 

	UPROPERTY(EditAnywhere, Config, Category="Walk")
	float WalkAccelInterpSpeed;
	
	UPROPERTY(EditAnywhere, Config, Category="Walk")
	float WalkGroundFriction;
	
	UPROPERTY(EditAnywhere, Config, Category="Walk")
	float WalkBrakingFriction;
	
	UPROPERTY(EditAnywhere, Config, Category="Walk")
	float WalkBrakingDecelerationWalking;

	UPROPERTY(EditAnywhere, Config, Category="Run")
	float RunSmoothedMaxSpeed; 

	UPROPERTY(EditAnywhere, Config, Category="Run")
	float RunAccelInterpSpeed;
	
	UPROPERTY(EditAnywhere, Config, Category="Run")
	float RunGroundFriction;
	
	UPROPERTY(EditAnywhere, Config, Category="Run")
	float RunBrakingFriction;
	
	UPROPERTY(EditAnywhere, Config, Category="Run")
	float RunBrakingDecelerationWalking;
	
};
