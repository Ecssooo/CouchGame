// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"

#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSettings.h"
#include "Kismet/GameplayStatics.h"

void ULocalMultiplayerSubsystem::CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	if (!Settings) return;
	for (int i = 0; i < Settings->NbMaxGamepads; i++)
	{
		UGameplayStatics::CreatePlayer(GetWorld(),i);
	}
}


#pragma region Keyboard
int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex) const
{
	if (PlayerIndexFromKeyboardProfileIndex.Contains(KeyboardProfileIndex))
	{
		return PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex];
	}
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
	LastAssignedPlayerIndex++;
	PlayerIndexFromKeyboardProfileIndex.Add(KeyboardProfileIndex, LastAssignedPlayerIndex);
	return LastAssignedPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	if (!Settings) return;

	//Get IMC
	UInputMappingContext* IMC = Settings->KeyboardProfilesData[KeyboardProfileIndex].GetIMCFromType(ELocalMultiplayerInputMappingType::InGame);

	//Get PlayerController assign to PlayerIndex (How unreal know IDK but OK all good :) )
	APlayerController* LocalPlayer = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex);
	if (!LocalPlayer) return;

	//Get EnhancedInputLocalPlayerSubsystem from LocalPlayer
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer->GetLocalPlayer());
	if (InputSubsystem)
	{
		//Add IMC to Subsystem
		InputSubsystem->AddMappingContext(IMC, 0);
	}
}
#pragma endregion


#pragma region Gamepad
int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID)
{
	if (PlayerIndexFromGamepadProfileIndex.Contains(DeviceID))
	{
		return PlayerIndexFromGamepadProfileIndex[DeviceID];
	}
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID)
{
	LastAssignedPlayerIndex++;
	PlayerIndexFromGamepadProfileIndex.Add(DeviceID, LastAssignedPlayerIndex);
	return LastAssignedPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	if (!Settings) return;

	//Get IMC
	UInputMappingContext* IMC = Settings->GamepadProfileData.GetIMCFromType(ELocalMultiplayerInputMappingType::InGame);

	//Get PlayerController assign to PlayerIndex (How unreal know IDK but OK all good :) )
	APlayerController* LocalPlayer = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex);
	if (!LocalPlayer) return;

	//Get EnhancedInputLocalPlayerSubsystem from LocalPlayer
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer->GetLocalPlayer());
	if (InputSubsystem)
	{
		//Add IMC to Subsystem
		InputSubsystem->AddMappingContext(IMC, 0);
	}
}

#pragma endregion
