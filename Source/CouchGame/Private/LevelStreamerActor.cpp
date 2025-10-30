// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamerActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"


// Sets default values
ALevelStreamerActor::ALevelStreamerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentLevelIndex = -1; // pour commencer � -1
}

void ALevelStreamerActor::BeginPlay()
{
	Super::BeginPlay();

	CurrentLevelName = StartingLevel;
	SwitchToSpecificLevel(StartingLevel);
}

// Called every frame
void ALevelStreamerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelStreamerActor::UnloadActualLevel()
{
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnLevelUnloaded"); // callback
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__;

	UGameplayStatics::UnloadStreamLevel(this, CurrentLevel, LatentInfo, false);
}

void ALevelStreamerActor::SwitchToSpecificLevel(FName NewLevelName)
{
	NextLevel = NewLevelName;
	LoadLevel(NewLevelName); //si il est pas sur un level je charge directement pas besoin de d�charger
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Load level"));
}

//quand le level est d�charg�
void ALevelStreamerActor::OnLevelUnloaded()
{
	UE_LOG(LogTemp, Warning, TEXT("Ancien niveau d�charg�, on charge %s"), *NextLevel.ToString());
}

// mettre la bonne rotation de al face chargé
void ALevelStreamerActor::RotateLevel()
{
	FLevelNeighbors* Neigh = Adjacency.Find(CurrentLevelName);
	if (!Neigh)
	{
		UE_LOG(LogTemp, Warning, TEXT("RotateLevel: no adjacency data for %s"), *CurrentLevelName.ToString());
		return;
	}

	if (!Neigh->Arrow)
	{
		UE_LOG(LogTemp, Warning, TEXT("RotateLevel: no Arrow set for %s"), *CurrentLevelName.ToString());
		return;
	}

	const FVector ArrowForwardWS = Neigh->Arrow->GetActorRotation().Vector();
	FRotator ArrowRot = Neigh->Arrow->GetActorRotation();

	ArrowRot.Pitch = 0.f;
	ArrowRot.Roll  = 0.f;

	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("ParentTag"), Found);

	for (AActor* A : Found)
	{
		if (!A) continue;

		FRotator FaceRot = A->GetActorRotation();
		FaceRot.Yaw = ArrowRot.Yaw;
		FaceRot.Pitch = 0.f;
		FaceRot.Roll = 0.f;

		A->SetActorRotation(FaceRot);
		break;
	}
}


//charger le level
void ALevelStreamerActor::LoadLevel(FName NewLevelName)
{
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnLevelLoaded"); // callback
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__;

	UGameplayStatics::LoadStreamLevel(this, NewLevelName, true, false, LatentInfo);
	CurrentLevel = NewLevelName;
}

//quand le level est charg�
void ALevelStreamerActor::OnLevelLoaded()
{
	UE_LOG(LogTemp, Warning, TEXT("Niveau %s charg� avec succ�s."), *CurrentLevel.ToString());
	RotateLevel();
}

FName ALevelStreamerActor::GetNeighborLevel(FName FromLevel, ELevelDir Dir) const
{
	if (const FLevelNeighbors* Neigh = Adjacency.Find(FromLevel))
	{
		CurrentLevelName = Neigh->Get(Dir);
		return Neigh->Get(Dir);
	}
	return NAME_None;
}
