// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamerActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"

// Sets default values
ALevelStreamerActor::ALevelStreamerActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    CurrentLevelIndex = -1; // pour commencer à -1
}

// Called when the game starts or when spawned
void ALevelStreamerActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ALevelStreamerActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ALevelStreamerActor::SwitchToNextLevel()
{
    // Sécurité : si le tableau est vide, on ne fait rien.
    if (LevelSequence.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("tableau vide."));
        return;
    }

    // 1. On passe à l'index suivant. // vu que on commence à -1 pas là 0
    CurrentLevelIndex++;

    // 2. Si on dépasse la fin du tableau, on revient au début. C'est la boucle !
    if (CurrentLevelIndex >= LevelSequence.Num())
    {
        CurrentLevelIndex = 0;
    }

    // 3. On charge le niveau correspondant à notre nouvel index.
    SwitchToSpecificLevel(LevelSequence[CurrentLevelIndex]);
}

void ALevelStreamerActor::SwitchToSpecificLevel(FName NewLevelName)
{
    if (NewLevelName == CurrentLevel || NewLevelName.IsNone())
        return;

    if (!CurrentLevel.IsNone()) //vérifie si le joueur est déjà sur un level 
    {
        FLatentActionInfo LatentInfo;
        LatentInfo.CallbackTarget = this;
        LatentInfo.ExecutionFunction = FName("OnLevelUnloaded"); // callback
        LatentInfo.Linkage = 0;
        LatentInfo.UUID = __LINE__;

        // Décharge l'ancien niveau, et appelle OnLevelUnloaded() quand fini
        UGameplayStatics::UnloadStreamLevel(this, CurrentLevel, LatentInfo, false);

        // Stocke temporairement le nom du prochain niveau à charger
        NextLevel = NewLevelName;
    }
    else
    {
        LoadLevel(NewLevelName); //si il est pas sur un level je charge directement pas besoin de décharger
    }
}

//quand le level est déchargé
void ALevelStreamerActor::OnLevelUnloaded()
{
    UE_LOG(LogTemp, Warning, TEXT("Ancien niveau déchargé, on charge %s"), *NextLevel.ToString());
    LoadLevel(NextLevel);//quand le level est déchargé je charge le prochain level
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

//quand le level est chargé
void ALevelStreamerActor::OnLevelLoaded()
{
    UE_LOG(LogTemp, Warning, TEXT("Niveau %s chargé avec succès."), *CurrentLevel.ToString());
}

