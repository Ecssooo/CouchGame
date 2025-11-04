#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateID.h"
#include "PlayerBaseState.generated.h"

class UPlayerStateMachine;
class ACharacterPlayer;

UCLASS(Abstract, Blueprintable, ClassGroup=(States), meta=(BlueprintSpawnableComponent))
class COUCHGAME_API UPlayerBaseState : public UActorComponent
{
    GENERATED_BODY()
public:
    UPlayerBaseState();
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool CanMove;
    
    virtual EPlayerStateID GetStateID() const PURE_VIRTUAL(UPlayerBaseState::GetStateID, return EPlayerStateID::Idle;);

    virtual void EnterState(UPlayerStateMachine* InSM);
    void ExitState(UPlayerStateMachine* InSM);
    void TickState(UPlayerStateMachine* InSM, float DeltaTime);

protected:
    virtual void OnEnter(UPlayerStateMachine* InSM) {}
    virtual void OnExit(UPlayerStateMachine* InSM) {}
    virtual void OnTick(UPlayerStateMachine* InSM, float DeltaTime) {}

    UFUNCTION(BlueprintCallable, Category="State")
    ACharacterPlayer* GetPlayer() const;

private:
    TWeakObjectPtr<UPlayerStateMachine> SM;
};
