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

bool UGrabSocketSubsystem::AddLevelData(int levelId, AGrabSocketManager* SocketManager)
{
	if (AllLevelDatas[levelId].IsDiscovered) return false;
	if (!SocketManager) return false;

	for (AGrabSocketActor* socket : SocketManager->AllGrabSocketActors)
	{
		FGrabSocketData NewSocketData = {socket->id, false};
		AllLevelDatas[levelId].GrabSocketDatas.Add(NewSocketData);
	}
	
	return true;
}

void UGrabSocketSubsystem::SaveSocketData(int LevelId, int SocketId)
{
	for (FGrabSocketData socketData : AllLevelDatas[LevelId].GrabSocketDatas)
	{
		if (socketData.Id == SocketId) socketData.HasObjectInSocket = true;
	}
}

void UGrabSocketSubsystem::LoadLevelDataInSocket(int levelId, AGrabSocketManager* SocketManager)
{
	for (FGrabSocketData socketData : AllLevelDatas[levelId].GrabSocketDatas)
	{
		if (!socketData.HasObjectInSocket) continue;

		AGrabSocketActor* socketActor = SocketManager->FindSocketFromId(socketData.Id);
		if (!socketActor) continue;

		socketActor->SpawnObjectInSocket();
	}
}
