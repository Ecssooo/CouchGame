#include "Player/CharacterPlayer.h"
#include "Player/PlayerStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ACharacterPlayer::ACharacterPlayer()
{
	StateMachine = CreateDefaultSubobject<UPlayerStateMachine>(TEXT("StateMachine"));
}

void ACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->JumpZVelocity = JumpPower;
}

void ACharacterPlayer::OnJumpTriggered(const FInputActionValue&)
{
	IsWantsJump = true;
	if (StateMachine) { StateMachine->ChangeState(EPlayerStateID::Jump); }
}

void ACharacterPlayer::OnInteractTriggered(const FInputActionValue&)
{
	IsWantsInteract = true;
}

void ACharacterPlayer::OnJumpInput()
{
	IsWantsJump = true;
	if (StateMachine) { StateMachine->ChangeState(EPlayerStateID::Jump); }
}

