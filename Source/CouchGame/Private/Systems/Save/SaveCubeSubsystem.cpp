// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Save/SaveCubeSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Systems/InteractionID.h"
#include "Systems/Save/SaveInteractionManager.h"
#include "Systems/Save/SaveObjectManager.h"

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
	else
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

void USaveCubeSubsystem::SetInteractionHighlight(int InInteractionID, bool InIsHighlight)
{
	FInteractionsDatas& data = GetInteractionsDatasFromID(InInteractionID);

	data.IsHighlight = InIsHighlight;
	
	ASaveInteractionManager* SaveInteractionManager = Cast<ASaveInteractionManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASaveInteractionManager::StaticClass()));
	if (!SaveInteractionManager) return;
	SaveInteractionManager->UpdateInteractionHighlight();
	
	UE_LOG(LogTemp, Log, TEXT("Interaction number %d : IsHighlight is %d"), InInteractionID, data.IsHighlight);
}


#pragma endregion

#pragma region Objects

void USaveCubeSubsystem::InitObjectsDatas(TArray<FGrabObject> InObjectsDatas)
{
	nullGrabObject = {-1};
	
	AllObjectsDatas = InObjectsDatas;
	UE_LOG(LogTemp, Log, TEXT("Object datas are initialize"))
}

void USaveCubeSubsystem::SetObjectState(int ObjectID, EObjectState InObjectState, int Id)
{
	FGrabObject& data = GetGrabObjectFromID(ObjectID);
	if (data.ObjectID == -1) return;

	switch (InObjectState)
	{
		case(EObjectState::InSocket):
			{
				data.ObjectState = EObjectState::InSocket;
				data.SocketID = Id;
				break;
			}
		case(EObjectState::InSpawner):
			{
				data.ObjectState = EObjectState::InSpawner;
				data.SpawnerID = Id;
				break;
			}
		case(EObjectState::InPlayer):
			{
				data.ObjectState = EObjectState::InPlayer;
				data.PlayerID = Id;
				break;
			}
		default:
			break;
	}
}

void USaveCubeSubsystem::SetObjectState(int ObjectID, EObjectState InObjectState, int FaceID, FVector Position)
{
	FGrabObject& data = GetGrabObjectFromID(ObjectID);
	if (data.ObjectID == -1) return;

	if (InObjectState == EObjectState::InWorld)
	{
		data.ObjectState = EObjectState::InWorld;
		data.FaceID = FaceID;
		data.Position = Position;
	}
}

void USaveCubeSubsystem::SetObjectHighlight(int ObjectID, bool InIsHighlight)
{
	FGrabObject& data = GetGrabObjectFromID(ObjectID);
	if (data.ObjectID == -1) return;

	data.IsHighlight = InIsHighlight;
	data.IsSocketHighlight = InIsHighlight;

	ASaveObjectManager* SaveObjectManager = Cast<ASaveObjectManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASaveObjectManager::StaticClass()));
	if (!SaveObjectManager) return;
	SaveObjectManager->UpdateAllObjectHighlight();
}

FGrabObject& USaveCubeSubsystem::GetGrabObjectFromID(int InObjectID)
{
	for (FGrabObject& data : AllObjectsDatas)
	{
		if (data.ObjectID == InObjectID) return data;
	}
	return nullGrabObject;
}

#pragma endregion



void USaveCubeSubsystem::SetTeleporterDatas(int TeleporterID, bool InIsHighlight)
{
	FTeleporterData* data = GetTeleporterDatas(TeleporterID);
	if (!data)
	{
		UE_LOG(LogTemp, Warning, TEXT("Data is null in SetTeleporterDatas"));
		return;
	}

	data->IsHighlight = InIsHighlight;
	UE_LOG(LogTemp, Log, TEXT("Teleporter number %d : IsHighlight is %d"), TeleporterID, data->IsHighlight);
}

