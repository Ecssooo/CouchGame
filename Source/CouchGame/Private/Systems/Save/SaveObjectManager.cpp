// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Save/SaveObjectManager.h"

#include "LevelStreamerActor.h"
#include "Grab/GrabActor.h"
#include "Grab/GrabActorSocket.h"
#include "Grab/GrabActorSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CharacterPlayer.h"
#include "Systems/CubeGameMode.h"
#include "Systems/Save/SaveCubeSubsystem.h"


// Sets default values
ASaveObjectManager::ASaveObjectManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASaveObjectManager::BeginPlay()
{
	Super::BeginPlay();
	LoadObjectData();
}

// Called every frame
void ASaveObjectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASaveObjectManager::LoadObjectData()
{
	USaveCubeSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveCubeSubsystem>();
	if (!SaveSubsystem) return;

	ALevelStreamerActor* LSA = Cast<ALevelStreamerActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelStreamerActor::StaticClass()));
	if (!LSA) return;
	ULevelStreaming* MyStreamedLevel =
		UGameplayStatics::GetStreamingLevel(GetWorld(), LSA->CurrentLevel);
	ULevel* LoadedLevel = MyStreamedLevel ? MyStreamedLevel->GetLoadedLevel() : nullptr;

	if (LoadedLevel)
	{
		FActorSpawnParameters Params;
		Params.OverrideLevel = LoadedLevel;
		if(SaveSubsystem->AllObjectsDatas.IsEmpty()) return;
		for (FGrabObject ObjectData : SaveSubsystem->AllObjectsDatas)
		{
			switch (ObjectData.ObjectState)
			{
				case(EObjectState::InSocket):
					{
						AGrabActorSocket* socket = GetActorSocketFromID(ObjectData.SocketID);
						if (socket)
						{
							AGrabActor* actor = GetWorld()->SpawnActor<AGrabActor>(ObjectData.ObjectType.Get(),
								socket->GetActorLocation(),
								socket->GetActorRotation(),
								Params);
							if (actor)
							{
								actor->AttachToActor(socket, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
								actor->ObjectData = ObjectData;
								GrabActors.Add(actor);
								socket->EnableSocketVisibility(false);
							}
						}
						break;
					}
				case(EObjectState::InSpawner):
					{
						AGrabActorSpawner* spawner = GetActorSpawnerFromID(ObjectData.SpawnerID);
						if (spawner)
						{
							AGrabActor* actor = GetWorld()->SpawnActor<AGrabActor>(ObjectData.ObjectType.Get(),
								spawner->GetActorLocation(),
								spawner->GetActorRotation(),
								Params);
							if (actor)
							{
								actor->AttachToActor(spawner, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
								actor->ObjectData = ObjectData;
								GrabActors.Add(actor);
							}
						}
						break;
					}
				case(EObjectState::InPlayer):
					{
						if (ObjectData.PlayerID == -1) break;
 						ACubeGameMode* CGM = Cast<ACubeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
						if (CGM)
						{
							ACharacterPlayer* Player = CGM->GetPlayerFromID(ObjectData.PlayerID);
							AGrabActor* actor = GetWorld()->SpawnActor<AGrabActor>(ObjectData.ObjectType.Get(),
								Player->GetGrabParent()->GetComponentLocation(),
								Player->GetGrabParent()->GetComponentRotation(),
								Params);
							if (actor)
							{
								actor->ObjectData = ObjectData;
								Player->GrabObject(actor);
								GrabActors.Add(actor);
							}
						}
						break;
					}
				case(EObjectState::InWorld):
					{
						if (ObjectData.FaceID == LevelID)
						{
							AGrabActor* actor = GetWorld()->SpawnActor<AGrabActor>(ObjectData.ObjectType.Get(),
								ObjectData.Position,
								FRotator::ZeroRotator,
								Params);
							if (actor)
							{
								actor->ObjectData = ObjectData;
								GrabActors.Add(actor);
							}
						}
						break;
					}
				default:
					break;
			}
		}
	}
	UpdateAllObjectHighlight();	
}

void ASaveObjectManager::UpdateAllObjectHighlight()
{
	USaveCubeSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveCubeSubsystem>();
	if (!SaveSubsystem) return;
	
	for (AGrabActor* object : GrabActors)
	{
		if (!object) return;
		object->HighlightObject(SaveSubsystem->GetGrabObjectFromID(object->ObjectData.ObjectID).IsHighlight);
		if (AGrabActorSocket* socket = GetActorSocketFromID(object->ObjectData.SocketID)) socket->HighlightSocket(SaveSubsystem->GetGrabObjectFromID(object->ObjectData.ObjectID).IsSocketHighlight); 
	}
}

AGrabActorSocket* ASaveObjectManager::GetActorSocketFromID(int socketID)
{
	for (AGrabActorSocket* socket : SocketActors)
	{
		if (socket->SocketID == socketID) return socket;
	}
	return nullptr;
}

AGrabActorSpawner* ASaveObjectManager::GetActorSpawnerFromID(int socketID)
{
	for (AGrabActorSpawner* spawner : SpawnersActors)
	{
		if (spawner->SpawnerID == socketID) return spawner;
	}
	return nullptr;
}

