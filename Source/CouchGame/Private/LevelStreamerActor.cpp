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
	const FRotator FaceRot = GetCurrentFaceRotation();

	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("ParentTag"), Found);

	for (AActor* A : Found)
	{
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
		return Neigh->Get(Dir);
	}
	return NAME_None;
}
