// Fill out your copyright notice in the Description page of Project Settings.


#include "Grab/GrabActor.h"

#include "Components/BoxComponent.h"
#include "Player/CharacterPlayer.h"
#include "Systems/Save/SaveCubeSubsystem.h"


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

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, OnBoxEndOverlap);
}

// Called every frame
void AGrabActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrabActor::OnGrab_Implementation(ACharacterPlayer* Player)
{
	USaveCubeSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveCubeSubsystem>();
	if (!SaveSubsystem) return;
	
	SaveSubsystem->SetObjectState(ObjectData.ObjectID, EObjectState::InPlayer, Player->PlayerIndex);
	ObjectData.ObjectState = EObjectState::InPlayer;

	IGrabbable::OnGrab_Implementation(Player);
}

void AGrabActor::OnDrop_Implementation(ACharacterPlayer* Player)
{
	IGrabbable::OnDrop_Implementation(Player);
}

FGrabObject AGrabActor::GetData_Implementation()
{
	return ObjectData;
}


