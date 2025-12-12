// Fill out your copyright notice in the Description page of Project Settings.


#include "Grab/GrabActorSocket.h"

#include "LevelStreamerActor.h"
#include "Grab/GrabActor.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/Save/SaveObjectManager.h"


// Sets default values
AGrabActorSocket::AGrabActorSocket()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGrabActorSocket::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrabActorSocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrabActorSocket::SpawnObjectInSocket(UClass* ActorToSpawn)
{
	UStaticMeshComponent* socket = GetSocketParent();
	if (!socket) return;

	ALevelStreamerActor* LSA = Cast<ALevelStreamerActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelStreamerActor::StaticClass()));
	if (!LSA) return;
	ULevelStreaming* MyStreamedLevel =
		UGameplayStatics::GetStreamingLevel(GetWorld(), LSA->CurrentLevel);
	ULevel* LoadedLevel = MyStreamedLevel ? MyStreamedLevel->GetLoadedLevel() : nullptr;
	ASaveObjectManager* ObjectManager = Cast<ASaveObjectManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASaveObjectManager::StaticClass())); 
	if (LoadedLevel)
	{
		FActorSpawnParameters Params;
		Params.OverrideLevel = LoadedLevel;

		AGrabActor* actor = GetWorld()->SpawnActor<AGrabActor>(ActorToSpawn,
								socket->GetComponentLocation(),
								socket->GetComponentRotation(),
								Params);
		actor->AttachToComponent(socket, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		ObjectManager->GrabActors.Add(actor);
	}
}

