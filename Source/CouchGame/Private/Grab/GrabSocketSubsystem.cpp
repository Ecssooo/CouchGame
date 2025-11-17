// Fill out your copyright notice in the Description page of Project Settings.


#include "Grab/GrabSocketSubsystem.h"

#include "Grab/GrabSocketActor.h"
#include "Grab/GrabSocketManager.h"

void UGrabSocketSubsystem::InitLevelDatas()
{
	for (int i = 0; i < 6; i++)
	{
		FGrabSocketLevelData newLevel = {i};
		AllLevelDatas.Add(newLevel);		
	}
	
}

bool UGrabSocketSubsystem::AddLevelData(int levelId, AGrabSocketManager* InSocketManager)
{
	if (AllLevelDatas[levelId].IsDiscovered)
	{
		LoadLevelDataInSocket(levelId, InSocketManager);
		return false;
	}
	if (!InSocketManager) return false;
	for (AGrabSocketActor* socket : InSocketManager->AllGrabSocketActors)
	{
		FGrabSocketData NewSocketData = {socket->id, false};
		AllLevelDatas[levelId].GrabSocketDatas.Add(NewSocketData);
	}
	AllLevelDatas[levelId].IsDiscovered = true;
	LoadLevelDataInSocket(levelId, InSocketManager);
	return true;
}

void UGrabSocketSubsystem::SaveSocketData(int LevelId, int SocketId)
{
	for (FGrabSocketData& socketData : AllLevelDatas[LevelId].GrabSocketDatas)
	{
		if (socketData.Id == SocketId)
		{
			socketData.HasObjectInSocket = true;
		}
	}
}

void UGrabSocketSubsystem::LoadLevelDataInSocket(int levelId, AGrabSocketManager* InSocketManager)
{
	for (FGrabSocketData& socketData : AllLevelDatas[levelId].GrabSocketDatas)
	{
		AGrabSocketActor* socketActor = InSocketManager->FindSocketFromId(socketData.Id);
		if (!socketActor) continue;
		socketActor->SpawnObject(socketData.HasObjectInSocket);
	}
}
