// Fill out your copyright notice in the Description page of Project Settings.


#include "Grab/GrabSwitchFaceSubsystem.h"

#include "LevelStreamerActor.h"
#include "Grab/GrabActor.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CharacterPlayer.h"
#include "Player/PlayerStateMachine.h"


/*void UGrabSwitchFaceSubsystem::InitPlayer(ACharacterPlayer* InPlayer, int PlayerIndex)
{
	switch (PlayerIndex)
	{
		case(0):
			Player1 = InPlayer;
			break;
		case(1):
			Player2 = InPlayer;
			break;
		default:
			break;
	}
}



void UGrabSwitchFaceSubsystem::SaveGrabObject(ACharacterPlayer* player, TSubclassOf<AGrabActor> grabActorClass)
{
	if (player == Player1) GrabActorInPlayer1 = grabActorClass;
	else if (player == Player2) GrabActorInPlayer2 = grabActorClass;
}

void UGrabSwitchFaceSubsystem::LoadGrabObject(int playerIndex, ACharacterPlayer* Player)
{
	ALevelStreamerActor* LSA = Cast<ALevelStreamerActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelStreamerActor::StaticClass()));
	
	ULevelStreaming* MyStreamedLevel =
	UGameplayStatics::GetStreamingLevel(GetWorld(), LSA->CurrentLevel);
	ULevel* LoadedLevel = MyStreamedLevel ? MyStreamedLevel->GetLoadedLevel() : nullptr;

	if (LoadedLevel)
	{
		FActorSpawnParameters Params;
		Params.OverrideLevel = LoadedLevel;

		Player->GrabbableActor = GetWorld()->SpawnActor<AGrabActor>(
			GetGrabActorSubclass(playerIndex).Get(),
			Player->GrabParent->GetComponentLocation(),
			Player->GrabParent->GetComponentRotation(),
			Params
		);
	}
	Player->StateMachine->ChangeState(EPlayerStateID::Grab);
}

TSubclassOf<AGrabActor> UGrabSwitchFaceSubsystem::GetGrabActorSubclass(int playerIndex)
{
	switch (playerIndex)
	{
		case(0):
			return GrabActorInPlayer1;
		case(1):
			return GrabActorInPlayer2;
		default:
			return nullptr;
	}
}

void UGrabSwitchFaceSubsystem::ClearSubclass(ACharacterPlayer* player)
{
	if (player == Player1) GrabActorInPlayer1 = nullptr;
	else if (player == Player2) GrabActorInPlayer2 = nullptr;
}*/
