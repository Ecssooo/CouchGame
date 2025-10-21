// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamerActor.h"

#include "MeshPaintVisualize.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"


// Sets default values
ALevelStreamerActor::ALevelStreamerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentLevelIndex = -1; // pour commencer � -1
}

// Called when the game starts or when spawned
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

void ALevelStreamerActor::SwitchToNextLevel()
{
	// S�curit� : si le tableau est vide, on ne fait rien.
	if (LevelSequence.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("tableau vide."));
		return;
	}

	// 1. On passe � l'index suivant. // vu que on commence � -1 pas l� 0
	CurrentLevelIndex++;

	// 2. Si on d�passe la fin du tableau, on revient au d�but. C'est la boucle !
	if (CurrentLevelIndex >= LevelSequence.Num())
	{
		CurrentLevelIndex = 0;
	}

	// 3. On charge le niveau correspondant � notre nouvel index.
	SwitchToSpecificLevel(LevelSequence[CurrentLevelIndex]);
}

void ALevelStreamerActor::UnloadActualLevel()
{
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	// LatentInfo.ExecutionFunction = FName("OnLevelUnloaded"); // callback
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__;

	UGameplayStatics::UnloadStreamLevel(this, CurrentLevel, LatentInfo, false);
}

void ALevelStreamerActor::SwitchToSpecificLevel(FName NewLevelName)
{
	NextLevel = NewLevelName;
	LoadLevel(NewLevelName); //si il est pas sur un level je charge directement pas besoin de d�charger
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Load level"));

	// if (NewLevelName == CurrentLevel || NewLevelName.IsNone())
	// 	return;
	//
	// if (!CurrentLevel.IsNone()) //v�rifie si le joueur est d�j� sur un level 
	// {
	// 	FLatentActionInfo LatentInfo;
	// 	LatentInfo.CallbackTarget = this;
	// 	LatentInfo.ExecutionFunction = FName("OnLevelUnloaded"); // callback
	// 	LatentInfo.Linkage = 0;
	// 	LatentInfo.UUID = __LINE__;
	//
	// 	// D�charge l'ancien niveau, et appelle OnLevelUnloaded() quand fini
	// 	UGameplayStatics::UnloadStreamLevel(this, CurrentLevel, LatentInfo, false);
	//
	// 	// Stocke temporairement le nom du prochain niveau � charger
	// 	NextLevel = NewLevelName;
	// }
	// else
	// {
	// 	LoadLevel(NewLevelName); //si il est pas sur un level je charge directement pas besoin de d�charger
	// 	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Load level"));
	// }
}

//quand le level est d�charg�
void ALevelStreamerActor::OnLevelUnloaded()
{
	UE_LOG(LogTemp, Warning, TEXT("Ancien niveau d�charg�, on charge %s"), *NextLevel.ToString());
	LoadLevel(NextLevel); //quand le level est d�charg� je charge le prochain level
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
}

FName ALevelStreamerActor::GetNeighborLevel(FName FromLevel, ELevelDir Dir) const
{
	if (const FLevelNeighbors* Neigh = Adjacency.Find(FromLevel))
	{
		return Neigh->Get(Dir);
	}
	return NAME_None;
}
