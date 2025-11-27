#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterPlayer.generated.h"

class IInteractable;
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

	UPROPERTY()
	int PlayerIndex;
	
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


	//Interact / Grab Interface
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<AActor> InteractableActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<AActor> GrabbableActor;

	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* GrabParent;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GrabObject(AActor* actorToGrab);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DropObject(AActor* actorToGrab);
	
	UFUNCTION(BlueprintCallable)
	void SetGrabParent(UStaticMeshComponent* StaticMesh);

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetGrabParent();

	UPROPERTY()
	UAnimInstance* AnimBlueprint;
	UFUNCTION(BlueprintCallable)
	UAnimInstance* GetBlueprintAnim();
	UFUNCTION(BlueprintCallable)
	void SetBlueprintAnim(UAnimInstance* BP);

	UFUNCTION()
	void StopVelocity();
	
protected:

	void OnJumpTriggered(const FInputActionValue&);
	void OnInteractTriggered(const FInputActionValue&);
};
