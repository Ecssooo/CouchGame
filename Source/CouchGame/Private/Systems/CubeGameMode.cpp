// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CubeGameMode.h"

#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CharacterPlayer.h"
#include "Systems/CharacterSettings.h"
#include "Systems/LevelComunicationManager.h"
#include "Systems/LevelComunicationSubsystem.h"
#include "Systems/PlayerSpawners/MainCharacterSpawner.h"
#include "Systems/PlayerSpawners/MainTeleporterOut.h"
#include "Systems/PlayerSpawners/StreamedCharacterSpawner.h"

void ACubeGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	FindMainCharacterSpawners(MainCharacterSpawners);
	FindMainTeleporterOut(MainTeleportersOut);
	SpawnCharacterAtBeginPlay();
	

}


#pragma region Manage players
void ACubeGameMode::SpawnCharacterAtBeginPlay()
{
	const UCharacterSettings* Settings = GetDefault<UCharacterSettings>();
	for (AMainCharacterSpawner* CharacterSpawner : MainCharacterSpawners)
	{
		if (CharacterSpawner == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerSpawner null"))
			continue;
		};
		
		APlayerController* LocalPlayer = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), CharacterSpawner->PlayerIndex);
		if (!LocalPlayer)
		{
			LocalPlayer = UGameplayStatics::CreatePlayer(GetWorld(), CharacterSpawner->PlayerIndex, true);
			UE_LOG(LogTemp, Warning, TEXT("PlayerController create for index : %d"), CharacterSpawner->PlayerIndex);
		};
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		ACharacterPlayer* NewCharacter = GetWorld()->SpawnActor<ACharacterPlayer>(
			GetCharacterClass(CharacterSpawner->PlayerIndex),
			CharacterSpawner->GetActorLocation(),
			CharacterSpawner->GetActorRotation(),
			SpawnParams
		);
		
		if (!NewCharacter) continue;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Character Created"));
		NewCharacter->PlayerIndex = CharacterSpawner->PlayerIndex;
		LocalPlayer->Possess(NewCharacter);
		Players.Add(NewCharacter);


		if (!LocalPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("Local player not found : %d"), CharacterSpawner->PlayerIndex);
		};
		
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer->GetLocalPlayer());
		if (!InputSubsystem)
		{
			UE_LOG(LogTemp, Warning, TEXT("Input subsystem not found"))
			
			continue;	
		}
		if (Settings->UsKeyboardControl)
		{
			InputSubsystem->AddMappingContext(GetKeyboardProfile(CharacterSpawner->PlayerIndex),0);
			UE_LOG(LogTemp, Warning, TEXT("LocalPlayer : %d, Add keyboard IMC : %d"), LocalPlayer->GetLocalPlayer()->GetControllerId(), CharacterSpawner->PlayerIndex);
		}
		else
		{	
			InputSubsystem->AddMappingContext(Settings->IMCInGame.IMCGamePad,0);
			UE_LOG(LogTemp, Warning, TEXT("Add GamePad IMC : %d"), CharacterSpawner->PlayerIndex);
		}
	}
}

void ACubeGameMode::TeleportCharacterOut()
{
	for (AMainTeleporterOut* Teleporter : MainTeleportersOut)
	{
		if(!Teleporter)
		{
			UE_LOG(LogTemp, Warning, TEXT("TeleportersOut is null"));
			continue;
		}
		ACharacterPlayer* CharacterToTP = Players[Teleporter->PlayerIndex];
		CharacterToTP->SetActorLocation(Teleporter->GetActorLocation());

		UE_LOG(LogTemp, Warning, TEXT("Character (%d) teleport to Teleporter : %d"), Teleporter->PlayerIndex,Teleporter->PlayerIndex);
	}
}

