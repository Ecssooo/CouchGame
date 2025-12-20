#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Sound/SoundBase.h"
#include "UObject/NameTypes.h"
#include "SoundDataAsset.generated.h"

UENUM(BlueprintType)
enum class ESoundType : uint8
{
	SFX     UMETA(DisplayName = "SFX"),
	Music   UMETA(DisplayName = "Music"),
	Voice   UMETA(DisplayName = "Voice")
};

USTRUCT(BlueprintType)
struct FSoundDataStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SoundName; //nom du son

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> SoundFile; //le fichier du son

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESoundType SoundType; //le type du son

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool SoundLoop; // si le son est un loop ou pas

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float VolumeMultiplier = 1.0f; // le volume du son souhaitez

	// Constructeur par defaut pour mettre des valeurs
	FSoundDataStruct()
	{
		SoundName = NAME_None; 
		SoundType = ESoundType::SFX;
		SoundFile = nullptr;
		SoundLoop = false;
		VolumeMultiplier = 1.0f;
	}
};

UCLASS()
class COUCHGAME_API USoundDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	USoundDataAsset();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Library")
	TArray<FSoundDataStruct> SoundList;  
};
