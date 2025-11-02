#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerStateID.h"
#include "CharacterPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class UPlayerStateMachine;

UCLASS(Blueprintable)
class COUCHGAME_API ACharacterPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterPlayer();
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category="Input")
	FVector2D PlayerMoveInput = FVector2D::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category="Input")
	bool IsRunPressed = false;

	UPROPERTY(BlueprintReadWrite, Category="Input")
	bool IsWantsJump = false;

	UPROPERTY(BlueprintReadWrite, Category="Input")
	bool IsWantsInteract = false;

	UFUNCTION(BlueprintCallable, Category="Input")
	void ConsumeJump() { IsWantsJump = false; }

	UFUNCTION(BlueprintCallable, Category="Input")
	void ConsumeInteract() { IsWantsInteract = false; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	UPlayerStateMachine* StateMachine = nullptr;

	UFUNCTION(BlueprintCallable, Category="Input")
	void OnJumpInput();

protected:

	void OnJumpTriggered(const FInputActionValue&);
	void OnInteractTriggered(const FInputActionValue&);
};
