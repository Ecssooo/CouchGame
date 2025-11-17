// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamerActor.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "Systems/ArrowHelper.h"
#include "Systems/CubeGameMode.h"
#include "Systems/CubeController.h"
#include "Components/ArrowComponent.h"
#include "Grab/GrabSocketSubsystem.h"
#include "Containers/UnrealString.h"


class UGrabSocketSubsystem;
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
	SwitchToSpecificLevel(StartingLevel, ELevelDir::None);
	GameMode = Cast<ACubeGameMode>(GetWorld()->GetAuthGameMode());
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

void ALevelStreamerActor::SwitchToSpecificLevel(FName NewLevelName, ELevelDir Dir)
{
	NextLevel = NewLevelName;
	TmpLevelDir = Dir;
	LoadLevel(NewLevelName);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Load level"));
}

//quand le level est d�charg�
void ALevelStreamerActor::OnLevelUnloaded()
{
	UE_LOG(LogTemp, Warning, TEXT("Ancien niveau decharge, on charge %s"), *NextLevel.ToString());
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


	AArrowHelper* ArrowHelper = CubeController->GetArrow(Neigh->ArrowIndex);
	FRotator ArrowRot = ArrowHelper->Arrow->GetComponentRotation();
	const float DesiredYaw = FRotator::NormalizeAxis(ArrowRot.Yaw);


	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("ParentTag"), Found);

	for (AActor* A : Found)
	{
		if (!A) continue;

		FRotator FaceRot = FRotator::ZeroRotator;
		FaceRot.Yaw = DesiredYaw;
		FaceRot.Pitch = 0.f;
		FaceRot.Roll = 0.f;

		A->SetActorRotation(FaceRot);

		UE_LOG(LogTemp, Log, TEXT("RotateLevel: set %s yaw=%.1f"), *A->GetName(), FaceRot.Yaw);
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

//quand le level est charge
void ALevelStreamerActor::OnLevelLoaded()
{
	UE_LOG(LogTemp, Warning, TEXT("Niveau %s charge avec succes."), *CurrentLevel.ToString());
	RotateLevel();
	if (TmpLevelDir != ELevelDir::None)
		GameMode->SpawnCharacterInStreamedLevel(TmpLevelDir);
	if (UGrabSocketSubsystem* SocketSubsystem = GetGameInstance()->GetSubsystem<UGrabSocketSubsystem>())
	{
		AGrabSocketManager* SocketManager = Cast<AGrabSocketManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrabSocketManager::StaticClass()));
		SocketSubsystem->AddLevelData(SocketManager->LevelId, SocketManager);
	}
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

int ALevelStreamerActor::GetArrowIndex()
{
	FLevelNeighbors* Neigh = Adjacency.Find(CurrentLevelName);
	if (!Neigh)
	{
		UE_LOG(LogTemp, Warning, TEXT("RotateLevel: no adjacency data for %s"), *CurrentLevelName.ToString());
		return -1;
	}
	return Neigh->ArrowIndex;
}
