
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"
class ACharacterPlayer;
class UUserWidget;
enum class EInputID : uint8;
// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COUCHGAME_API IInteractable
{
	GENERATED_BODY()

public:
	//<summary>Start interact</summary>
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void StartInteract(ACharacterPlayer* Player);

	//<summary>End interact</summary>
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void EndInteract(ACharacterPlayer* Player);
	
	//<summary>Input : Start interaction</summary>
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void OnStartInteract(ACharacterPlayer* Player, EInputID InputID);

	//<summary>During interaction </summary>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInteract(ACharacterPlayer* Player, EInputID InputID);

	//<summary>Interact completed</summary>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnEndInteract(ACharacterPlayer* Player, EInputID InputID);
	
	//<summary>Get parent to lock player</summary>
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UStaticMeshComponent* GetPlayerLockParent();

	//<summary>Get the teleporter to go out the interaction</summar>
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	USceneComponent* GetUnlockTP();
	
	//<summary>Player is lockable</summary>
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool LockedPlayer();
	
	//<summary>Player is locked</summary>
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool IsPlayerLocked();

	//<summary>Get the player lock in the interaction</summary>
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	ACharacterPlayer* GetCharacterInInteract();

	//<summary>Set the player lock in the interaction</summary>
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void SetCharacterInInteract(ACharacterPlayer* Player);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TSubclassOf<UUserWidget> GetInteractWidget();
};
