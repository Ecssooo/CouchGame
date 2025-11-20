// Fill out your copyright notice in the Description page of Project Settings.


#include "Grab/GrabSocketActor.h"

#include "LevelStreamerActor.h"
#include "Grab/GrabActor.h"
#include "Grab/GrabActorSpawner.h"
#include "Grab/GrabSocketManager.h"
#include "Grab/GrabSocketSubsystem.h"
#include "Interfaces/Grabbable.h"
#include "Kismet/GameplayStatics.h"

#pragma region UE
// Sets default values
AGrabSocketActor::AGrabSocketActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGrabSocketActor::BeginPlay()
{
	Super::BeginPlay();
	HasObjectInSocket = false;
	BoxCollision = Cast<UBoxComponent>(GetRootComponent());
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AGrabSocketActor::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AGrabSocketActor::OnOverlapEnd);
}

// Called every frame
void AGrabSocketActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// UE_LOG(LogTemp, Warning, TEXT("ActorInOverlap : %d; HasObjectInSocket : %d"), ActorInOverlap==nullptr, HasObjectInSocket);
	if (ActorInOverlap && !HasObjectInSocket)
	{
		if (!ActorInOverlap->IsGrabbed)
		{
			AttachObjectToSocket(ActorInOverlap);
		}
	}
}
#pragma endregion

void AGrabSocketActor::AttachObjectToSocket(AGrabActor* GrabActor)
{
	if (!GrabActor) return;
	GrabActor->AttachToActor(this,FAttachmentTransformRules::SnapToTargetIncludingScale);
	GrabActor->IsInSocket = true;
	HasObjectInSocket = true;
	OnObjectPlaced.Broadcast(this);
	SaveState();
}

void AGrabSocketActor::SaveState()
{
	if (UGrabSocketSubsystem* SocketSubsystem = GetGameInstance()->GetSubsystem<UGrabSocketSubsystem>())
	{
		AGrabSocketManager* SocketManager = Cast<AGrabSocketManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrabSocketManager::StaticClass()));
		SocketSubsystem->SaveSocketData(SocketManager->LevelId, id);
	}	
}

#pragma region Spawn GrabActor
void AGrabSocketActor::SpawnObject(bool IsInSocket)
{
	if (IsInSocket)
	{
		SpawnObjectInSocket();
	}else
	{
		SpawnObjectInSpawners();
	}
}

void AGrabSocketActor::SpawnObjectInSocket()
{
	if (!ObjectType) return;

	ALevelStreamerActor* LSA = Cast<ALevelStreamerActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelStreamerActor::StaticClass()));

	ULevelStreaming* MyStreamedLevel =
		UGameplayStatics::GetStreamingLevel(GetWorld(), LSA->CurrentLevel);
	ULevel* LoadedLevel = MyStreamedLevel ? MyStreamedLevel->GetLoadedLevel() : nullptr;

	if (LoadedLevel)
	{
		FActorSpawnParameters Params;
		Params.OverrideLevel = LoadedLevel;

		AGrabActor* Actor = GetWorld()->SpawnActor<AGrabActor>(
			ObjectType.Get(),
			GetActorLocation(),
			GetActorRotation(),
			Params
		);
		if (!Actor) return;

		ActorInSocket = Actor;
		AttachObjectToSocket(ActorInSocket);
	}
}

void AGrabSocketActor::SpawnObjectInSpawners()
{
	if (!ObjectType) return;

	ALevelStreamerActor* LSA = Cast<ALevelStreamerActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelStreamerActor::StaticClass()));
	
	ULevelStreaming* MyStreamedLevel =
		UGameplayStatics::GetStreamingLevel(GetWorld(), LSA->CurrentLevel);
	ULevel* LoadedLevel = MyStreamedLevel ? MyStreamedLevel->GetLoadedLevel() : nullptr;

	if (LoadedLevel)
	{
		FActorSpawnParameters Params;
		Params.OverrideLevel = LoadedLevel;

		AGrabActor* Actor = GetWorld()->SpawnActor<AGrabActor>(
			ObjectType.Get(),
			GetActorLocation(),
			GetActorRotation(),
			Params
		);
		if (!Actor) return;

		ActorInSocket = Actor;
		Actor->SetActorLocation(ActorSpawner->GetActorLocation());	
	}
}
#pragma endregion

#pragma region Collision
void AGrabSocketActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UGrabbable>()) return;
	if (OtherActor->IsA(ObjectType))
	{
		UE_LOG(LogTemp, Warning, TEXT("GrabSocket Overlap"))
		ActorInOverlap = Cast<AGrabActor>(OtherActor);
	}
}

void AGrabSocketActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->Implements<UGrabbable>()) return;
	
	if (OtherActor->IsA(ObjectType)) ActorInOverlap = nullptr;
}
#pragma endregion