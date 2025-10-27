// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CubeGameMode.h"

#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CharacterPlayer.h"
#include "Systems/CharacterSettings.h"
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
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White,
 TEXT("Character Created"));
		//NewCharacter->AutoPossessPlayer = CharacterSpawner->Input;
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
			return;	
		}
		
		InputSubsystem->AddMappingContext(Settings->GamepadProfileData.IMCInGame,0);
	}
}

void ACubeGameMode::TeleportCharacterOut()
{
	for (AMainTeleporterOut* Teleporter : MainTeleportersOut)
	{
		ACharacterPlayer* CharacterToTP = Players[Teleporter->PlayerIndex];
		CharacterToTP->SetActorLocation(Teleporter->GetActorLocation());
	}
}

void ACubeGameMode::SpawnCharacterInStreamedLevel()
{
	FindStreamedCharacterSpawners(StreamedCharacterSpawners);

	for (AStreamedCharacterSpawner* CharacterSpawner : StreamedCharacterSpawners)
	{
		ACharacterPlayer* CharacterToSpawn = Players[CharacterSpawner->PlayerIndex];
		CharacterToSpawn->SetActorLocation(CharacterSpawner->GetActorLocation());
	}
}
#pragma endregion

#pragma region Find Spawners
void ACubeGameMode::FindMainCharacterSpawners(TArray<AMainCharacterSpawner*>& InMainCharacterSpawners)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainCharacterSpawner::StaticClass(), FoundActors);

	for (AActor* actor : FoundActors)
	{
		if (actor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor find : %s"), *actor->GetName());
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
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainTeleporterOut::StaticClass(), FoundActors);

	for (AActor* actor : FoundActors)
	{
		InMainTeleportersOut.Add(Cast<AMainTeleporterOut>(AMainTeleporterOut::StaticClass()));
	}
}

void ACubeGameMode::FindStreamedCharacterSpawners(TArray<AStreamedCharacterSpawner*>& InStreamedCharacterSpawners)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStreamedCharacterSpawner::StaticClass(), FoundActors);

	for (AActor* actor : FoundActors)
	{
		InStreamedCharacterSpawners.Add(Cast<AStreamedCharacterSpawner>(AStreamedCharacterSpawner::StaticClass()));
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