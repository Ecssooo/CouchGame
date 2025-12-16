#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelStreamerActor.h"
#include "TPManager.generated.h"

class ALevelStreamerActor;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME_API UTPManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTPManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TPManager")
	ALevelStreamerActor* LevelStreamer = nullptr;

	UFUNCTION(BlueprintCallable, Category="TPManager|Level")
	void UnloadLevel();

	// /** Appelé quand on appuie sur la touche "Haut" */
	// UFUNCTION(BlueprintCallable, Category="TPManager|Input")
	// void OnUpPressed();
	//
	// /** Appelé quand on appuie sur la touche "Bas" */
	// UFUNCTION(BlueprintCallable, Category="TPManager|Input")
	// void OnDownPressed();
	//
	// /** Appelé quand on appuie sur la touche "Gauche" */
	// UFUNCTION(BlueprintCallable, Category="TPManager|Input")
	// void OnLeftPressed();
	//
	// /** Appelé quand on appuie sur la touche "Droite" */
	// UFUNCTION(BlueprintCallable, Category="TPManager|Input")
	// void OnRightPressed();

	UFUNCTION(BlueprintCallable)
	void TrySwitch(ELevelDir DirectionDir, ELevelDir EndDirection);
};
