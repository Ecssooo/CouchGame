#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Systems/Save/SaveCubeStruct.h"
#include "Step.generated.h"

class AStepManager;

UCLASS(Abstract, Blueprintable)
class COUCHGAME_API AStep : public AActor
{
	GENERATED_BODY()

public :
	AStep();
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSubLevel> SubLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StepName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AStepManager> StepManager;

	UFUNCTION(BlueprintImplementableEvent, Category="Step", meta=(DisplayName="Start Step"))
	void OnStartStep();

	UFUNCTION(BlueprintCallable, Category="Step")
	void StartStep();

	UFUNCTION(BlueprintImplementableEvent, Category="Step", meta=(DisplayName="End Step"))
	void OnEndStep();

	UFUNCTION(BlueprintCallable, Category="Step")
	void EndStep();
};