void ACubeGameMode::SpawnCharacterInStreamedLevel(ELevelDir dir)
{
	FindStreamedCharacterSpawners(StreamedCharacterSpawners);
	
	for (AStreamedCharacterSpawner* CharacterSpawner : StreamedCharacterSpawners)
	{
		if (!CharacterSpawner)
		{
			UE_LOG(LogTemp, Warning, TEXT("StreamedCharacterSpawners is null"));
		}
		if (CharacterSpawner->SpawnerDirection == dir)
		{
			ACharacterPlayer* CharacterToSpawn = Players[CharacterSpawner->PlayerIndex];
			CharacterToSpawn->SetActorLocation(CharacterSpawner->GetActorLocation());
			// CharacterToSpawn->StateMachine->ChangeState(EPlayerStateID::Idle);
			UE_LOG(LogTemp, Warning, TEXT("Character (%d) teleport to CharacterSpawner : %d"), CharacterSpawner->PlayerIndex, CharacterSpawner->PlayerIndex);
		}
	}	
	ALevelComunicationManager* ComManager = Cast<ALevelComunicationManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelComunicationManager::StaticClass()));
	if (!ComManager) return;

	ComManager->GetPartitionLevelsInWorld();

	ULevelComunicationSubsystem* ComSubsystem = GetGameInstance()->GetSubsystem<ULevelComunicationSubsystem>();
	if (!ComSubsystem) return;
	//ComSubsystem->DiscoveredSubLevel(1,0);
	
}

ACharacterPlayer* ACubeGameMode::GetPlayerFromID(int PlayerID)
{
	for (ACharacterPlayer* player : Players)
	{
		if (player->PlayerIndex == PlayerID) return player;
	}
	return nullptr;
}
#pragma endregion

#pragma region Find Spawners
void ACubeGameMode::FindMainCharacterSpawners(TArray<AMainCharacterSpawner*>& InMainCharacterSpawners)
{
	InMainCharacterSpawners.Empty();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainCharacterSpawner::StaticClass(), FoundActors);

	for (AActor* actor : FoundActors)
	{
		if (actor)
		{
			// UE_LOG(LogTemp, Warning, TEXT("Actor find : %s"), *actor->GetName());
			InMainCharacterSpawners.Add(Cast<AMainCharacterSpawner>(actor));
		}
	}
	if (InMainCharacterSpawners[0]->PlayerIndex == 1)
	{
		AMainCharacterSpawner* temp = InMainCharacterSpawners[0];
		InMainCharacterSpawners[0] = InMainCharacterSpawners[1];  
		InMainCharacterSpawners[1] = temp; 
	}
}

void ACubeGameMode::FindMainTeleporterOut(TArray<AMainTeleporterOut*>& InMainTeleportersOut)
{
	InMainTeleportersOut.Empty();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainTeleporterOut::StaticClass(), FoundActors);

	for (AActor* actor : FoundActors)
	{
		InMainTeleportersOut.Add(Cast<AMainTeleporterOut>(actor));
	}
}

void ACubeGameMode::FindStreamedCharacterSpawners(TArray<AStreamedCharacterSpawner*>& InStreamedCharacterSpawners)
{
	InStreamedCharacterSpawners.Empty();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStreamedCharacterSpawner::StaticClass(), FoundActors);

	for (AActor* actor : FoundActors)
	{
		InStreamedCharacterSpawners.Add(Cast<AStreamedCharacterSpawner>(actor));
	}
}
#pragma endregion





TSubclassOf<ACharacterPlayer> ACubeGameMode::GetCharacterClass(int PlayerIndex)
{
	const UCharacterSettings* Settings = GetDefault<UCharacterSettings>();
		
	switch (PlayerIndex)
	{
	case 0:
		return Settings->CharacterPlayer1;
	case 1:
		return Settings->CharacterPlayer2;
	default:
		return nullptr;
	}
}

UInputMappingContext* ACubeGameMode::GetKeyboardProfile(int PlayerIndex)
{
	const UCharacterSettings* Settings = GetDefault<UCharacterSettings>();
		
	switch (PlayerIndex)
	{
	case 0:
		return Settings->IMCInGame.IMCKeyboardP1;
	case 1:
		return Settings->IMCInGame.IMCKeyboardP2;
	default:
		return nullptr;
	}
}