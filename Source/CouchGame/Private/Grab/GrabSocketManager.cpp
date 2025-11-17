// Fill out your copyright notice in the Description page of Project Settings.


#include "Grab/GrabSocketManager.h"

#include "Grab/GrabSocketActor.h"
#include "Grab/GrabSocketSubsystem.h"


// Sets default values
AGrabSocketManager::AGrabSocketManager(): LevelId(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGrabSocketManager::BeginPlay()
{
	Super::BeginPlay();
	if (UGrabSocketSubsystem* SocketSubsystem = GetGameInstance()->GetSubsystem<UGrabSocketSubsystem>())
	{
		SocketSubsystem->SocketManager = this;
		SocketSubsystem->InitLevelDatas();
	}
}

// Called every frame
void AGrabSocketManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TObjectPtr<AGrabSocketActor> AGrabSocketManager::FindSocketFromId(int socketId)
{
	for (AGrabSocketActor* socketActor : AllGrabSocketActors)
	{
		if (socketActor->id == socketId) return socketActor;
	}
	return nullptr;
}

void AGrabSocketManager::SaveSocketState()
{
	UGrabSocketSubsystem* SocketSubsystem = GetGameInstance()->GetSubsystem<UGrabSocketSubsystem>();
	if (!SocketSubsystem) return;

	for (AGrabSocketActor* socketActor : AllGrabSocketActors)
	{
		if (socketActor->HasObjectInSocket) SocketSubsystem->SaveSocketData(LevelId, socketActor->id);
	}
}

void AGrabSocketManager::LoadAllSocket()
{
	UGrabSocketSubsystem* subsystem = GetGameInstance()->GetSubsystem<UGrabSocketSubsystem>();
	if (!subsystem) return;
	subsystem->LoadLevelDataInSocket(1,this);
}