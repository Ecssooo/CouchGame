#include "TPManager.h"
#include "Kismet/GameplayStatics.h"
#include "LevelStreamerActor.h"

UTPManager::UTPManager()
{
	PrimaryComponentTick.bCanEverTick = false; // pas de Tick pour l’instant
}

void UTPManager::BeginPlay()
{
	Super::BeginPlay();

	// Optionnel : auto-récup du LevelStreamer si pas assigné dans l’éditeur
	if (!LevelStreamer)
	{
		TArray<AActor*> Found;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelStreamerActor::StaticClass(), Found);
		if (Found.Num() > 0)
		{
			LevelStreamer = Cast<ALevelStreamerActor>(Found[0]);
		}
	}
}

void UTPManager::UnloadLevel()
{
	LevelStreamer->UnloadActualLevel();
}


void UTPManager::TrySwitch(ELevelDir Dir)
{
	if (!LevelStreamer)
	{
		UE_LOG(LogTemp, Warning, TEXT("TPManager: LevelStreamer non assigné"));
		return;
	}

	const FName From = LevelStreamer->CurrentLevel;
	const FName Target = LevelStreamer->GetNeighborLevel(From, Dir);

	if (Target.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("TPManager: pas de voisin %d pour le level %s"), (int32)Dir, *From.ToString());
		return;
	}

	LevelStreamer->CubeBasis.Turn(Dir);
	LevelStreamer->SwitchToSpecificLevel(Target);
}

void UTPManager::OnUpPressed() { TrySwitch(ELevelDir::Up); }
void UTPManager::OnDownPressed() { TrySwitch(ELevelDir::Down); }
void UTPManager::OnLeftPressed() { TrySwitch(ELevelDir::Left); }
void UTPManager::OnRightPressed() { TrySwitch(ELevelDir::Right); }
