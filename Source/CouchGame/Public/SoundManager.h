#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundDataAsset.h"
#include "UObject/SoftObjectPtr.h"
#include "SoundManager.generated.h"

UCLASS(Config=Game) //pour lire le fichier game ini
class COUCHGAME_API USoundManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    USoundManager();

    virtual void Initialize(FSubsystemCollectionBase& Library) override; // charge la biblioteque de son soundatalibrary

    UFUNCTION(BlueprintCallable, Category = "Sound")
    bool PlaySound(FName SoundName, UAudioComponent* AudioComponent);

    UFUNCTION(BlueprintCallable, Category = "Sound")
    bool StopSound(UAudioComponent* AudioComponent); // pas besoin du sound name car je vais stop l audio component

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float VolumeMultiplierGlobal = 1.0f; // le volume du son global

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float VolumeMultiplierSFX = 1.0f; // le volume du son sfx

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float VolumeMultiplierVoice = 1.0f; // le volume du son voice

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float VolumeMultiplierMusic = 1.0f; // le volume du son music

protected:
    UPROPERTY(Config) 
    TSoftObjectPtr<USoundDataAsset> SoundLibraryPath;  //stock le chemin vers mon DA_Soundlibrary

    UPROPERTY(Transient) 
    TObjectPtr<USoundDataAsset> LoadedSoundLibrary;   //stock l'asset apres load
};

