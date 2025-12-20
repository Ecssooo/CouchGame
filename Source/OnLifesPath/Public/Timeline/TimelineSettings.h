// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TimelineSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName = "Timeline Settings"))
class ONLIFESPATH_API UTimelineSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(Config, EditAnywhere, Category="Timeline | Swing", BlueprintReadOnly)
	TSoftObjectPtr<UCurveFloat> SwingCurve;
	UPROPERTY(Config, EditAnywhere, Category="Timeline | Swing", BlueprintReadOnly)
	float SwingDuration;

	UPROPERTY(Config, EditAnywhere, Category="Timeline | Slide", BlueprintReadOnly)
	TSoftObjectPtr<UCurveFloat> SlideCurve;
	UPROPERTY(Config, EditAnywhere, Category="Timeline | Slide", BlueprintReadOnly)
	float SlideDuration;
	
	UPROPERTY(Config, EditAnywhere, Category="Timeline | SubLevelUp", BlueprintReadOnly)
	TSoftObjectPtr<UCurveFloat> SubLevelUp;
	UPROPERTY(Config, EditAnywhere, Category="Timeline | SubLevelUp", BlueprintReadOnly)
	float SublevelUpDuration;

	UPROPERTY(Config, EditAnywhere, Category="Timeline | SublevelDown", BlueprintReadOnly)
	TSoftObjectPtr<UCurveFloat> SubLevelDown;
	UPROPERTY(Config, EditAnywhere, Category="Timeline | SublevelDown", BlueprintReadOnly)
	float SublevelDownDuration;

	UPROPERTY(Config, EditAnywhere, Category="Timeline | SkyboxFade", BlueprintReadOnly)
	TSoftObjectPtr<UCurveFloat> SkyboxFade;
	UPROPERTY(Config, EditAnywhere, Category="Timeline | SkyboxFade", BlueprintReadOnly)
	float SkyboxFadeDuration;

	UPROPERTY(Config, EditAnywhere, Category="Timeline | BlendCameraIn", BlueprintReadOnly)
	TSoftObjectPtr<UCurveFloat> BlendCameraIn;
	UPROPERTY(Config, EditAnywhere, Category="Timeline | BlendCameraIn", BlueprintReadOnly)
	float BlendCameraInDuration;

	UPROPERTY(Config, EditAnywhere, Category="Timeline | BlendCameraOut", BlueprintReadOnly)
	TSoftObjectPtr<UCurveFloat> BlendCameraOut;
	UPROPERTY(Config, EditAnywhere, Category="Timeline | BlendCameraOut", BlueprintReadOnly)
	float BlendCameraOutDuration;

	
	UPROPERTY(Config, EditAnywhere, Category="Transition", BlueprintReadOnly)
	float WaitForBlendOut;

	UPROPERTY(Config, EditAnywhere, Category="Transition", BlueprintReadOnly)
	float WaitForRotationStart;
	
	UPROPERTY(Config, EditAnywhere, Category="Transition", BlueprintReadOnly)
	float RotationDuration;
	
};
