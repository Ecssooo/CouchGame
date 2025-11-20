#include "Player/States/PlayerGrabState.h"

#include "Interfaces/Grabbable.h"
#include "Player/CharacterPlayer.h"
#include "Player/PlayerStateMachine.h"


EPlayerStateID UPlayerGrabState::GetStateID() const
{
	return EPlayerStateID::Grab;
	
}

void UPlayerGrabState::OnEnter(UPlayerStateMachine* InSM)
{
	Super::OnEnter(InSM);
	ACharacterPlayer* Player = GetPlayer();
	AActor* GrabbableActor = Player->GrabbableActor;
	if (!GrabbableActor) return;
	if (!GrabbableActor->Implements<UGrabbable>()) return;
	
	if (!IGrabbable::Execute_GetIsInSocket(GrabbableActor))
	{
		if (!Player->GetGrabParent()) return;
		IGrabbable::Execute_OnGrab(GrabbableActor, Player);
		GrabbableActor->AttachToComponent(Player->GetGrabParent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		//GrabbableActor->SetActorLocation(Player->GetGrabParent()->GetRelativeLocation());
		Player->GrabbableActor = GrabbableActor;
	}else
	{
		Player->StateMachine->ChangeState(EPlayerStateID::Idle);
	}
}

void UPlayerGrabState::OnExit(UPlayerStateMachine* InSM)
{
	Super::OnExit(InSM);
	
	ACharacterPlayer* Player = GetPlayer();
	AActor* GrabbableActor = Player->GrabbableActor;
	if (!GrabbableActor) return;
	if (!GrabbableActor->Implements<UGrabbable>()) return;
	
	IGrabbable::Execute_OnDrop(GrabbableActor, Player);
	GrabbableActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	GrabbableActor = nullptr;
}


