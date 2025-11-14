// Fill out your copyright notice in the Description page of Project Settings.


#include "Grab/GrabSocketActor.h"

#include "Grab/GrabActor.h"
#include "Interfaces/Grabbable.h"
#include "Interfaces/IPluginManager.h"


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

void AGrabSocketActor::AttachObjectToSocket(AGrabActor* GrabActor)
{
	if (!GrabActor) return;
	GrabActor->AttachToActor(this,FAttachmentTransformRules::SnapToTargetIncludingScale);
	GrabActor->IsInSocket = true;
	HasObjectInSocket = true;
}

void AGrabSocketActor::SaveState()
{
	
}

void AGrabSocketActor::SpawnObjectInSocket()
{
	if (!ObjectType) return;
	auto actor = GetWorld()->SpawnActor(ObjectType);
	if (!actor) return;
	ActorInSocket = Cast<AGrabActor>(actor);
	AttachObjectToSocket(ActorInSocket);
}

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
