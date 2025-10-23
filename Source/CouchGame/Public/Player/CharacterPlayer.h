#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerStateID.h"
#include "CharacterPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class UPlayerStateMachine;

UCLASS()
class COUCHGAME_API ACharacterPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterPlayer();

	virtual void BeginPlay() override;

	//Variable SM
	UPROPERTY(BlueprintReadWrite, Category="Input")
	FVector2D PlayerMoveInput = FVector2D::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category="Input")
	bool bRunPressed = false;

	UPROPERTY(BlueprintReadWrite, Category="Input")
	bool bWantsJump = false;

	UPROPERTY(BlueprintReadWrite, Category="Input")
	bool bWantsInteract = false;

	UFUNCTION(BlueprintCallable, Category="Input")
	void ConsumeJump() { bWantsJump = false; }

	UFUNCTION(BlueprintCallable, Category="Input")
	void ConsumeInteract() { bWantsInteract = false; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Move")
	float WalkSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Move")
	float RunSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Move")
	float JumpPower = 420.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	UPlayerStateMachine* StateMachine = nullptr;

protected:
	// callbacks inputs
	void OnMove(const FInputActionValue& Value);
	void OnRunPressed(const FInputActionValue&);
	void OnRunReleased(const FInputActionValue&);
	void OnJumpTriggered(const FInputActionValue&);
	void OnInteractTriggered(const FInputActionValue&);
};
