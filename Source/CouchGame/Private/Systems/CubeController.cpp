// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CubeController.h"
#include "Systems/ArrowHelper.h"
#include "LevelStreamerActor.h"
#include "TPManager.h"
#include "Components/ArrowComponent.h"


// Sets default values
ACubeController::ACubeController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TPManager = CreateDefaultSubobject<UTPManager>(TEXT("TPManager"));
}

// Called when the game starts or when spawned
void ACubeController::BeginPlay()
{
	Super::BeginPlay();
	CollectArrowHelpers();

	CurrentRotationQuat = FQuat::Identity;
}

// Called every frame
void ACubeController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateStepAxis(DeltaTime);
}

void ACubeController::RotateStepAxis(float DeltaTime)
{
	if (!IsRotating) return;

	Elapsed += DeltaTime;
	float Alpha = Elapsed / AnimationDuration;
	Alpha = FMath::Clamp(Alpha, 0, 1);

	CurrentRotationQuat = FQuat::Slerp(StartQuat, TargetQuat, Alpha);
	SetActorRotation(CurrentRotationQuat);

	if (Elapsed > AnimationDuration)
	{
		UE_LOG(LogTemp, Log, TEXT("Animation termine"));
		IsRotating = false;
		CurrentRotationQuat = TargetQuat;
		SetActorRotation(CurrentRotationQuat);

		TPManager->TrySwitch(CurrentStartLevelDir, CurrentEndLevelDir);
	}
}

void ACubeController::StartRotationQuat(ELevelDir StartDir, ELevelDir EndDir)
{
	IsRotating = true;
	Elapsed = 0.f;

	TPManager->UnloadLevel();

	int ArrowIndex = LevelStreamerActor->GetArrowIndex();
	UArrowComponent* ArrowComponent = ArrowHelpers[ArrowIndex]->Arrow;
	ELevelDir ArrowDir = GetDirectionFromArrow(ArrowComponent);

	UE_LOG(LogTemp, Log, TEXT("StartDir = %d, ArrowDir = %d, NewStartDir = %d"), (uint8)StartDir, (uint8)ArrowDir,
	       (uint8)GetNewStartDir(StartDir, ArrowDir));

	CurrentStartTPLevelDir = GetNewStartDir(StartDir, ArrowDir);

	FQuat DeltaRotationQuat;
	switch (CurrentStartTPLevelDir)
	{
	case ELevelDir::Up:
		DeltaRotationQuat = FQuat::MakeFromEuler(FVector(0.0f, 90.0f, 0.0f));
		break;
	case ELevelDir::Down:
		DeltaRotationQuat = FQuat::MakeFromEuler(FVector(0.0f, -90.0f, 0.0f));
		break;
	case ELevelDir::Left:
		DeltaRotationQuat = FQuat::MakeFromEuler(FVector(90.0f, 0.0f, 0.0f));
		break;
	case ELevelDir::Right:
		DeltaRotationQuat = FQuat::MakeFromEuler(FVector(-90.0f, 0.0f, 0.0f));
		break;
	default:
		DeltaRotationQuat = FQuat::MakeFromEuler(FVector(0.0f, 90.0f, 0.0f));
		break;
	}

	CurrentStartLevelDir = StartDir;
	CurrentEndLevelDir = EndDir;

	StartQuat = CurrentRotationQuat;
	TargetQuat = DeltaRotationQuat * CurrentRotationQuat;
}

void ACubeController::CollectArrowHelpers()
{
	ArrowHelpers.Reset();

	TArray<UChildActorComponent*> ChildComps;
	GetComponents<UChildActorComponent>(ChildComps);

	// recuperer les ArrowHelper enfant du CubeController
	for (UChildActorComponent* CAC : ChildComps)
	{
		if (!CAC) continue;
		if (AActor* Child = CAC->GetChildActor())
		{
			if (AArrowHelper* Helper = Cast<AArrowHelper>(Child))
			{
				ArrowHelpers.Add(Helper);
			}
		}
	}

	// trier par FaceIndex
	ArrowHelpers.Sort([](const TObjectPtr<AArrowHelper>& L, const TObjectPtr<AArrowHelper>& R)
	{
		const int32 Li = (L ? L->FaceIndex : TNumericLimits<int32>::Max());
		const int32 Ri = (R ? R->FaceIndex : TNumericLimits<int32>::Max());
		return Li < Ri; // croissant
	});

	UE_LOG(LogTemp, Log, TEXT("CubeController: %d ArrowHelpers collectés."), ArrowHelpers.Num());
}

AArrowHelper* ACubeController::GetArrow(int Index)
{
	if (Index < 0 || Index >= ArrowHelpers.Num())
	{
		return nullptr;
	}
	return ArrowHelpers[Index];
}

ELevelDir ACubeController::GetDirectionFromArrow(const UArrowComponent* Arrow)
{
	if (!Arrow) return ELevelDir::None;

	// recuperer le Yaw de la face
	const float Yaw = FRotator::NormalizeAxis(Arrow->GetComponentRotation().Yaw);

	const int32 Step = FMath::RoundToInt(Yaw / 90.f); // -> -2, -1, 0, 1, 2

	switch ((Step % 4 + 4) % 4) // normalise dans {0,1,2,3}
	{
	case 0: return ELevelDir::Up;
	case 1: return ELevelDir::Right;
	case 2: return ELevelDir::Down;
	case 3: return ELevelDir::Left;
	default:
		return ELevelDir::None;
	}
}

ELevelDir ACubeController::GetNewStartDir(ELevelDir StartDir, ELevelDir ArrowDir)
{
	switch (ArrowDir)
	{
	case ELevelDir::Up:
		switch (StartDir)
		{
		case ELevelDir::Up: return ELevelDir::Up;
		case ELevelDir::Down: return ELevelDir::Down;
		case ELevelDir::Left: return ELevelDir::Left;
		case ELevelDir::Right: return ELevelDir::Right;
		default: return StartDir;
		}
	case ELevelDir::Down:
		switch (StartDir)
		{
		case ELevelDir::Up: return ELevelDir::Down;
		case ELevelDir::Down: return ELevelDir::Up;
		case ELevelDir::Left: return ELevelDir::Right;
		case ELevelDir::Right: return ELevelDir::Left;
		default: return StartDir;
		}
	case ELevelDir::Left:
		switch (StartDir)
		{
		case ELevelDir::Up: return ELevelDir::Left;
		case ELevelDir::Down: return ELevelDir::Right;
		case ELevelDir::Left: return ELevelDir::Down;
		case ELevelDir::Right: return ELevelDir::Up;
		default: return StartDir;
		}
	case ELevelDir::Right:
		switch (StartDir)
		{
		case ELevelDir::Up: return ELevelDir::Right;
		case ELevelDir::Down: return ELevelDir::Left;
		case ELevelDir::Left: return ELevelDir::Up;
		case ELevelDir::Right: return ELevelDir::Down;
		default: return StartDir;
		}
	default: return StartDir;
	}
}
