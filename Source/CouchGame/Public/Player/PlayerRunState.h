#pragma once
#include "PlayerBaseState.h"
#include "PlayerRunState.generated.h"

UCLASS(ClassGroup=(States), meta=(BlueprintSpawnableComponent))
class COUCHGAME_API UPlayerRunState : public UPlayerBaseState
{
	GENERATED_BODY()
public:
	virtual EPlayerStateID GetStateID() const override { return EPlayerStateID::Run; }

protected:
	virtual void OnEnter(UPlayerStateMachine* InSM) override;
	virtual void OnTick(UPlayerStateMachine* InSM, float DeltaTime) override;

};
