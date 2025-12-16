#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PartitionLevel.generated.h"

UCLASS(Blueprintable)
class ONLIFESPATH_API APartitionLevel : public AActor
{
	GENERATED_BODY()

public:

	APartitionLevel();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DiscoverSubLevel();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UndiscoverSubLevel();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int IdSublevel;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;
};
