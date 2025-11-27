// Fill out your copyright notice in the Description page of Project Settings.


#include "Grab/GrabActor.h"

#include "Components/BoxComponent.h"
#include "Grab/GrabActorSocket.h"
#include "Interfaces/GrabSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CharacterPlayer.h"
#include "Systems/Save/SaveCubeSubsystem.h"
#include "Systems/Save/SaveObjectManager.h"


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
	BoxComponent = FindComponentByClass<UBoxComponent>();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AGrabActor::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AGrabActor::OnBoxEndOverlap);
}

// Called every frame
void AGrabActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrabActor::Grab(ACharacterPlayer* Player)
{
	USaveCubeSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveCubeSubsystem>();
	if (!SaveSubsystem) return;

	if (ObjectData.ObjectState == InPlayer || ObjectData.ObjectState == InSocket) return;
	
	SaveSubsystem->SetObjectState(ObjectData.ObjectID, EObjectState::InPlayer, Player->PlayerIndex);
	ObjectData.ObjectState = EObjectState::InPlayer;

	UE_LOG(LogTemp, Log, TEXT("Grab CPP"))

	IGrabbable::Execute_OnGrab(this, Player);
}

void AGrabActor::Drop(ACharacterPlayer* Player)
{
	USaveCubeSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveCubeSubsystem>();
	if (!SaveSubsystem) return;
	
	if (GrabSocketInOverlap)
	{
		SaveSubsystem->SetObjectState(ObjectData.ObjectID, EObjectState::InSocket, GrabSocketInOverlap->SocketID);
		ObjectData.ObjectState = EObjectState::InSocket;
		AttachToActor(GrabSocketInOverlap, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}else
	{
		ASaveObjectManager* ObjectManager = Cast<ASaveObjectManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASaveObjectManager::StaticClass()));
		if (!ObjectManager) return;
		SaveSubsystem->SetObjectState(ObjectData.ObjectID, EObjectState::InWorld, ObjectManager->LevelID, GetActorLocation());
		ObjectData.ObjectState = EObjectState::InWorld;
	}

	IGrabbable::Execute_OnDrop(this, Player);
}

FGrabObject AGrabActor::GetData_Implementation()
{
	return ObjectData;
}

void AGrabActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UGrabSocket>())
	{
		AGrabActorSocket* socket = Cast<AGrabActorSocket>(OtherActor);
		if (socket->SocketID == ObjectData.SocketID)
		{
			GrabSocketInOverlap = socket;
		}
	}
}

void AGrabActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UGrabSocket>())
	{
		AGrabActorSocket* socket = Cast<AGrabActorSocket>(OtherActor);
		if (socket->SocketID == ObjectData.SocketID)
		{
			GrabSocketInOverlap = nullptr;
		}
	}
}
