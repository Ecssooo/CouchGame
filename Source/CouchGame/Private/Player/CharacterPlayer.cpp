#include "Player/CharacterPlayer.h"
#include "Player/PlayerStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Systems/CharacterSettings.h"

ACharacterPlayer::ACharacterPlayer()
{
	StateMachine = CreateDefaultSubobject<UPlayerStateMachine>(TEXT("StateMachine"));
	bUseControllerRotationYaw = false;
}

void ACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	const UCharacterSettings* Settings = GetDefault<UCharacterSettings>();
	GetCharacterMovement()->JumpZVelocity = Settings->JumpPower;

	auto* Move = GetCharacterMovement();
	Move->bOrientRotationToMovement = true;
	Move->bUseControllerDesiredRotation = false;
	Move->RotationRate = FRotator(0.f, 720.f, 0.f);
}


void ACharacterPlayer::SetGrabParent(UStaticMeshComponent* StaticMesh)
{
	if (!StaticMesh) return;
	GrabParent = StaticMesh;
}

UStaticMeshComponent* ACharacterPlayer::GetGrabParent()
{
	return GrabParent;
}

void ACharacterPlayer::StopVelocity()
{
	this->GetMovementComponent()->Velocity = FVector::Zero();
}

void ACharacterPlayer::OnJumpTriggered(const FInputActionValue&)
{
	IsWantsJump = true;
	if (StateMachine) { StateMachine->ChangeState(EPlayerStateID::Jump); }
}

void ACharacterPlayer::OnJumpInput()
{
	UE_LOG(LogTemp, Warning, TEXT("IA_Jump on %s (PC=%s)"),
	*GetName(), GetController() ? *GetController()->GetName() : TEXT("None"));
	IsWantsJump = true;
	if (StateMachine) { StateMachine->ChangeState(EPlayerStateID::Jump); }
}

void ACharacterPlayer::OnInteractTriggered(const FInputActionValue&)
{
	IsWantsInteract = true;
}
