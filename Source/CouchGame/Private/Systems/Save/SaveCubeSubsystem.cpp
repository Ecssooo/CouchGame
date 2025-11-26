// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Save/SaveCubeSubsystem.h"

#include "Systems/InteractionID.h"

#pragma region Levels

void USaveCubeSubsystem::InitLevelData()
{
	nullSubLevelDatas = {-1, false};
	for (int i = 1; i <= 6; i++)
	{
		FLevelCube NewLevel {i};

		for (int j = 1; j <= 9; j++)
		{
			FSublevelCube NewSubLevel {j, false};
			NewLevel.SubLevels.Add(NewSubLevel);
		}
		LevelsDatas.Add(NewLevel);
		UE_LOG(LogTemp, Log, TEXT("Data for Level %d initialize"), i);
	}
}

void USaveCubeSubsystem::SaveLevelData(int idFace, int idSubLevel, bool isUnlocked)
{
	FSublevelCube& subLevel = GetSublevelDatas(idFace, idSubLevel);
	if (subLevel.SubLevelId != -1) subLevel.IsUnlocked = isUnlocked;
	
	UE_LOG(LogTemp, Log, TEXT("SubLevel : {%d, %d}; Unlocked %d"),idFace, idSubLevel, isUnlocked);
}

FSublevelCube& USaveCubeSubsystem::GetSublevelDatas(int idFace, int idSubLevel)
{
	if (LevelsDatas.IsValidIndex(idFace-1))
		if (LevelsDatas[idFace-1].SubLevels.IsValidIndex(idSubLevel-1))
			return LevelsDatas[idFace-1].SubLevels[idSubLevel-1];
	return nullSubLevelDatas;
}

#pragma endregion

#pragma region Interactions

void USaveCubeSubsystem::InitInteractionsDatas(TArray<FInteractionsDatas> InInteractionsDatas)
{
	nullInteractionsDatas = {-1, EInteractionsID::None};
	
	InteractionsDatas = InInteractionsDatas;
	UE_LOG(LogTemp, Log, TEXT("Interactions Datas are initialized"))
}

void USaveCubeSubsystem::SaveInteractionData(int InInteractionID, bool InIsCompleted)
{
	FInteractionsDatas& interactDatas = GetInteractionsDatasFromID(InInteractionID);
	if (interactDatas.InteractionIndex != -1) {
		interactDatas.IsCompleted = InIsCompleted;
		UE_LOG(LogTemp, Log, TEXT("Interactions %d; completed %d"), InInteractionID, InIsCompleted);
	}
	UE_LOG(LogTemp, Log, TEXT("Interactions %d not found"), InInteractionID);
}

FInteractionsDatas& USaveCubeSubsystem::GetInteractionsDatasFromID(int InInteractionID)
{
	
	for (FInteractionsDatas& interactionData : InteractionsDatas)
	{
		if (interactionData.InteractionIndex == InInteractionID) return interactionData;
	}

	return nullInteractionsDatas;
}


#pragma endregion

#pragma region Objects

void USaveCubeSubsystem::InitObjectsDatas(TArray<FGrabObject> InObjectsDatas)
{
	AllObjectsDatas = InObjectsDatas;
	UE_LOG(LogTemp, Log, TEXT("Object datas are initialize"))
}

bool USaveCubeSubsystem::SetObjectInSocket(int InObjectID, bool InSocket)
{
	FGrabObject* Data = GetGrabObjectFromID(InObjectID);
	if (Data)
	{
		if (InSocket) SetObjectInGrab(InObjectID, -1, false);
		return true;
	}
	return false;
}

bool USaveCubeSubsystem::SetObjectInGrab(int InObjectID, int InPlayerID, bool InGrab)
{
	FGrabObject* Data = GetGrabObjectFromID(InObjectID);
	if (Data)
	{
		if (InGrab) Data->PlayerID = InPlayerID;
		else Data->PlayerID = -1;
		return true;
	}
	return false;
}

bool USaveCubeSubsystem::SetObjectNewPosition(int InObjectID, FVector InPosition, int idFace)
{
	FGrabObject* Data = GetGrabObjectFromID(InObjectID);
	if (Data)
	{
		Data->Position = InPosition;
		Data->FaceID = idFace;
		return true;
	}
	return false;
}

FGrabObject* USaveCubeSubsystem::GetGrabObjectFromID(int InObjectID)
{
	for (FGrabObject& data : AllObjectsDatas)
	{
		if (data.ObjectID == InObjectID) return &data;
	}
	return nullptr;
}


#pragma endregion
