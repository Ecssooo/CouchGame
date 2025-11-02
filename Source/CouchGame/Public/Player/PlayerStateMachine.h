#pragma once
#include "CoreMinimal.h"
#include "CharacterPlayer.h"
#include "Components/ActorComponent.h"
#include "PlayerStateID.h"
#include "PlayerStateMachine.generated.h"

class UPlayerBaseState;
class ACharacterPlayer;

UCLASS(ClassGroup=(States), meta=(BlueprintSpawnableComponent))

class COUCHGAME_API UPlayerStateMachine : public UActorComponent
{
    GENERATED_BODY()

public:
    UPlayerStateMachine();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category="State")
    void ChangeState(EPlayerStateID NewState);

    UFUNCTION(BlueprintPure, Category="State")
    UPlayerBaseState* GetStateFromID(EPlayerStateID ID) const;

    UFUNCTION(BlueprintPure, Category="State")
    ACharacterPlayer* GetPlayer() const { return CachedPlayer.Get(); }

    UFUNCTION(BlueprintPure, Category="State")
    EPlayerStateID GetCurrentStateID_BP() const { return CurrentStateID; }

    UPROPERTY(EditAnywhere, Category="States")
    EPlayerStateID InitialState = EPlayerStateID::Idle;



    // debug
    UPROPERTY(EditAnywhere, Category="Debug")
    bool IsPrintStateChanges = true;

    UPROPERTY(EditAnywhere, Category="Debug")
    bool IsVerboseLogs = true;

private:
    void CacheStatesOnOwner();

    TWeakObjectPtr<ACharacterPlayer> CachedPlayer;

    UPROPERTY() UPlayerBaseState* CurrentState = nullptr;
    EPlayerStateID CurrentStateID = EPlayerStateID::Idle;

    UPROPERTY() TMap<EPlayerStateID, UPlayerBaseState*> StateMap;
};
