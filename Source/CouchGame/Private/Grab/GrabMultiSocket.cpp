// Fill out your copyright notice in the Description page of Project Settings.


#include "Grab/GrabMultiSocket.h"

#include "Grab/GrabSocketActor.h"

#pragma region UE
// Sets default values
AGrabMultiSocket::AGrabMultiSocket()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGrabMultiSocket::BeginPlay()
{
	Super::BeginPlay();
	InitEvent();
}

// Called every frame
void AGrabMultiSocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
#pragma endregion

void AGrabMultiSocket::InitEvent()
{
	for (AGrabSocketActor* socket : GrabSockets)
	{
		socket->OnObjectPlaced.AddDynamic(this, &AGrabMultiSocket::GrabActorPlacedInSocket);
	}
}

void AGrabMultiSocket::GrabActorPlacedInSocket(AGrabSocketActor* socket)
{
	for (AGrabSocketActor* socket  : GrabSockets)
	{
		if (!socket->HasObjectInSocket) return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("All Socket quest are completed"))
}