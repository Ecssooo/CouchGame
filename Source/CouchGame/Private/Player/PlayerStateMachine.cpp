#include "Player/PlayerStateMachine.h"
#include "Player/PlayerBaseState.h"
#include "Player/CharacterPlayer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"

UPlayerStateMachine::UPlayerStateMachine()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerStateMachine::BeginPlay()
{
	Super::BeginPlay();
	CachedPlayer = Cast<ACharacterPlayer>(GetOwner());
	CacheStatesOnOwner();
	ChangeState(InitialState);
}

void UPlayerStateMachine::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CurrentState)
	{
		CurrentState->TickState(this, DeltaTime);
	}
}

void UPlayerStateMachine::CacheStatesOnOwner()
{
	StateMap.Empty();

	TArray<UActorComponent*> Comps;
	GetOwner()->GetComponents(UPlayerBaseState::StaticClass(), Comps);
	for (UActorComponent* C : Comps)
	{
		if (auto* S = Cast<UPlayerBaseState>(C))
		{
			StateMap.Add(S->GetStateID(), S);
			S->SetComponentTickEnabled(false);
		}
	}
}

UPlayerBaseState* UPlayerStateMachine::GetStateFromID(EPlayerStateID ID) const
{
	if (UPlayerBaseState* const* Found = StateMap.Find(ID))
	{
		return *Found;
	}
	return nullptr;
}

void UPlayerStateMachine::ChangeState(EPlayerStateID NewState)
{
	if (NewState == CurrentStateID && CurrentState != nullptr)
		return;

	const bool bFirstEnter = (CurrentState == nullptr);
	const EPlayerStateID Old = CurrentStateID;

	if (CurrentState)
	{
		CurrentState->ExitState(this);
		CurrentState = nullptr;
	}

	if (UPlayerBaseState* Next = GetStateFromID(NewState))
	{
		CurrentState = Next;
		CurrentStateID = NewState;
		CurrentState->EnterState(this);

		// Affiche les changemant
		if (!bFirstEnter && Old != NewState)
		{
			const UEnum* Enum = StaticEnum<EPlayerStateID>();
			const FString Msg = FString::Printf(TEXT("State: %s → %s"),
			                                    *Enum->GetNameStringByValue((int64)Old),
			                                    *Enum->GetNameStringByValue((int64)NewState));
			static const int32 Key = 3630;
			if (GEngine) { GEngine->AddOnScreenDebugMessage(Key, 1.2f, FColor::Yellow, Msg); }
		}
	}
}
