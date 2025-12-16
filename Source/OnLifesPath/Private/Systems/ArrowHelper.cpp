// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/ArrowHelper.h"
#include "Components/ArrowComponent.h"


// Sets default values
AArrowHelper::AArrowHelper()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
}

// Called when the game starts or when spawned
void AArrowHelper::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AArrowHelper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
