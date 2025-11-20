// Fill out your copyright notice in the Description page of Project Settings.


#include "Grab/GrabSwitchFaceSubsystem.h"

#include "LevelStreamerActor.h"
#include "Grab/GrabActor.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CharacterPlayer.h"
#include "Player/PlayerStateMachine.h"

void UGrabSwitchFaceSubsystem::SaveGrabObject(int playerIndex, TSubclassOf<AGrabActor> grabActorClass)
{
	switch (playerIndex)
	{
		case(0):
			GrabActorInPlayer1 = grabActorClass;
			break;
		case(1):
			GrabActorInPlayer2 = grabActorClass;
			break;
		default:
			break;
	}
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

void UGrabSwitchFaceSubsystem::ClearSubclass(int playerIndex)
{
	switch (playerIndex)
	{
		case(0):
			GrabActorInPlayer1 = nullptr;
			break;
		case(1):
			GrabActorInPlayer2 = nullptr;
			break;
		default:
			break;
	}
}
