// Fill out your copyright notice in the Description page of Project Settings.


#include "Grab/GrabActor.h"


// Sets default values
AGrabActor::AGrabActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGrabActor::BeginPlay()
{
	Super::BeginPlay();
	IsInSocket = false;
}

// Called every frame
void AGrabActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrabActor::OnGrab_Implementation(ACharacterPlayer* Player)
{
	IGrabbable::OnGrab_Implementation(Player);
	IsGrabbed = true;
}

void AGrabActor::OnDrop_Implementation(ACharacterPlayer* Player)
{
	IGrabbable::OnDrop_Implementation(Player);
	IsGrabbed = false;
}

bool AGrabActor::GetIsInSocket_Implementation()
{
	return IsInSocket;
}

