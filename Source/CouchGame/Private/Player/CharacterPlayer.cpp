#include "Player/CharacterPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/PlayerIdleState.h"
#include "Player/PlayerStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ACharacterPlayer::ACharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	StateMachine = CreateDefaultSubobject<UPlayerStateMachine>(TEXT("StateMachine"));
}

void ACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->JumpZVelocity = JumpPower;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}


void ACharacterPlayer::OnMove(const FInputActionValue& Value)
{
	PlayerMoveInput = Value.Get<FVector2D>();

	// Déplacement
	const FRotator YawRot(0.f, GetControlRotation().Yaw, 0.f);
	const FVector Fwd = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

	AddMovementInput(Fwd, PlayerMoveInput.Y);
	AddMovementInput(Right, PlayerMoveInput.X);
}

void ACharacterPlayer::OnRunPressed(const FInputActionValue&) { bRunPressed = true; }
void ACharacterPlayer::OnRunReleased(const FInputActionValue&) { bRunPressed = false; }
void ACharacterPlayer::OnJumpTriggered(const FInputActionValue&) { bWantsJump = true; }
void ACharacterPlayer::OnInteractTriggered(const FInputActionValue&) { bWantsInteract = true; }
