#include "Player/PlayerStateMachine.h"
#include "Player/PlayerBaseState.h"
#include "Player/CharacterPlayer.h"
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
	
	if (IsVerboseLogs)
	{
		const UEnum* Enum = StaticEnum<EPlayerStateID>();
		const FString Name = Enum ? Enum->GetNameStringByValue((int64)CurrentStateID) : TEXT("None");
		UE_LOG(LogTemp, Log, TEXT("[SM] BeginPlay: Initial State = %s"), *Name);
	}
}

void UPlayerStateMachine::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CurrentState)
	{
		if (IsVerboseLogs)
		{
			const UEnum* Enum = StaticEnum<EPlayerStateID>();
			const FString Name = Enum ? Enum->GetNameStringByValue((int64)CurrentStateID) : TEXT("None");
			UE_LOG(LogTemp, Log, TEXT("[SM] Tick: %s"), *Name);
		}

		CurrentState->TickState(this, DeltaTime);
	}
}

UPlayerBaseState* UPlayerStateMachine::GetCurrentPlayerState()
{
	return CurrentState;
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

			if (IsVerboseLogs)
			{
				const UEnum* Enum = StaticEnum<EPlayerStateID>();
				const FString Name = Enum ? Enum->GetNameStringByValue((int64)S->GetStateID()) : TEXT("Unknown");
				UE_LOG(LogTemp, Log, TEXT("[SM] Cached state component: %s"), *Name);
			}
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

		const UEnum* Enum = StaticEnum<EPlayerStateID>();
		const FString OldName = Enum ? Enum->GetNameStringByValue((int64)Old) : TEXT("None");
		const FString NewName = Enum ? Enum->GetNameStringByValue((int64)NewState) : TEXT("None");

		if (IsPrintStateChanges)
		{
			if (!bFirstEnter && Old != NewState)
			{
				static const int32 Key = 3630;
				if (GEngine) { GEngine->AddOnScreenDebugMessage(Key, 1.2f, FColor::Yellow, FString::Printf(TEXT("State: %s -> %s"), *OldName, *NewName)); }
				UE_LOG(LogTemp, Log, TEXT("[SM] State change: %s -> %s"), *OldName, *NewName);
			}
			else if (bFirstEnter)
			{
				static const int32 KeyInit = 3631;
				if (GEngine) { GEngine->AddOnScreenDebugMessage(KeyInit, 1.2f, FColor::Yellow, FString::Printf(TEXT("State: %s"), *NewName)); }
				UE_LOG(LogTemp, Log, TEXT("[SM] Initial state: %s"), *NewName);
			}
		}
	}
}
