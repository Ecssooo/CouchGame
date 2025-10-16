// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSettings.h"

#include "EnhancedActionKeyMapping.h"
#include "InputMappingContext.h"

//Struct FLocalMultiplayerProfileData
UInputMappingContext* FLocalMultiplayerProfileData::GetIMCFromType(ELocalMultiplayerInputMappingType MappingType) const
{
	if (MappingType == ELocalMultiplayerInputMappingType::InGame) return IMCInGame;
	else return IMCMenu;
}


bool FLocalMultiplayerProfileData::ContainsKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const
{
	UInputMappingContext* MappingContext = GetIMCFromType(MappingType);
	if (!MappingContext) return false;
	
	const TArray<FEnhancedActionKeyMapping>& Mappings = MappingContext->GetMappings();
	for (const FEnhancedActionKeyMapping& Mapping : Mappings)
	{
		if (Mapping.Key == Key){return true;}
	}
	return false;
}


//Class ULocalMultiplayerSettings

int ULocalMultiplayerSettings::GetNbKeyboardProfiles() const
{
	return KeyboardProfilesData.Num();
}

int ULocalMultiplayerSettings::FindKeyboardProfileIndexFromKey(const FKey& Key,
	ELocalMultiplayerInputMappingType MappingType) const
{
	for (int i = 0; i < KeyboardProfilesData.Num(); i++)
	{
		if (KeyboardProfilesData[i].ContainsKey(Key, MappingType))
		{
			return i;
		}
	}
	return -1;
}


FLocalMultiplayerProfileData ULocalMultiplayerSettings::FindKeyboardProfileFromIndex(int PlayerIndex) const
{
	if (KeyboardProfilesData.IsValidIndex(PlayerIndex))
	{
		return KeyboardProfilesData[PlayerIndex];
	}else
	{
		return KeyboardProfilesData[0];
	}
}