void USaveCubeSubsystem::SetTeleporterHighlightForFace(int NumFace)
{
	switch (NumFace)
	{
		case(1):
			SetTeleporterDatas(601, true);
			SetTeleporterDatas(602, true);
			SetTeleporterDatas(603, true);
			SetTeleporterDatas(604, true);
			SetTeleporterDatas(502, true);
			SetTeleporterDatas(404, true);
			SetTeleporterDatas(303, true);
			SetTeleporterDatas(201, true);
			return;
		case (2):
			SetTeleporterDatas(501, true);
			SetTeleporterDatas(502, true);
			SetTeleporterDatas(503, true);
			SetTeleporterDatas(504, true);
			SetTeleporterDatas(102, true);
			SetTeleporterDatas(402, true);
			SetTeleporterDatas(302, true);
			SetTeleporterDatas(602, true);
			return;
		case (3):
			SetTeleporterDatas(401, true);
			SetTeleporterDatas(402, true);
			SetTeleporterDatas(403, true);
			SetTeleporterDatas(404, true);
			SetTeleporterDatas(104, true);
			SetTeleporterDatas(204, true);
			SetTeleporterDatas(504, true);
			SetTeleporterDatas(603, true);
			return;
		case (4):
			SetTeleporterDatas(301, true);
			SetTeleporterDatas(302, true);
			SetTeleporterDatas(303, true);
			SetTeleporterDatas(304, true);
			SetTeleporterDatas(604, true);
			SetTeleporterDatas(203, true);
			SetTeleporterDatas(503, true);
			SetTeleporterDatas(103, true);
			return;
		case (5):
			SetTeleporterDatas(201, true);
			SetTeleporterDatas(205, true);
			SetTeleporterDatas(203, true);
			SetTeleporterDatas(204, true);
			SetTeleporterDatas(601, true);
			SetTeleporterDatas(301, true);
			SetTeleporterDatas(101, true);
			SetTeleporterDatas(401, true);
			return;
		case (6):
			SetTeleporterDatas(101, true);
			SetTeleporterDatas(102, true);
			SetTeleporterDatas(103, true);
			SetTeleporterDatas(104, true);
			SetTeleporterDatas(504, true);
			SetTeleporterDatas(403, true);
			SetTeleporterDatas(304, true);
			SetTeleporterDatas(205, true);
			return;
		default:
			return;
	}
}

void USaveCubeSubsystem::ResetAllTeleporterHighlight()
{
	// for (int i = 1; i < 7; i++)
	// {
	// 	for (int j = 1; j < 5; j++)
	// 	{
	// 		int TpID = i * 100 + j;
	// 		SetTeleporterDatas(TpID, false);
	// 	}
	// }
	
	SetTeleporterDatas(101, false);
	SetTeleporterDatas(102, false);
	SetTeleporterDatas(103, false);
	SetTeleporterDatas(104, false);
	SetTeleporterDatas(201, false);
	SetTeleporterDatas(205, false);
	SetTeleporterDatas(203, false);
	SetTeleporterDatas(204, false);
	SetTeleporterDatas(301, false);
	SetTeleporterDatas(302, false);
	SetTeleporterDatas(303, false);
	SetTeleporterDatas(304, false);
	SetTeleporterDatas(401, false);
	SetTeleporterDatas(402, false);
	SetTeleporterDatas(403, false);
	SetTeleporterDatas(404, false);
	SetTeleporterDatas(501, false);
	SetTeleporterDatas(502, false);
	SetTeleporterDatas(503, false);
	SetTeleporterDatas(504, false);
	SetTeleporterDatas(601, false);
	SetTeleporterDatas(602, false);
	SetTeleporterDatas(603, false);
	SetTeleporterDatas(604, false);
}

FTeleporterData* USaveCubeSubsystem::GetTeleporterDatas(int TeleporterID)
{
	for (FTeleporterData& data : TeleporterDatas)
	{
		if (data.TeleporterID == TeleporterID) return &data;
	}
	
	return nullptr;
}

void USaveCubeSubsystem::InitTeleporterDatas(TArray<FTeleporterData> InTeleporterDatas)
{
	TeleporterDatas = InTeleporterDatas;
	UE_LOG(LogTemp, Log, TEXT("Telepoter Datas are initialized"))
}
