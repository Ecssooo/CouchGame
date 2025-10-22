#pragma once
#include "PlayerBaseState.h"
#include "PlayerWalkState.generated.h"

UCLASS(ClassGroup=(States), meta=(BlueprintSpawnableComponent))
class COUCHGAME_API UPlayerWalkState : public UPlayerBaseState
{
	GENERATED_BODY()
public:
	virtual EPlayerStateID GetStateID() const override { return EPlayerStateID::Walk; }

protected:
	virtual void OnEnter(UPlayerStateMachine* InSM) override;
	virtual void OnTick(UPlayerStateMachine* InSM, float DeltaTime) override;

private:
	float SmoothedMaxSpeed = 0.f;
	float AccelInterpSpeed = 3.0f;
};
