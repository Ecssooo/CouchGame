// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CharacterSettings.generated.h"

class ACharacterPlayer;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName = "Character Settings"))
class COUCHGAME_API UCharacterSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category="Character class")
	TSubclassOf<ACharacterPlayer> CharacterPlayer1;
	
	UPROPERTY(Config, EditAnywhere, Category="Character class")
	TSubclassOf<ACharacterPlayer> CharacterPlayer2;

};
