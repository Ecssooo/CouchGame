// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/LocalMultiplayerViewport/LocalMultiplayerViewPort.h"

#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/CharacterSettings.h"

class UEnhancedInputLocalPlayerSubsystem;

void ULocalMultiplayerViewPort::PostInitProperties()
{
	Super::PostInitProperties();
}

bool ULocalMultiplayerViewPort::InputKey(const FInputKeyEventArgs& EventArgs)
{
	
	//GetCharaSettings
	const UCharacterSettings* CharacterSettings = GetDefault<UCharacterSettings>();
	if (!CharacterSettings) return false;

	if (CharacterSettings->UsKeyboardControl)
	{
		//GetPlayerIndexFromKey
		int PlayerIndex = CharacterSettings->IMCInGame.GetPlayerIndexFromKey(EventArgs.Key);
		UE_LOG(LogTemp, Log, TEXT("Input Player index : %d"), PlayerIndex);
		if (PlayerIndex == -1) return false;
		
		//GetPlayer
		APlayerController* LocalPlayer = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex);
		if (!LocalPlayer) return false;

		//Use input
		FInputKeyParams params = FInputKeyParams(EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed);
		return LocalPlayer->InputKey(params);
	}
	return Super::InputKey(EventArgs);
	
}

bool ULocalMultiplayerViewPort::InputAxis(const FInputKeyEventArgs& Args)
{
	//GetCharaSettings
	const UCharacterSettings* CharacterSettings = GetDefault<UCharacterSettings>();
	if (!CharacterSettings) return false;
	if (CharacterSettings->UsKeyboardControl)
	{
		//GetPlayerIndexFromKey
		int PlayerIndex = CharacterSettings->IMCInGame.GetPlayerIndexFromKey(Args.Key);
		UE_LOG(LogTemp, Log, TEXT("Axis Input Player index : %d"), PlayerIndex);
		if (PlayerIndex == -1) return false;
		
		//GetPlayer
		APlayerController* LocalPlayer = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex);
		if (!LocalPlayer) return false;
		
		//Use input
		FInputKeyParams params = FInputKeyParams(Args.Key, Args.Event, Args.AmountDepressed);
		return LocalPlayer->InputKey(params);
	}
	return Super::InputAxis(
		GetWorld()->GetGameViewport()->Viewport,
		0,
		Args.Key,
		Args.AmountDepressed,
		GetWorld()->GetDeltaSeconds(),
		1,
		Args.Key.IsGamepadKey());
}
