// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerGameViewportClient.h"

#include "LocalMultiplayerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSettings.h"

void ULocalMultiplayerGameViewportClient::PostInitProperties()
{
	Super::PostInitProperties();
	MaxSplitscreenPlayers = 8;
}

bool ULocalMultiplayerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	//What happend here :
	// Platform Input (OS) -> Viewport (This class)-> GameViewportClient::InputKey() (This function)
	// -> LocalPlayer → PlayerController::InputKey() (End function)
	// ->InputComponent / EnhancedInput / Blueprint Events

	
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	if (!Settings) return false;
	
	ULocalMultiplayerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (!Subsystem) return false;

	//Get KeyboardProfile index (struct)
	int KeyboardProfileIndex = Settings->FindKeyboardProfileIndexFromKey(EventArgs.Key, ELocalMultiplayerInputMappingType::InGame);
	int PlayerIndex = -1;
	if (KeyboardProfileIndex != -1)
		{
		//Get player index from KeyboardProfileIndex
		PlayerIndex = Subsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);
		//If not already assign to a KeyboardProfile
		if (PlayerIndex == -1)
		{
			//Assign
			PlayerIndex = Subsystem->AssignNewPlayerToKeyboardProfile(KeyboardProfileIndex);
			//Assign IMC
			Subsystem->AssignKeyboardMapping(PlayerIndex, KeyboardProfileIndex, ELocalMultiplayerInputMappingType::InGame);
		}
	}

	if (EventArgs.Key.IsGamepadKey())
	{
		PlayerIndex = Subsystem->GetAssignedPlayerIndexFromGamepadDeviceID(EventArgs.ControllerId);
		if (PlayerIndex == -1)
		{
			PlayerIndex = Subsystem->AssignNewPlayerToGamepadDeviceID(EventArgs.ControllerId);
			Subsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);
		}
	}

	if (PlayerIndex == -1) return false;
	//Get PlayerController assign before
	APlayerController* PlayerController = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex);

	//Give the input to the PlayerController
	FInputKeyParams params = FInputKeyParams(EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed);
	return PlayerController->InputKey(params);	
}

bool ULocalMultiplayerGameViewportClient::InputAxis(const FInputKeyEventArgs& EventArgs)
{
	//What happend here :
	// Platform Input (OS) -> Viewport (This class)-> GameViewportClient::InputKey() (This function)
	// -> LocalPlayer → PlayerController::InputKey() (End function)
	// ->InputComponent / EnhancedInput / Blueprint Events

	
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	if (!Settings) return false;
	
	ULocalMultiplayerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (!Subsystem) return false;

	//Get KeyboardProfile index (struct)
	int KeyboardProfileIndex = Settings->FindKeyboardProfileIndexFromKey(EventArgs.Key, ELocalMultiplayerInputMappingType::InGame);
	int PlayerIndex = -1;
	if (KeyboardProfileIndex != -1)
	{
		//Get player index from KeyboardProfileIndex
		PlayerIndex = Subsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);
		//If not already assign to a KeyboardProfile
		if (PlayerIndex == -1)
		{
			//Assign
			PlayerIndex = Subsystem->AssignNewPlayerToKeyboardProfile(KeyboardProfileIndex);
			//Assign IMC
			Subsystem->AssignKeyboardMapping(PlayerIndex, KeyboardProfileIndex, ELocalMultiplayerInputMappingType::InGame);
		}
	}

	if (EventArgs.Key.IsGamepadKey())
	{
		PlayerIndex = Subsystem->GetAssignedPlayerIndexFromGamepadDeviceID(EventArgs.ControllerId);
		if (PlayerIndex == -1)
		{
			PlayerIndex = Subsystem->AssignNewPlayerToGamepadDeviceID(EventArgs.ControllerId);
			Subsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);
		}
	}

	if (PlayerIndex == -1) return false;
	//Get PlayerController assign before
	APlayerController* PlayerController = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex);

	//Give the input to the PlayerController
	FInputKeyParams params = FInputKeyParams(EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed);
	return PlayerController->InputKey(params);	
}
