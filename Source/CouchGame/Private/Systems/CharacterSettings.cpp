// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CharacterSettings.h"

#include "EnhancedActionKeyMapping.h"
#include "InputMappingContext.h"


int FLocalMultiplayerProfile::GetPlayerIndexFromKey(const FKey& Key) const
{
	const TArray<FEnhancedActionKeyMapping>& Mappings = IMCKeyboardP1->GetMappings();
	for (const FEnhancedActionKeyMapping& Mapping : Mappings)
	{
		if (Mapping.Key == Key){return 0;}
	}

	const TArray<FEnhancedActionKeyMapping>& Mappings2 = IMCKeyboardP2->GetMappings();
	for (const FEnhancedActionKeyMapping& Mapping : Mappings2)
	{
		if (Mapping.Key == Key){return 1;}
	}
	return -1;
}
